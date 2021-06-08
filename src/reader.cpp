/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include <sstream>
#include <locale>
#include <vector>
#include "reader.h"

namespace gtf
{
  Parser::Parser()
  { }

  Parser::~Parser()
  { }

  void Parser::parse(gtf::Parser::Processor& proc)
  {
    for(std::string line; std::getline(std::cin, line);)
    {
      if(line[0] == '#')
      {
        continue;
      }
      std::vector cols = tokenize(line, '\t');
      gtf::GtfEntry e {cols[0], cols[1], cols[2], std::stol(cols[3]), std::stol(cols[4])};
      process_comments(e, strip(cols[8]));
      cols.clear();
      proc.process_entry(e);
    }
  }

  void process_comments(gtf::GtfEntry& e, std::string& gtf_comments)
  {
    for(auto& i : tokenize(gtf_comments, ';'))
    {
      std::vector<std::string> comment = tokenize(strip(i), ' ');
      if(comment[0] == "gene_id")
        e.gene_id = trim(comment[1]);
      if(comment[1] == "transcript_id")
        e.transcript_id = trim(comment[1]);
    }
  }

  std::vector<std::string> tokenize(const std::string& line, char delim)
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

  //https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
  //https://en.cppreference.com/w/cpp/string/byte/isspace
  std::string& lstrip(std::string &s)
  {
    auto it = std::find_if(s.begin(), s.end(),
                          [](unsigned char c){return !std::isspace(c);
                                    });
    s.erase(s.begin(), it);
    return s;
  }

  std::string& rstrip(std::string &s)
  {
    auto it = std::find_if(s.rbegin(), s.rend(),
                           [](unsigned char c){ return !std::isspace(c);
                           });
    s.erase(it.base(), s.end());
    return s;
  }

  std::string& strip(std::string &s)
  {
    return lstrip(rstrip(s));
  }

  std::string& ltrim(std::string &s)
  {
    auto it = std::find_if(s.begin(), s.end(),
                          [](unsigned char c){return c != '\"';
                                    });
    s.erase(s.begin(), it);
    return s;
  }

  std::string& rtrim(std::string &s)
  {
    auto it = std::find_if(s.rbegin(), s.rend(),
                           [](unsigned char c){return c != '\"';
                           });
    s.erase(it.base(), s.end());
    return s;
  }

  std::string& trim(std::string &s)
  {
    return ltrim(rtrim(s));
  }

}//end namespace gtf
