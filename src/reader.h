/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <unordered_map>
#include <fstream>
#include <vector>

#include "gffentry.h"

namespace gff
{
  class Parser
  {
    public:
      class Processor
      {
        public:
          virtual void process_entry(gff::GffEntry e, std::unordered_map<std::string, std::vector<std::string>>& header) = 0;
      };
      Parser(std::string gff_file);
      ~Parser();
      void parse(gff::Parser::Processor& proc);

    private:
      std::string gffsource;
      unsigned int line_num = 0;
      void parse_header(const std::string& line);
      std::unordered_map<std::string, std::vector<std::string>> header;
  };
}//end gff namespace
