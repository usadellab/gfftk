/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include <iostream>

#include "ivalnode.h"
#include "gffentry.h"

IntervalNode::IntervalNode(gff::GffEntry e)
  :start(e.start), end(e.end), max(e.end)
{
  entries.push_back(e);
}

IntervalNode::~IntervalNode()
  { }

void IntervalNode::show_entries()
    {
      for(auto& i:entries)
      {
        std::cout << i.start << "\t" << i.gene_id << "\t"<< i.transcript_id << "\n";
      }
    }
