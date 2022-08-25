/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include "overlap.h"

Overlap::Overlap()
  { }
Overlap::~Overlap()
  { }

bool Overlap::isEmpty()
{
  return nodes.empty();
}
