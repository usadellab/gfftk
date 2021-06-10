/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <vector>

#include "feature.h"
#include "gffentry.h"

namespace gff
{
  class Locus
  {
    public:
      Locus(gff::GffEntry& e);
      ~Locus();
      std::int_fast32_t start;
      std::int_fast32_t end;

    private:
      std::vector<const gff::Feature> features;

  };
} // namespace gff
