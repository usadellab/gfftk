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
  // std::cout << "\tFeatures: " << features.size() <<"\n";
  // for(auto& i: features)
  // {
  //   i.show();
  // }

  for(auto& i : featuresa)
  {
    std::cout << i.first << "\n";
    for(auto& j : i.second)
    {
      // std::cout << i.first << "\n";
      for(auto& k : j.second)
      {
        std::cout << k.id() << "\n";
      }
      std::cout << "\n\n";
    }
  }

}

void Locus::show()
{
  std::cout << "Locus: "<< id() << "\n" << "\tCoords: " << start() << "-" << end() << "\n";
  show_features();
}

void Locus::add_feature(gff::GffEntry e)
{
  // // std::cout << "Par: " << e.id() << " " << e.get_parent().id() << "   " << e.parent() << "\n";
  // if(featuresl.contains(e.feature))
  // {
  //   featuresl[e.feature].push_back(e);
  // }
  // else
  // {
  //   featuresl.insert(std::pair<std::string, std::vector<gff::GffEntry>> (e.feature, {e}));
  // }

  if(featuresa.contains(e.feature))
  {
    // std::unordered_map<std::string, std::vector<gff::GffEntry>> f = { {e.parent(), std::vector<gff::GffEntry>{e}} };
    auto [it, success] = featuresa[e.feature].try_emplace(e.parent(), std::vector<gff::GffEntry>{e});
    if(!success)
    {
      it->second.push_back(e);
      // std::cout << "Etry: " << it->first << " : " << it->second.back().id() << "\n";
    }
  }
  else
  {
    // featuresl.insert(std::pair<std::string, std::vector<gff::GffEntry>> (e.feature, {e}));
    // featuresa.insert({e.feature, std::unordered_map<std::string, std::vector<gff::GffEntry>> {} });
    featuresa.insert({e.feature, std::unordered_map<std::string, std::vector<gff::GffEntry>>{ {e.parent(), std::vector<gff::GffEntry>{e}}} });
    // std::cout << "\tNew feature: " << e.feature << "\tid: " << e.id() << "\tparent: " << e.parent() <<"\n";
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

