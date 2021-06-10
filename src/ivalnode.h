/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <vector>
#include "gtfentry.h"

class IntervalNode
{
  public:
    IntervalNode(gtf::GtfEntry e);
    ~IntervalNode();
    std::int_fast32_t start;
    std::int_fast32_t end;
    std::int_fast32_t max;
    IntervalNode* left =  nullptr;
    IntervalNode* right = nullptr;
    std::vector<gtf::GtfEntry> entries;
    void show_entries();

  private:

};
