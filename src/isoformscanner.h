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

#include "reader.h"
#include "gffentry.h"
#include "ivalnode.h"
#include "locus.h"

class IsoformScanner : public gff::Parser::Processor
{
  public:
    IsoformScanner();
    ~IsoformScanner();
    void process_entry(gff::GffEntry e);
    IntervalNode* insert(IntervalNode* root, IntervalNode* ival);
    gff::GffEntry get_feature(const std::string& feature_name);
    const std::string get_locus_id(gff::GffEntry e);
    void show_tree();
    void show_loci();

  private:
    IntervalNode* root = nullptr;
    void walk_inorder(IntervalNode* root);
    std::unordered_map<std::string, gff::Locus> loci;
    std::unordered_map<std::string, gff::GffEntry> features;
    bool isFeature(const IntervalNode* node, std::string feature);
    void assemble_locus(gff::GffEntry e);
    std::vector<IntervalNode*> nodes;
    std::string prevloc;

};

