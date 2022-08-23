/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */
#include "components/locus.h"

namespace gff
{

Locus::Locus(const std::string& id, const std::string& type, std::int_fast32_t start, std::int_fast32_t end)
//  : id(id), type(type), positions{ {start, end} }
: Feature{id, type, start, end}
{
  // std::cout << "INIT: "<<  e.start << "\t"  << e.end << "\t" << this->start << "\t" << this->end << "\n";
}

Locus::~Locus()
{
  // std::cout <<"Destructing\t" << this->end << " \n";
}

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

void Locus::add_feature(const gff::GffRow& row)
{
  gff::Feature* feature = new gff::Feature(row.id, row.type, row.start, row.end);
  // if(features.contains(feat.type))
  // {
  //   const auto &[it, pass] = features[e.feature()].try_emplace(feat.parent_id, feat);
  //   if(!pass)
  //     {it->second.update_coords(e);}
  // }
  // else
  // {
  //  features.insert({feat.type, std::unordered_map<std::string, Locus::Feature> {{feat.parent_id, feat}}});
  // }
}
/*
bool Locus::hasFeature(const std::string& level)
  {return features.contains(level);}

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

Locus::Feature::Feature(GffEntry& e)
 : id(e.id()), type(e.feature()), parent_id(e.parent()), start(e.start()), end(e.end())
{
  update_length(e.length());
  entries.push_back(e);
}

Locus::Feature::Feature(const std::string& parent_id)
 : parent_id(parent_id)
{ }

Locus::Feature::~Feature(){ }

const std::int_fast32_t Locus::Feature::length() const
  {return feat_length;}

void Locus::Feature::update_coords(GffEntry e)
{
  update_length(e.length());
  if(e.start() < start)
    {start = e.start();}
  if(e.end() > end)
    {end = e.end();}
  add_entry(e);
}

void Locus::Feature::update_length(std::int_fast32_t entry_len)
  {feat_length += entry_len;}

void Locus::Feature::add_entry(GffEntry e)
  {entries.push_back(e);}

void Locus::Feature::show_entries()
{
  for(auto& i : entries)
  {
    i.show();
  }
}

void Locus::Feature::show()
{
  std::cerr << "\n\t" "Feature: " << id << "\t" << type << "\t"
            << start << "-" << end << "\t" << length() << "\n\tEntries:\n";
  show_entries();
}

const std::vector<std::string> Locus::Feature::get_comment(const std::string& key) const
  {return entries.front().get_comment(key);}

const std::string& Locus::Feature::sequence() const
  {return entries.front().sequence();}

bool Locus::Feature::hasComment(const std::string& commentkey)
{
  return entries.front().hasComment(commentkey);
}

const std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>>& Locus::featuremap()
{
  return features;
}
*/
} // namespace gff
