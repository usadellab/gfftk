/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

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
      std::unordered_map<std::string, const gff::GffEntry&> features;



  };
} // namespace gff
