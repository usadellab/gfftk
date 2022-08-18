/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>

#include "helpers/linetools.h"
#include "components/featurepart.h"

namespace gff
{
  class GffRow
  {
    public:
      GffRow(const std::string& gff_file, int row_number);
      ~GffRow();
      const gff::GffFeaturePart parse(std::string& gffrow);

    private:
      using attributemap = std::unordered_map<std::string, std::vector<std::string>>;
      using columns = std::vector<std::string>;
      const std::string& gff_file;
      const long unsigned int expected_columns = 9;
      int row_number;
      int convert_strand(const std::string&) const;
      void parse_attributes(std::string& attribute_line);
      attributemap attributes;
      const std::string id_key = "ID";
      const std::string parent_key= "Parent";
  };
} // end namespace gff
