/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include <sstream>
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
      split_cols(line, cols);
      gtf::GtfEntry e {cols[0], cols[1], cols[2], std::stol(cols[3]), std::stol(cols[4]), cols[8]};
      proc.process_entry(e);
      cols.clear();
    }
  }

  void Parser::split_cols(std::string& line, std::vector<std::string>& cols)
  {
    std::string value;
    std::istringstream valuestream(line);
    while(std::getline(valuestream, value, '\t'))
    {
      cols.push_back(value);
    }
  }
}//end namespace gtf
