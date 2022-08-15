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
    std::string id;
    std::string type;
    std::vector<std::string> children;
    std::vector<std::string> parents;
    std::int_fast32_t start;
    std::int_fast32_t end;
    std::unordered_map<std::string, std::vector<std::string>> comments;
  } GffFeaturePart;
} // namespace gff
