/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */
#include "components/locus.h"

namespace gff
{

Locus::Locus(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, position start, position end)
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
