/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>

#include "locus/featurepart.h"
#include "helpers/linetools.h"

namespace gff
{
  class DataRow
  {
    public:
      DataRow();
      ~DataRow();
      gff::GffFeaturePart collect_featurepart(const std::vector<std::string>&);

    private:
      typedef std::unordered_map<std::string, std::vector<std::string>> gffattributes;
      int strand_to_int(const std::string&) const;
      int phase_to_int(const std::string&) const;
      const gffattributes parse_attributes(std::string attribute_line);
  };
} // end namespace gff
