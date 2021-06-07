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
    std::string gene_id;
    std::string transcript_id;
    //int strand;
    //float score;
    //int frame;
     //std::map<std::string, std::string> comments;
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
  };

  std::vector<std::string> tokenize(const std::string& line, const char delim);
  std::string& lstrip(std::string &s);
  std::string& rstrip(std::string &s);
  std::string& strip(std::string &s);

  std::string& ltrim(std::string &s);
  std::string& rtrim(std::string &s);
  std::string& trim(std::string &s);
  void process_comments(gtf::GtfEntry& e, std::string& gtf_comments);
}//end gtf namespace
