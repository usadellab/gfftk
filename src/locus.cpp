/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include <iostream>

#include "locus.h"
#include "gffentry.h"

namespace gff
{
  Locus::Locus(gff::GffEntry& e)
  : start(e.start), end(e.end)
    { }

  Locus::~Locus()
    { }

} // namespace gff

