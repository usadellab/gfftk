/*
 *------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 *------------------------------------------------------------------------------
 */

#include "gff/gfffile.h"

#include "gff/gffentry.h"
#include "utils/stringtools.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace gff
{

GffFile::GffFile(std::string gff_file)
  : inpath(gff_file)
{
  open();
}

GffFile::~GffFile()
{
  clean_up();
  // std::cerr << "[ Info ] Finished parsing: " << path << "\n";
}

void GffFile::close()
{

  if(gff_in.is_open())
  {
    std::cerr << "[ Info ] Closing GFF: " << inpath << "\t";
    gff_in.close();
  }
}

std::filesystem::path GffFile::path() const { return inpath; }

void GffFile::open()
{
  std::error_code ec;

  if(!std::filesystem::exists(inpath, ec))
  {
    if(ec) throw GffException(ec.message());
    throw GffFileNotFound(inpath);
  }

  if(!std::filesystem::is_regular_file(inpath, ec))
    throw GffException(inpath + " : Not a regular file");

  gff_in.open(inpath);
  if(!gff_in.is_open()) throw GffOpenError(inpath);
}

static float score_to_float(const std::string& value)
{
  if(value == ".") { return 0.0; }
  return std::stof(value);
}

static int phase_to_int(const std::string& value)
{ // 0 fwd, 1: rev, 2: . 3: ? 4: bad
  if(value == ".") { return 3; }
  return std::stoi(value);
}

static int strand_to_int(std::string& value)
{ // 0 fwd, 1: rev, 2: . 3: ? 4: bad
  if(value == "+") { return 0; }
  if(value == "-") { return 1; }
  if(value == ".") { return 2; }
  if(value == "?") { return 3; }
  std::cerr << "Unknwon stradndess:  " << value << "\n";
  return 4;
}

void parse_attributes(const std::string& attribs, gff::GffEntry& entry)
{
  std::istringstream ss(attribs);
  std::string token;

  while(std::getline(ss, token, ';'))
  {
    auto [key, value] = stringtools::split_kv(token, '=');
    if(key == "ID") { entry.id = value; }
    else if(key == "parent") { entry.parent = value; }
    else
    {
      if(!key.empty()) entry.attributes[key] = value;
    }
  }
}

std::istream& operator>>(std::istream& is, GffEntry& e)
{
  std::string score = "";
  std::string strand = "";
  std::string phase = "";
  std::string attributes = "";

  is >> e.seqname >> e.source >> e.feature >> e.beg >> e.end >> score >> strand
    >> phase >> attributes;
  if(is.fail()) return is; // do smomething with errors here
  e.score = score_to_float(score);
  e.strand = strand_to_int(strand);
  e.phase = phase_to_int(phase);
  parse_attributes(attributes, e);
  return is;
}

int GffFile::parse()
{
  for(std::string line; std::getline(gff_in, line);)
  {
    ++row_num;
    if(line.empty()) { continue; }
    if(line[0] == '#')
    {
      // parse_directive(line); // parse header/comments here
      continue;
    }
    gff::GffEntry entry;
    std::stringstream ss(line);
    if(!(ss >> entry)) { std::cerr << "Bad GFF line: " << row_num << "\n"; }
    std::cout << entry.seqname << "\t" << entry.source << "\t" << entry.feature
              << "\t" << std::to_string(entry.beg) << "\t"
              << std::to_string(entry.end) << "\t"
              << std::to_string(entry.score) << "\t" << entry.strand << "\t"
              << entry.id << "\t" << entry.parent.value_or("None") << "\n";
  }
  return 0;
}

void GffFile::clean_up() { row_num = 0; }

} // end namespace gff
/*

void GffFile::parse_directive(const std::string& line)
{
  int status = 0;
  int directive_beg;
  for(long unsigned int i = 0; i < line.size(); ++i)
  {
    // std::cout << i << " : " << line[i] << " " << status << "\n";
    switch(status)
    {
      case(0):
        if(line[i] != '#')
        {
          status = 1;
          break;
        }
        ++i;

      case(1):
        if(line[i] != '#' || line[i] != '!')
        {
          ++i;
          directive_beg = i;
          status = 2;
          break;
        }
        ++i;

      case(2):
        if(line[i] == ' ')
        {
          std::string directive = line.substr(directive_beg, i - 1);
          std::string val = line.substr(i + 1);
          const auto& [it, pass] = directives.try_emplace(
            stringtools::strip(directive),
            std::vector<std::string>{stringtools::strip(val)});
          if(!pass) { it->second.push_back(line.substr(i + 1)); }
          return;
        }
        continue;
      default:
        return;
    }
  }
}

gff::TypeFeature* GffFile::add_feature(const gff::GffRow& row)
{
  gff::TypeFeature* feat = new gff::TypeFeature(
    row.seqid, row.id, row.source, row.type, row.score, row.strand, row.phase,
    row.start, row.end, row.attributes);
  // std::cerr << "[ Info ] " << feat->id << "\t" << feat->type << "\t";
  for(const auto& i : row.parents)
  {
    feat->add_parent(get_feature(i));
  }

  // Try adding new feature
  const auto& [it, inserted] = features.try_emplace(feat->id, feat);
  if(inserted) // feature does not exist at locus
  {
    // std::cerr << "\tinserted\n";
    feat->sort_coords();
    return feat;
  }
  // Feature is already known. If it is a duplicate, delete currrent feature
  // and warn
  if(feat->is_duplicate(it->second)) // check for duplicate entry
  {
    std::cerr << "[ Warning ] identified likely identical features" << path
              << "::" << row_num << " " << it->second->id << " and " <<
feat->id
              << "\n";
    delete feat;
    return nullptr;
  }
  // std::cerr << "\textending with " << it->second->id << "\n";
  // Feature is already known. Extending with new part of same feature.
  // If this fails, delete feature part and emit warning
  if(!it->second->extend_with(feat))
  {
    std::cerr << "[ Warning ] extending " << it->second << " with " <<
feat->id
              << "failed\n";
    delete feat;
    return nullptr;
  }
  // Feature is already known and has been extended with new part. Delete
  // feature part as its info has been used in the extension.
  // Sort coords and return extended feauture
  delete feat;
  it->second->sort_coords();
  return it->second;
}

gff::TypeFeature* GffFile::get_feature(const std::string& id)
{
  if(features.count(id)) { return features[id]; }
  std::cerr << "[ Warning ] feature " << id << " not found\n";
  return nullptr;
}

gff::TypeFeature* GffFile::assemble_locus(gff::TypeFeature* feat)
{
  if(feat->is_locus())
  {
    if(prev_loc)
    {
      gff::TypeFeature* loc = prev_loc;
      prev_loc = feat;
      return loc;
    }
    prev_loc = feat;
  }
  return nullptr;
}

*/

/*
void GffFile::empty_features()
{
  // std::cerr << features.size() << " features \n";
  for(auto it = features.cbegin(); it != features.cend();)
  {
    // std::cerr << "[ Info ]\tDeleting: " << it->first << "\n";
    delete (it->second);
    it++;
  }
}
  */
