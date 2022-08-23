/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#include "components/typefeature.h"

namespace gff
{

  TypeFeature::TypeFeature(const std::string& id, const std::string& type, std::int_fast32_t start, std::int_fast32_t end)
  //  : id(id), type(type), positions{ {start, end} }
  : Feature{id, type, start, end}
  {
    // std::cout << "INIT: "<<  e.start << "\t"  << e.end << "\t" << this->start << "\t" << this->end << "\n";
  }

  TypeFeature::~TypeFeature()
  {
    // std::cout <<"Destructing\t" << this->end << " \n";
  }
}
