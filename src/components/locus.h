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
#include "components/typefeature.h"
#include "gfftypes.h"


namespace gff
{

class Locus : public gff::Feature
{
  public:
    Locus(const std::string& seqid, const std::string& id, const std::string&
          source, const std::string& type, position start, position end,
          attributemap& attribs);
    ~Locus();
    void extend_with_row(const gff::GffRow& row);
    void show();
    // Locus::Feature* find_longest_feature(const std::string& level);
    // const std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>>& featuremap();
};

} // namespace gff
