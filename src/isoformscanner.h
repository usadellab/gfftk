/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <unordered_map>

#include "reader.h"
#include "gffentry.h"
#include "ivalnode.h"
#include "locus.h"

// struct Locus
// {
//   std::string id;
//   std::vector<IntervalNode*> nodes;
//   void show();
// };



class IsoformScanner : public gff::Parser::Processor
{
  public:
    IsoformScanner();
    ~IsoformScanner();
    void process_entry(gff::GffEntry e);
    IntervalNode* insert(IntervalNode* root, IntervalNode* ival);
    void show_tree();
    void show_loci();

  private:
    IntervalNode* root = nullptr;
    void walk_inorder(IntervalNode* root);
    std::unordered_map<std::string, gff::Locus&> loci;
    bool isFeature(const IntervalNode* node, std::string feature);
    void assemble_loci(gff::GffEntry& e);
    std::vector<IntervalNode*> nodes;
};

