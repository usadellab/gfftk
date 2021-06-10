/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once
#include <iostream>
#include <vector>

namespace utils
{
  std::vector<std::string> tokenize(const std::string& line, char delim);

  std::string& lstrip(std::string &s);
  std::string& rstrip(std::string &s);
  std::string& strip(std::string &s);

  std::string& ltrim(std::string &s);
  std::string& rtrim(std::string &s);
  std::string& trim(std::string &s);

} // namespace utils