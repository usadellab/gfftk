/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include "reader.h"

#include <iostream>
#include <unordered_map>
#include <vector>

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
      if(line.empty())
      {
        continue;
      }
      if(line[0] == '#')
      {
        parse_header(line);
        continue;
      }
      // std::cout << cols[0] << "\n";
      gff::GffEntry e(utils::tokenize(line, '\t'));
      proc.process_entry(e, header);
    }
  }

  void Parser::parse_header(const std::string& line)
  {
    int status = 0;
    int directive_beg;
    for(long unsigned int i = 0; i < line.size(); ++i)
    {
      // std::cout << i << " : " << line[i] << " " << status << "\n";
      switch(status)
      {
        case(0):
          if(line[i] != '#' )
          {
            status = 1;
            break;
          }
          ++i;

        case(1):
          if(line[i] != '#' || line[i] != '!')
          {
            ++i;
            directive_beg = i;
            status = 2;
            break;
          }
          ++i;

        case(2):
          if(line[i] == ' ')
          {
            std::string directive = line.substr(directive_beg, i-1);
            std::string val = line.substr(i+1);
            const auto &[it, pass] = header.try_emplace(utils::strip(directive), std::vector<std::string>{utils::strip(val)});
            if(!pass)
              {it->second.push_back(line.substr(i+1));}
            return;
          }
          continue;
        default:
          return;
      }
    }
  }
}//end namespace gff
