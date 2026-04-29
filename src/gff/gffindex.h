/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#pragma once
#include "gff/gffentry.h"

#include <iostream>
#include <vector>

namespace gff
{
class GffIndex
{
  public:
    void build(std::vector<gff::GffEntry>& entries);
    // lookup singel by ID
    gff::GffEntry* find(const std::string& id);

    // get children of parent by feature type
    std::vector<GffEntry*>* children_of(const std::string& parent_id,
                                        const std::string& feature);

  private:
    std::unordered_map<std::string, std::unordered_map<std::string,
                                                       std::vector<GffEntry*>>>
      children; // parent_id > feature > [children]
    std::unordered_map<std::string, GffEntry*> by_id; // id > entry
};
} // namespace gff
