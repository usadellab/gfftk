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
    // lookup single by ID
    gff::GffEntry* find(const std::string& id);
    gff::GffEntry* parent_of_feat(const std::string& id);

    // get children of parent by feature type
    std::vector<GffEntry*> children_of_feat(const std::string& parent_id,
                                            const std::string& type = "");
    // get all children for feat id od type feature
    std::vector<GffEntry*> descendants_of_feat(const std::string& id,
                                               const std::string& type = "");
    std::vector<GffEntry*> ancestors_of_feat(const std::string& id);
    GffEntry* root_of_feat(const std::string& id);
    std::vector<GffEntry*> find_all(const std::string& type);

  private:
    // id > entry
    std::unordered_map<std::string, GffEntry*> by_id;

    // child id > parent id
    std::unordered_map<std::string, std::string> parent_of;

    // parent id > feature > all children
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<GffEntry*>>>
      children_of;
};
} // namespace gff
