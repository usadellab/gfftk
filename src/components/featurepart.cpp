/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/featurepart.h"

#include <iostream>

namespace gff
{
  GffFeaturePart::GffFeaturePart(std::string id,
                     std::vector<std::string> parents,
                     std::string seqid,
                     std::string source,
                     std::string type,
                     std::int_fast32_t start,
                     std::int_fast32_t end,
                     float score,
                     int strand,
                     int phase,
                     std::unordered_map<std::string, std::vector<std::string>> attributes,
                     int err_code)
    : id(id), parents(parents), seqid(seqid), source(source),type(type),
      start(start), end(end), score(score), strand(strand), phase(phase),
      attributes(attributes), error_code(err_code)
  { }

  GffFeaturePart::~GffFeaturePart()
  { }

  bool GffFeaturePart::has_parent()
  {
    return parents.empty();
  }
  bool GffFeaturePart::has_error()
  {
    return error_code != 0;
  }

  void GffFeaturePart::set_error(int err_code)
  {
    error_code = err_code;
  }


} // end namespace gff
