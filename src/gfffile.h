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
#include <iostream>
#include <filesystem>

#include "components/typefeature.h"
#include "components/gffrow.h"
#include "gfftypes.h"
#include "helpers/stringtools.h"

namespace gff
{
  class GffFile
  {
    public:
      class Processor
      {
        public:
          virtual int process_locus(gff::TypeFeature* locus) = 0;
      };
    GffFile(std::string gff_file);
    ~GffFile();
    void close();
    int parse(gff::GffFile::Processor& proc);
    gff::TypeFeature* add_feature(const gff::GffRow& row);
    gff::TypeFeature* get_feature(const std::string& id);

  private:
    gff::TypeFeature* prev_loc = nullptr;
    unsigned long int loc_count = 0;
    std::string path;
    std::ifstream gff_in;
    unsigned int row_num = 0;
    int open();
    void parse_directive(const std::string& line);
    std::unordered_map<std::string, std::vector<std::string>> directives;
    gff::TypeFeature* assemble_locus(gff::TypeFeature* feature);
    void empty_features();
    void clean_up();
    using featuremap = std::unordered_map<std::string, gff::TypeFeature*>;
    featuremap features;
  };
}//end gff namespace
