/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include <vector>

#include "reader.h"
#include "utils.h"

namespace gff
{
  Parser::Parser()
  { }

  Parser::~Parser()
  { }

  void Parser::parse(gff::Parser::Processor& proc)
  {
    for(std::string line; std::getline(std::cin, line);)
    {
      if(line[0] == '#')
      {
        continue;
      }
      // std::cout << cols[0] << "\n";
      gff::GffEntry e(utils::tokenize(line, '\t'));
      proc.process_entry(e);
    }
  }
}//end namespace gff
