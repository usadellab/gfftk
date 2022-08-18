/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "gfffile.h"
#include "gffentry.h"
#include "components/locus.h"

class IsoformScanner : public gff::GffFile::Processor
{
  public:
    IsoformScanner(std::string gffsource, int taxid = 0);
    ~IsoformScanner();
    int process_entry(gff::GffEntry e,
                       std::unordered_map<std::string,
                       std::vector<std::string>>& header);
    // gff::GffEntry get_feature(const std::string& feature_name);
    // bool hasFeature(const std::string& feature_name);
    // const std::string get_locus_id(gff::GffEntry e);
    // void show_tree();
    // void show_loci();
    // void show_feature(gff::Locus::Feature* f,
    //                   gff::Locus loc,
    //                   std::unordered_map<std::string, std::vector<std::string>>& header);
    // void list_locus_features(gff::Locus& loc, std::unordered_map<std::string, std::vector<std::string>>& header) const;

  private:
    std::string gffsource;
    int taxid;
    std::vector<std::string> directives = {"genome-build", "processor",
                                           "genome-build-accession",
                                           "genome-version"};
    std::vector<std::string> req_comments = {"protein_id", "locus_tag"};
    std::unordered_map<std::string, gff::Locus> loci;
    std::unordered_map<std::string, gff::GffEntry> features;
    std::string prevloc;
    void assemble_locus(gff::GffEntry e, std::unordered_map<std::string, std::vector<std::string>>& header);
};

void usage();
