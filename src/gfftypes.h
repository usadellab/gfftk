/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

namespace gff
{
  using attributemap = std::unordered_map<std::string, std::vector<std::string>>;
  using columns = std::vector<std::string>;
  using Coordinates = struct GffCoordinates
                      {
                        std::int_fast32_t start;
                        std::int_fast32_t end;
                      };
} // end namespace gff
