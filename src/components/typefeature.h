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
#include <set>

#include "components/gffrow.h"
#include "components/feature.h"

namespace gff
{

class TypeFeature : public gff::Feature
{
  public:
    TypeFeature(const std::string& seqid, const std::string& id,
                const std::string& source, const std::string& type,
                position start, position end, attributemap attribs);
    ~TypeFeature();
    void get_types(const std::string& type, std::set<gff::Feature*>& container);

  private:
    void bfs(gff::Feature* feat, const std::string& type, std::set<gff::Feature*>& container, int level = 0);
};

} // namespace gff
