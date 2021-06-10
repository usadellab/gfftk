/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <map>
#include "reader.h"
#include "ivalnode.h"

struct Locus
{
  std::string id;
  std::vector<IntervalNode*> nodes;
  void show();
};

struct Feature
{
  IntervalNode* center;
  IntervalNode* prev;
  IntervalNode* next;
  IntervalNode* up;
  IntervalNode* down;
};

class IsoformScanner : public gtf::Parser::Processor
{
  public:
    IsoformScanner();
    ~IsoformScanner();
    void process_entry(struct gtf::GtfEntry e);
    IntervalNode* insert(IntervalNode* root, IntervalNode* ival);
    IntervalNode* lastnode;
    void show_tree();
    void show_loci();

  private:
    IntervalNode* root = nullptr;
    void walk_inorder(IntervalNode* root);
    std::map<std::string, Locus> loci;
    bool isFeature(const IntervalNode* node, std::string feature);

};

