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
    class Feature
    {
      public:
        Feature(gff::GffEntry& e);
        ~Feature();
        std::string id;
        std::string type;
        std::string parent_id;
        std::int_fast32_t start;
        std::int_fast32_t end;
        std::int_fast32_t length();
        void add_entry(gff::GffEntry e);
        void show_entries();
        void update(gff::GffEntry e);
        void show();

      private:
        std::vector<gff::GffEntry> entries;
        std::int_fast32_t feat_length;
        void update_length();
    };

    Locus(gff::GffEntry e);
    ~Locus();
    const std::int_fast32_t start();
    const std::int_fast32_t end();
    void add_feature(gff::GffEntry e);
    void show();
    const std::string& id();
    void show_features();
    void set_longest_feature(const std::string& level);
    void find_longest_feature(const std::string& level);
    bool hasFeature(const std::string& level);

  private:
    //                    type                          parent          feature
    std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
    GffEntry loc_feature;
};

} // namespace gff
