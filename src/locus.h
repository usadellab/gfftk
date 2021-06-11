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
      gff::GffEntry locus;
      void show_features();
      void show();

    private:
      std::unordered_map<std::string, gff::GffEntry> features;

  };
} // namespace gff
