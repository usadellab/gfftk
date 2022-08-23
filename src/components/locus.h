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
#include "components/feature.h"
#include "gfftypes.h"


namespace gff
{

class Locus : public gff::Feature
{
  public:
    Locus(const std::string& id, const std::string& type, std::int_fast32_t start, std::int_fast32_t end);
    ~Locus();
    void extend_with_row(const gff::GffRow& row);
    void add_feature(const gff::GffRow& row);
    void extend_feature(const gff::GffRow& row);
    void show();
    // void add_feature(gff::GffEntry e);
    // void add_entry(gff::GffEntry e);
    // Locus::Feature* find_longest_feature(const std::string& level);
    // const std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>>& featuremap();

    private:
    //                    type                          ID          feature
    // GffEntry loc_feature;
};

} // namespace gff
