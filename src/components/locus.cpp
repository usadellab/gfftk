/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */
#include "components/locus.h"

namespace gff
{

Locus::Locus(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, std::int_fast32_t start, std::int_fast32_t end)
: Feature{seqid, id, source, type, start, end}
{ }

Locus::~Locus()
{ }

void Locus::show()
{
  std::cerr << "Locus: "<< id << "\tType: " << type << "\tCoords:";
  for(const auto& i : positions)
  {
    std::cerr << "\t" << i.start << "-" << i.end << "\n";
  }
  // show_features();
}

void Locus::extend_with_row(const gff::GffRow& row)
{
  for(const auto& i : positions)
  {
    if(i.start == row.start && i.end == row.end)
    {
      std::cerr << "[ Warning ] Duplicate row detected while extending locus: "
                <<  id << "\trow: " <<  row.id << "\n";
      return;
    }
  }
  add_positions(row.start, row.end);
  std::cerr << "[ Info ] Extended locus: " << id << " with row: "
            << row.id << " ("<< row.rownum << ")\n";
}

gff::Feature* Locus::add_feature(const gff::GffRow& row)
{
  std::cout << row.id << "\t" << row.type << "----------------------\n";
  const auto &[it, inserted] = features.try_emplace(row.type, std::unordered_map<std::string, gff::Feature*>{});
  if(inserted)  // feature type and feature do not exis at locus
  {
     // 0. Add new feature type, e.g. mRNA
     // 1. Add new feature to specified type
     std::cout << "inserted new feature " << row.id << "\n";
     gff::TypeFeature* feat = new gff::TypeFeature(row.seqid, row.id, row.source, row.type, row.start, row.end);
     const auto &[it, inserted] = features[row.type].emplace(row.id, feat);
     if(inserted)
     {
       std::cout << "inserted feature " << feat->id << " in " << feat->type << "\n";
     }
     else
     {
       std::cout << "insertion of feature " << feat->id << " in " << feat->type << "failed\n";
     }
   }
  else // feature exists at locus
  {
    // 0. Get feature type entry
    // 1. Get paret either same ID or same parent(s)
    // 2. extend feature
    std::cout << "EXISTING FEATURE: " << row.id << "\t" << row.type << "\n";
  }

  for(const auto& i: features)
    {
      std::cout << "\t\t" << i.first << "\n";
      for(const auto& j : i.second)
      {
        std::cout << j.first << "\t" << j.second->id << "\n";
      }
    }
}
/*

Locus::Feature* Locus::find_longest_feature(const std::string& level)
{
  std::cerr << "Finding longest " << level << " on " << id();
  if(features.empty())  // no feature per locus
    {std::cerr << ": locus without features";}
  if(!hasFeature(level))
    {std::cerr << "no " << level << " features";}
  std::cerr << "\n";

  Locus::Feature* long_feat = nullptr;
  for(auto& i : features[level])
  {
    if(long_feat)
    {
      if(long_feat->length() < i.second.length())
        {long_feat = &i.second;}
    }
    else
      {long_feat = &i.second;}
  }
  if(long_feat)
  {
    long_feat->isSelected = true;
  }
  return long_feat;
}


void Locus::Feature::update_coords(GffEntry e)
{
  update_length(e.length());
  if(e.start() < start)
    {start = e.start();}
  if(e.end() > end)
    {end = e.end();}
  add_entry(e);
}
*/

} // namespace gff
