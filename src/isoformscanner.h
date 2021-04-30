/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include "reader.h"

struct IntervalNode
{
  gtf::GtfEntry locus;
  std::int_fast32_t max;
  IntervalNode* left;
  IntervalNode* right;
};


class IsoformScanner : public gtf::Parser::Processor
{
  public:
    IsoformScanner();
    ~IsoformScanner();
    void process_entry(struct gtf::GtfEntry& e);

    IntervalNode* new_node(gtf::GtfEntry& e);
    IntervalNode* insert(IntervalNode* root, gtf::GtfEntry& e);
};

