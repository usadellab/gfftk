/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <vector>
#include "gffentry.h"

class IntervalNode
{
  public:
    IntervalNode(gff::GffEntry e);
    ~IntervalNode();
    std::int_fast32_t start;
    std::int_fast32_t end;
    std::int_fast32_t max;
    IntervalNode* left =  nullptr;
    IntervalNode* right = nullptr;
    std::vector<gff::GffEntry> entries;
    void show_entries();

  private:

};
