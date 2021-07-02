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

Locus::Locus(gff::GffEntry e)
: feature(e)
{
  // std::cout << "INIT: "<<  e.start << "\t"  << e.end << "\t" << this->start << "\t" << this->end << "\n";
}

Locus::~Locus()
{
  // std::cout <<"Destructing\t" << this->end << " \n";
}

const std::string& Locus::id()
{
  return feature.id();
}

void Locus::show_features()
{
  for(auto& i : features)
  {
    for(auto& j : i.second)
    {
      std::cout << "\t" << j.second.entries.front().id() << "\n\t" << j.second.type << ": " << j.second.start << "-" << j.second.end
                << "\n\tlen: " << j.second.end - j.second.start + 1 << "\n";
    std::cout << "\n";
    }
  }
}

void Locus::show()
{
  std::cout << "Locus: "<< id() << "\n\tType: " << feature.feature << "\n" << "\tCoords: " << start() << "-"
            << end() << "\n";
  show_features();
}

void Locus::add_feature(gff::GffEntry e)
{
  gff::FeatureA fa = {e.feature, e.parent(), e.start, e.end, std::vector<gff::GffEntry>{e}};
  if(features.contains(e.feature))
  {
    auto [it, success] = features[e.feature].try_emplace(e.parent(), fa);
    if(!success)
    {
      it->second.entries.push_back(e);
      if(e.start < it->second.start)
      {
        it->second.start = e.start;
      }
      if(e.end > it->second.end)
      {
        it->second.end = e.end;
      }
    }
  }
  else
  {
   features.insert({e.feature, std::unordered_map<std::string, gff::FeatureA> {{e.parent(), fa}} });
  }
}

void Locus::set_longest_feature(const std::string& level)
{
  if(features.contains(level))
  {
    for(auto& i : features[level])
    {
      if( (longest_feat.end - longest_feat.start + 1) < (i.second.end - i.second.start + 1))
      {
        longest_feat = i.second;
      }
      // std::cout << i.second.type << "\tlen: " << i.second.end - i.second.start + 1 << "\n";
    }
  }
}

const std::int_fast32_t Locus::start()
{
  return feature.start;
}

const std::int_fast32_t Locus::end()
{
  return feature.end;
}

} // namespace gff

