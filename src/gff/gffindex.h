/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#pragma once
#include "gff/gffentry.h"

#include <algorithm>
#include <iostream>
#include <vector>

namespace gff
{
class GffIndex
{
  public:
    void build(std::vector<gff::GffEntry>& entries);
    void compute_longest(
      const std::unordered_map<std::string, std::string>& sum_by);
    int summed_length(const std::string& id, const std::string& child_feature);
    int spliced_length(const std::string& id);
    int coding_length(const std::string& id);

    int total_length(const GffLongestEntry& le);
    int total_length(const GffEntry* e, const std::string& sum_by = "");
    int total_length(const std::string& id, const std::string& sum_by = "");

    // lookup single by ID
    gff::GffEntry* find(const std::string& id);
    std::vector<GffEntry*> find_all(const std::string& type);
    gff::GffEntry* parent_of_feat(const std::string& id);
    std::vector<GffEntry*> find_all_parents();

    // get children of parent by feature type
    std::vector<GffEntry*> children_of_feat(const std::string& parent_id,
                                            const std::string& type = "");
    // get all children for feat id od type feature
    std::vector<GffEntry*> descendants_of_type(const std::string& id,
                                               const std::string& target = "");
    std::vector<GffEntry*> ancestors_of_feat(const std::string& id);
    GffEntry* root_of_feat(const std::string& id);

    std::vector<GffLongestEntry> longest_per_root(const std::string& type);
    std::vector<GffLongestEntry> longest_per_root(const std::string& type,
                                                  const std::string& sum_by);
    std::vector<GffEntry*> roots();
    std::vector<TypeLengths> type_lengths(const std::string& sum_by = "");
    GffEntry* longest_of(const std::vector<GffEntry*>& entries,
                         const std::string& sum_by = "");
    GffEntry* longest_of(const TypeLengths tl, const std::string& child_feature,
                         const std::string& sum_by = "");

  private:
    int length_by(const GffEntry* e, const std::string& child_feature);
    std::unordered_map<std::string, std::unordered_map<std::string, GffEntry*>>
      longest;
    // id > entry
    std::unordered_map<std::string, GffEntry*> by_id;
    // child id > parent id
    std::unordered_map<std::string, std::string> parent_of;
    // parent id > feature > all children
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<GffEntry*>>>
      children_of;
};
void sort_entries(std::vector<GffEntry>& entries);
} // namespace gff
