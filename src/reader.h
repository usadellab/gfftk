#pragma once
/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
//#include <cstdint>
#include <vector>

namespace gtf
{
  struct GtfEntry
  {
    std::string seqname;
    std::string source;
    std::string feature;
    std::int_fast32_t start;
    std::int_fast32_t end;
    //int strand;
    //float score;
    //int frame;
     std::string comment;
  };

  class Parser
  {
    public:
      class Processor
      {
        public:
          virtual void process_entry(struct gtf::GtfEntry& e) = 0;
      };
      Parser();
      ~Parser();
      void parse(gtf::Parser::Processor& proc);

    private:
      std::vector<std::string> cols;
      void split_cols(std::string& line, std::vector<std::string>& cols);
  };

}//end gtf namespace
