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

#include "components/locus.h"
#include "components/typefeature.h"
#include "components/gffrow.h"
#include "gfftypes.h"
#include "helpers/linetools.h"


namespace gff
{
  class GffFile
  {
    public:
      class Processor
      {
        public:
          virtual int process_entry(gff::Locus* locus) = 0;
      };
    GffFile(std::string gff_file);
    ~GffFile();
    int parse(gff::GffFile::Processor& proc);
    gff::TypeFeature* add_feature(const gff::GffRow& row);
    gff::TypeFeature* get_feature(const std::string& id);

  private:
    std::string path;
    std::ifstream gff_in;
    unsigned int row_num = 0;
    int open();
    void close();
    void parse_directive(const std::string& line);
    std::unordered_map<std::string, std::vector<std::string>> directives;
    void show_attribute(const std::string& key, const std::unordered_map<std::string, std::vector<std::string>>) const;
    void assemble_locus(const gff::GffRow& row);
    void empty_features();
    void clean_up();
    void walk_features(const gff::Feature* feat, int level);
    //                    type                          ID          feature
    // std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
    using featuremap = std::unordered_map<std::string, gff::TypeFeature*>;
    featuremap features;
  };
}//end gff namespace
