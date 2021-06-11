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
  Locus::Locus(gff::GffEntry& e)
  :locus(e)
  {
      // std::cout << "Ceated: " << this->end << "\n";
  }

  Locus::~Locus()
  {
    // std::cout <<"Destructing\t" << this->end << " \n";
  }

  void Locus::show()
  {

    locus.show();
    show_features();
  }
  void Locus::show_features()
  {
    for(auto& i: features)
    {
      std::cout << "\tFeature: " << i.first << "\n";
      i.second.show();
    }
  }

} // namespace gff

