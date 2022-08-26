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


namespace gff
{

class TypeFeature : public gff::Feature
{
  public:
    TypeFeature(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, std::int_fast32_t start, std::int_fast32_t end);
    ~TypeFeature();
    // void extend_with_row(const gff::GffRow& row);
    // void add_feature(const gff::GffRow& row);
    // void extend_feature(const gff::GffRow& row);
    // void show();
    gff::Feature* add_feature(const gff::GffRow& row);
    // void add_entry(gff::GffEntry e);
    // Locus::Feature* find_longest_feature(const std::string& level);
    // const std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>>& featuremap();

    private:

};

} // namespace gff
