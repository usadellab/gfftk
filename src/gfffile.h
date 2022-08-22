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
#include "components/gffrow.h"
#include "helpers/linetools.h"
#include "gffentry.h"


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
    unsigned int row_num = 0;
    int open();
    void close();
    void parse_directive(const std::string& line);
    std::unordered_map<std::string, std::vector<std::string>> directives;
    void show_attribute(const std::string& key, const std::unordered_map<std::string, std::vector<std::string>>) const;
    void assemble_locus(const gff::GffRow& row);
    gff::Locus* add_locus(const gff::GffRow& row);
    gff::Locus* locus(const std::string& id);
    void extend_locus(gff::Locus* locus, const gff::GffRow& row);
    void delete_loci();
    void clean_up();
    //                    type                          ID          feature
    // std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
    std::unordered_map<std::string, gff::Locus*> loci;
  };
}//end gff namespace
