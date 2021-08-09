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
        Feature(const std::string& id);
        ~Feature();
        std::string id;
        std::string type;
        std::string parent_id;
        std::int_fast32_t start;
        std::int_fast32_t end;
        std::int_fast32_t length();
        void add_entry(gff::GffEntry e);
        void show_entries();
        void update_coords(gff::GffEntry e);
        void show();
        const std::vector<std::string> get_comment(const std::string& key) const;
        const std::string& sequence() const;
        bool hasComment(const std::string& commentkey);

      private:
        std::vector<gff::GffEntry> entries;
        void update_length(std::int_fast32_t entry_len);
        std::int_fast32_t feat_length = 0;
    };

    Locus(gff::GffEntry e);
    ~Locus();
    const std::string& id();
    const std::int_fast32_t start();
    const std::int_fast32_t end();
    void add_feature(gff::GffEntry e);
    void add_entry(gff::GffEntry e);
    void show();
    void show_features();
    Locus::Feature* find_longest_feature(const std::string& level);
    bool hasFeature(const std::string& level);

  private:
    //                    type                          ID          feature
    std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
    GffEntry loc_feature;
};

} // namespace gff
