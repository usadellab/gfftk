/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <unordered_map>

#include "helpers/linetools.h"
#include "gfftypes.h"

namespace gff
{
  class GffRow
  {
    public:
      GffRow(std::string& gff_row, const std::string& gff_file, int row_number);
      ~GffRow();
      std::string id;
      std::string seqid;
      std::string source;
      std::string type;
      std::int_fast32_t start;
      std::int_fast32_t end;
      float score;
      int strand;
      int phase;
      std::vector<std::string> parents;
      int err_code;
      int rownum;
      void parse(std::string& gffrow);
      const attributemap& comment(const std::string& comment); // ToDo

    private:
      const std::string& gff_file;
      const long unsigned int expected_columns = 9;
      int convert_strand(const std::string&) const;
      void parse_attributes(std::string& attribute_line);
      attributemap attributes;
      const std::string id_key = "ID";
      const std::string parent_key= "Parent";
  };
} // end namespace gff
