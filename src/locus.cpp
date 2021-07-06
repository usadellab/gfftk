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
  std::cout << "Locus: "<< id() << "\tType: " << loc_feature.feature() << "\tCoords: " << start() << "-"
            << end() << "\n";
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
    std::cout << "\n";
  }
}

void Locus::add_feature(GffEntry e)
{
  gff::Locus::Feature feat(e);
  if(features.contains(e.feature()))
  {
    const auto &[it, pass] = features[e.feature()].try_emplace(e.parent(), feat);
    if(!pass)
    {
      // std::cout << "Checking for update: " << feat.parent_id << " : " << feat.start << " with " << e.id() << " : " << it->second.length() << "\n";
      it->second.update(e);
    }
  }
  else
  {
   features.insert({e.feature(), std::unordered_map<std::string, Locus::Feature> {{e.parent(), feat}}});
  //  std::cout << "New Feature init with  "<< e.id() <<"\n";
  //  feat.show();
  //  std::cout << "\t\t++++++++\n";
  }
}

bool Locus::hasFeature(const std::string& level)
{
  return features.contains(level);
}

// compare foi2 entries with new feature
void Locus::find_longest_feature(const std::string& level)
{
  std::cout << "Finding longest " << level << " on " << id() << "\n";
  if(features.empty())// no feature per locus
  {
    std::cout << "No features on locus " << id();
    return;
  }
  if(!hasFeature(level))
  {
    std::cout << "No " << level << " features on locus " << id();
    return;
  }

  Locus::Feature* long_feat = nullptr;
  for(auto& i : features[level])
  {
    if(long_feat)
    {
      if(long_feat->length() < i.second.length())
      {
        long_feat = &i.second;
      }
    }
    else
    {
      long_feat = &i.second;
    }
  }
  if(long_feat)
  {
    std::cout << "Longest " << long_feat->type << " on " << id() << "\n";
    long_feat->show();
    long_feat->show_entries();
  }
}

Locus::Feature::Feature(GffEntry& e)
 : id(e.id()), type(e.feature()), parent_id(e.parent()), start(e.start()), end(e.end())
{
  update_length();
  entries.push_back(e);
}

Locus::Feature::~Feature(){ }

std::int_fast32_t Locus::Feature::length() {return feat_length;}

void Locus::Feature::update(GffEntry e)
{
  if(e.start() < start)
  {
    start = e.start();
    update_length();
  }
  if(e.end() > end)
  {
    end = e.end();
    update_length();
  }
  add_entry(e);
}

void Locus::Feature::update_length()
{
  feat_length = end - start + 1;
}

void Locus::Feature::add_entry(GffEntry e)
{
  entries.push_back(e);
}

void Locus::Feature::show_entries()
{
  for(auto& i : entries)
  {
    i.show();
  }
}

void Locus::Feature::show()
{
  std::cout << "\t" "id: " << id << "\t" << type << "\t"
            << start << "-" << end << "\t" << length() << "\n";
}
} // namespace gff

