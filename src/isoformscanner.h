/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include <map>
#include "reader.h"

struct IntervalNode
{
  gtf::GtfEntry locus;
  std::int_fast32_t max;
  IntervalNode* left;
  IntervalNode* right;
};

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
    void process_entry(struct gtf::GtfEntry& e);

    IntervalNode* new_node(gtf::GtfEntry& e);
    IntervalNode* insert(IntervalNode* root, IntervalNode* ival);
    IntervalNode* lastnode;
    void show_tree();
    void show_loci();

  private:
    IntervalNode* root = 0;
    void walk_inorder(IntervalNode* root);
    std::map<std::string, Locus> loci;
    bool isFeature(IntervalNode* node, std::string feature);
    int node_count = 0;

};

