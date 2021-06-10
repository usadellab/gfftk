/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#pragma once

#include <iostream>
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
          virtual void process_entry(struct gff::GffEntry e) = 0;
      };
      Parser();
      ~Parser();
      void parse(gff::Parser::Processor& proc);

    private:
  };

  std::vector<std::string> tokenize(const std::string& line, const char delim);
  std::string& lstrip(std::string &s);
  std::string& rstrip(std::string &s);
  std::string& strip(std::string &s);

  std::string& ltrim(std::string &s);
  std::string& rtrim(std::string &s);
  std::string& trim(std::string &s);
  void process_comments(gff::GffEntry& e, std::string& gff_comments);
}//end gff namespace
