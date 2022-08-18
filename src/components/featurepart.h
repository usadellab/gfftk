/*
 * -----------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

namespace gff
{
  class GffFeaturePart
  {

    public:
      GffFeaturePart(std::string id,
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
                     int err_code);
      GffFeaturePart();
      ~GffFeaturePart();
      bool has_parent();
      bool has_error();
      void set_error(int error_code);

    private:
      std::string id;
      std::vector<std::string> parents {};
      std::string seqid {};
      std::string source {};
      std::string type {};
      std::int_fast32_t start {};
      std::int_fast32_t end {};
      float score {};
      int strand {};
      int phase {};
      std::unordered_map<std::string, std::vector<std::string>> attributes {};
      int error_code = 0;
  };
} // namespace gff
