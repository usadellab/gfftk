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
  class GffFile
  {
    public:
      class Processor
      {
        public:
          virtual int process_entry(gff::GffEntry e, std::unordered_map<std::string, std::vector<std::string>>& header, gff::GffFile& file) = 0;
      };
    GffFile(std::string gff_file);
    ~GffFile();
    int parse(gff::GffFile::Processor& proc);

  private:
    const std::string path;
    std::ifstream gff_in;
    unsigned int line_num = 0;
    int open();
    void close();
    void parse_directive(const std::string& line);
    std::unordered_map<std::string, std::vector<std::string>> directives;
    void parse_attributes(const std::string& attributes);
    //                    type                          ID          feature
    std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
  };
}//end gff namespace
