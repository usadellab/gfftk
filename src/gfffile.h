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
#include "locus/featurepart.h"
#include "helpers/linetools.h"

namespace gff
{
  class GffFile
  {
    public:
      class Processor
      {
        public:
          virtual int process_entry(gff::GffEntry e, std::unordered_map<std::string, std::vector<std::string>>& header) = 0;
      };
    GffFile(std::string gff_file);
    ~GffFile();
    int parse(gff::GffFile::Processor& proc);

  private:
    std::string path;
    std::ifstream gff_in;
    unsigned int line_num = 0;
    int open();
    void close();
    void parse_directive(const std::string& line);
    std::unordered_map<std::string, std::vector<std::string>> directives;
    const std::unordered_map<std::string, std::vector<std::string>> parse_attributes(std::string attributes);
    gff::GffFeaturePart row_to_featurepart(const std::vector<std::string>&);
    int strand_to_int(const std::string&) const;
    int phase_to_int(const std::string&) const;
    //                    type                          ID          feature
    // std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
  };
}//end gff namespace
