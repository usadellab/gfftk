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

namespace gff
{
  class Locus
  {
    public:
      Locus();
      ~Locus();

    private:
      std::vector<const gff::Feature> features;

  };
} // namespace gff
