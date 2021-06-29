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
  for(auto& i: features)
  {
    std::cout << "\tFeature: " << i.first << "\n";
    // i.second.show();
  }
}

void Locus::show()
{
  std::cout << "Locus: "<< id() << "\n";
  // show_features();
}

void Locus::add_feature(gff::GffEntry e)
{
  features.insert({e.id(), e});
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

