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
    Locus(gff::GffEntry e);
    ~Locus();
    const std::int_fast32_t start();
    const std::int_fast32_t end();
    void add_feature(gff::GffEntry e);
    void show();
    const std::string& id();
    void show_features();

  private:
    std::vector<gff::GffEntry> features;
    GffEntry feature;

};

} // namespace gff
