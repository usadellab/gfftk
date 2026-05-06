/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#include "gff/gfffile.h"

#include "gff/gffentry.h"
#include "gff/gffindex.h"
#include "summaries/summaries.h"
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
    std::cerr << "[ Info ] Closing GFF: " << inpath << "\n";
    gff_in.close();
  }
}

std::unordered_map<std::string, int> GffFile::synth_entries_count = {};
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
  if(entry.id.empty()) { make_synthetic_id(entry); }
}

std::istream& operator>>(std::istream& is, GffEntry& e)
{
  std::string score = "";
  std::string strand = "";
  std::string phase = "";
  std::string attributes = "";
  std::string type = "";

  is >> e.seqname >> e.source >> type >> e.beg >> e.end >> score >> strand
    >> phase >> attributes;

  if(is.fail()) { return is; } // do smomething with errors here

  e.score = score_to_float(score);
  e.strand = strand_to_int(strand);
  e.phase = phase_to_int(phase);
  e.type = stringtools::lowercase(type);
  parse_attributes(attributes, e);
  return is;
}

void make_synthetic_id(gff::GffEntry& e)
{
  e.id = e.type + std::to_string(++GffFile::synth_entries_count[e.type]);
  e.has_id = false;
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

std::vector<gff::GffEntry*> GffFile::find_all_subtypes_for_id(
  const std::string& id, const std::string& feat)
{
  return index.descendants_of_type(id, feat);
}
std::vector<gff::GffEntry*> GffFile::find_all_parents()
{
  return index.find_all_parents();
}

void GffFile::find_root_for_id(const std::string& id)
{
  GffEntry* root = index.root_of_feat(id);
  show_gffentry(*root);
}

std::vector<gff::GffEntry*> GffFile::find_type_for_id(const std::string& id,
                                                      const std::string& type)
{
  return index.children_of_feat(id, type);
}

void GffFile::show_gffentry(const GffEntry& e)
{
  std::cout << e.seqname << "\t" << e.source << "\t" << e.type << "\t"
            << std::to_string(e.beg) << "\t" << std::to_string(e.end) << "\t"
            << e.strand << "\t" << std::to_string(e.end) << "\t" << e.id << "\t"
            << e.parent.value_or("None") << "\n";
}

std::vector<gff::GffEntry*> GffFile::find_all_of_type(const std::string& type)
{
  return index.find_all(type);
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
  return 0;
}

std::vector<gff::GffSelectedEntry>& GffFile::find_longest_type(
  const std::string& type)
{
  return index.longest_per_root(type);

  // for(const auto& e : result)
  // {
  //   std::cout << e.seqname << "\t" << e.parent.value_or("None") << "\t" <<
  //   e.id
  //             << "\t" << index.total_length(e) << "bp\t" << e.coords.size()
  //             << " parts\n";
  //   for(const auto& c : e.coords)
  //   {
  //     std::cout << c.beg << "\t" << c.end << "\n";
  //   }
  // }
}

std::vector<gff::GffSelectedEntry>& GffFile::find_shortest_type(
  const std::string& type)
{
  return index.shortest_per_root(type);
}

void GffFile::find_longest_type_with(const std::string& type,
                                     const std::string& sum_by)
{
  auto summary = index.type_lengths();
  for(const auto& tl : summary)
  {
    // print summary
    for(const auto& [type, len] : tl.total)
      std::cout << tl.root_id << "\t" << type
                << "  count: " << tl.count.at(type) << "  total: " << len
                << "bp\n";
    // longest mRNA directly after summary
    auto* longest = index.longest_of(tl, "mrna", "cds");
    if(longest)
      std::cout << "  longest " << type << ": " << longest->id << " "
                << index.total_length(longest->id) << "bp\n--------------\n";
  }
}
void GffFile::clean_up() { row_num = 0; }

gff::GffSummary GffFile::summarize() const { return index.summarize(); }
} // end namespace gff
