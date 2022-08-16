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
  typedef struct
  {
    std::string seqid;
    std::string source;
    std::string type;
    std::int_fast32_t start;
    std::int_fast32_t end;
    float score;
    int strand;
    int phase;
    std::vector<std::string> children { };
    std::vector<std::string> parents { };
    std::unordered_map<std::string, std::vector<std::string>> attributes;
  } GffFeaturePart;
} // namespace gff
