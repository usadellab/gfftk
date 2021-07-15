/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include <iostream>

#include "locus.h"
#include "gffentry.h"
#include "feature.h"

namespace gff
{

Locus::Locus(GffEntry e)
 : loc_feature(e)
{
  // std::cout << "INIT: "<<  e.start << "\t"  << e.end << "\t" << this->start << "\t" << this->end << "\n";
}

Locus::~Locus()
{
  // std::cout <<"Destructing\t" << this->end << " \n";
}

const std::string& Locus::id(){return loc_feature.id();}
const std::int_fast32_t Locus::start(){return loc_feature.start();}
const std::int_fast32_t Locus::end(){return loc_feature.end();}

void Locus::show()
{
  std::cerr << "Locus: "<< id() << "\tType: " << loc_feature.feature()
            << "\tCoords: " << start() << "-" << end() << "\n";
  show_features();
}

void Locus::show_features()
{
  for(auto& i : features)
  {
    for(auto& j : i.second)
    {
      j.second.show();
    }
    std::cerr << "\n";
  }
}

void Locus::add_entry(GffEntry e)
{
  if(e.id().empty()) //  No ID for entry. Possible, e.g. ENSEMBLE gff's
    {return;}
  gff::Locus::Feature feat(e);
  if(features.contains(feat.type))
  {
    const auto &[it, pass] = features[e.feature()].try_emplace(feat.id, feat);
    if(!pass)
      {it->second.update_coords(e);}
  }
  else
  {
   features.insert({feat.type, std::unordered_map<std::string, Locus::Feature> {{feat.id, feat}}});
  }
}

bool Locus::hasFeature(const std::string& level)
  {return features.contains(level);}

void Locus::find_longest_feature(const std::string& level)
{
  std::cerr << "Finding longest " << level << " on " << id() << "\n";
  if(features.empty())// no feature per locus
  {
    std::cerr << "No features on locus " << id() << "\n";
    return;
  }
  if(!hasFeature(level))
  {
    std::cerr << "No " << level << " features on locus " << id();
    return;
  }

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
    std::cerr << "Longest " << long_feat->type << " on " << id() << "\n";
    long_feat->show();
    std::cout << long_feat->id         << "\t" << long_feat->type  << "\t"
              << long_feat->sequence() << "\t" << long_feat->start << "\t"
              << long_feat->end        << "\t" << long_feat->length();
    if(long_feat->hasComment("protein_id"))
    {
      for(auto& i : long_feat->get_comment("protein_id"))
      {
        std::cout << "\t" << i;
      }
    }
    if(long_feat->hasComment("locus_tag"))
    {
      for(auto& i : long_feat->get_comment("locus_tag"))
      {
        std::cout << "\t" << i;
      }
    }
    std::cout << "\n";
    // std::vector<std::string> c = long_feat->get_comment("protein_id");
    // vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
    // for(auto& i : c)
    // {
      // std::cout << "Represent:" << i << "\t" << long_feat->sequence() << "\n";
    // }
    // long_feat->show_entries();
  }
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

std::int_fast32_t Locus::Feature::length()
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
  std::cerr << "\t" "Feature: " << id << "\t" << type << "\t"
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

} // namespace gff
