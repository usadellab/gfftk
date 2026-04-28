/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include "utils/stringtools.h"

#include <algorithm>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

namespace stringtools
{
std::vector<std::string> tokenize(std::string& line, char delim)
{
  std::vector<std::string> values;
  std::string value;
  std::istringstream valuestream(line);
  while(std::getline(valuestream, value, delim))
  {
    values.push_back(value);
  }
  return values;
}

// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
// https://en.cppreference.com/w/cpp/string/byte/isspace
std::string& lstrip(std::string& s)
{
  auto it = std::find_if(s.begin(), s.end(),
                         [](unsigned char c) { return !std::isspace(c); });
  s.erase(s.begin(), it);
  return s;
}

std::string& rstrip(std::string& s)
{
  auto it = std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char c) { return !std::isspace(c); });
  s.erase(it.base(), s.end());
  return s;
}

std::string& strip(std::string& s) { return lstrip(rstrip(s)); }

std::string& ltrim(std::string& s)
{
  auto it = std::find_if(s.begin(), s.end(),
                         [](unsigned char c) { return c != '\"'; });
  s.erase(s.begin(), it);
  return s;
}

std::string& rtrim(std::string& s)
{
  auto it = std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char c) { return c != '\"'; });
  s.erase(it.base(), s.end());
  return s;
}

std::string& trim(std::string& s) { return ltrim(rtrim(s)); }

std::string& lowercase(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return s;
}

std::pair<std::string, std::string> split_kv(const std::string& s,
                                             char delim = '=')
{
  auto pos = s.find(delim);
  if(pos == std::string::npos) return {s, ""}; // no delimiter
  return {s.substr(0, pos), s.substr(pos + 1)};
}

} // end namespace stringtools
