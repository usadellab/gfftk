/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <unordered_map>
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
      Parser();
      ~Parser();
      void parse(gff::Parser::Processor& proc);

    private:
      void parse_header(const std::string& line);
      std::unordered_map<std::string, std::vector<std::string>> header;
  };
}//end gff namespace
