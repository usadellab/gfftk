/*
 *------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 *------------------------------------------------------------------------------
 */

#include "gff/gfffile.h"

#include "gff/gffentry.h"
#include "gff/gffindex.h"
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
  std::cerr << "Unknwon strandness: " << value << "\n";
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
    else if(key == "Parent") { entry.parent = value; }
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

  is >> e.seqname >> e.source >> e.type >> e.beg >> e.end >> score >> strand
    >> phase >> attributes;
  if(is.fail()) return is; // do smomething with errors here
  e.score = score_to_float(score);
  e.strand = strand_to_int(strand);
  e.phase = phase_to_int(phase);
  parse_attributes(attributes, e);
  return is;
}

void GffFile::find_by_id(const std::string& id)
{
  if(GffEntry* feature = index.find(id)) { show_gffentry(*feature); }
}

void GffFile::find_direct_subtypes_for_id(const std::string& id)
{
  auto all_children = index.children_of_feat(id);
  for(auto& e : all_children)
  {
    show_gffentry(*e);
  }
}

// finds only depth 1
void GffFile::find_all_subtypes_for_id(const std::string& id,
                                       const std::string& feat = "")
{
  std::vector<gff::GffEntry*> all_children
    = index.descendants_of_feat(id, feat);
  for(auto& e : all_children)
  {
    show_gffentry(*e);
  }
}

void GffFile::find_root_for_id(const std::string& id)
{
  GffEntry* root = index.root_of_feat(id);
  show_gffentry(*root);
}

void GffFile::find_type_for_id(const std::string& id, const std::string& type)
{
  std::vector<gff::GffEntry*> exons = index.children_of_feat(id, type);
}

void GffFile::show_gffentry(const GffEntry& e)
{
  std::cout << e.seqname << "\t" << e.source << "\t" << e.type << "\t"
            << std::to_string(e.beg) << "\t" << std::to_string(e.end) << "\t"
            << e.strand << "\t" << std::to_string(e.end) << "\t" << e.id << "\t"
            << e.parent.value_or("None") << "\n";
}

void GffFile::find_all_of_type(const std::string& type)
{
  std::vector<gff::GffEntry*> types = index.find_all(type);
  for(auto& e : types)
  {
    show_gffentry(*e);
  }
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
    entries.push_back(std::move(entry));
  }
  index.build(entries);
  // find_by_id("gene-LOC101263636");
  // find_direct_subfeatures_for_id("rna-XM_004228713.4");
  // find_all_subfeatures_for_id("gene-LOC101263636");
  // find_root_for_id("exon-XM_004228895.4-1");
  // find_all_of_type("gene");
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
*/
