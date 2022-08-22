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


#include "components/gffrow.h"
#include "gfftypes.h"


namespace gff
{
class Locus
{
  public:
    Locus(const std::string& id, const std::string& type, std::int_fast32_t start, std::int_fast32_t end);
    ~Locus();
    struct Coordinates
    {
      std::int_fast32_t start;
      std::int_fast32_t end;
    };
    std::string id;
    std::string type;
    void extend_with_row(const gff::GffRow& row);
    void add_feature(const gff::GffRow& row);
    void extend_feature(const gff::GffRow& row);
    void show();
    void show_features();
    bool hasFeature(const std::string& level);
    const std::vector<Coordinates>& coordinates() const;
    // void add_feature(gff::GffEntry e);
    // void add_entry(gff::GffEntry e);
    // Locus::Feature* find_longest_feature(const std::string& level);
    // const std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>>& featuremap();

  private:
    std::vector<Coordinates> positions;
    void add_positions(std::int_fast32_t start, std::int_fast32_t end);
    //                    type                          ID          feature
    // std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>> features;
    // GffEntry loc_feature;
};

} // namespace gff
