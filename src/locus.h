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

struct FeatureA
{
  std::string type;
  std::string parent_id;
  std::int_fast32_t start;
  std::int_fast32_t end;
  std::vector<gff::GffEntry> entries;
};

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
    void set_longest_feature(const std::string& level);
    gff::FeatureA longest_feat = {"dummy", "none", 0, 0};

  private:
    std::unordered_map<std::string, std::unordered_map<std::string, gff::FeatureA>> features;
    // gff::FeatureA longest_feat;

    GffEntry feature;
};

} // namespace gff
