/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#include "components/typefeature.h"

namespace gff
{

  TypeFeature::TypeFeature(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, position start, position end)
  : Feature{seqid, id, source, type, start, end}
  {
    // std::cout << "INIT: "<<  e.start << "\t"  << e.end << "\t" << this->start << "\t" << this->end << "\n";
  }

  TypeFeature::~TypeFeature()
  {
    // std::cout <<"Destructing\t" << this->end << " \n";
  }
}
