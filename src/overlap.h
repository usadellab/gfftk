/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include "ivalnode.h"

class Overlap
{
  public:
    Overlap();
    ~Overlap();
    bool isEmpty();

  private:
    std::vector<IntervalNode*> nodes;
};
