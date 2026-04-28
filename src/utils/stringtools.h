/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <algorithm>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

namespace stringtools
{
std::vector<std::string> tokenize(std::string& line, char delim);
std::string& lstrip(std::string& s);
std::string& rstrip(std::string& s);
std::string& strip(std::string& s);
std::string& ltrim(std::string& s);
std::string& rtrim(std::string& s);
std::string& trim(std::string& s);
std::string& lowercase(std::string& s);
std::pair<std::string, std::string> split_kv(const std::string& s, char delim);
} // namespace stringtools
