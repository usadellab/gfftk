/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#pragma once
#include "gff/gffentry.h"
#include "summaries/summaries.h"
#include "utils/stringtools.h"

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

    int total_length(const GffSelectedEntry& le);
    int total_length(const GffEntry* e, const std::string& sum_by = "");
    int total_length(const std::string& id, const std::string& sum_by = "");

    // get all children for feat id od type feature
    // Methods for descendants / children
    std::vector<GffEntry*> descendants_of_type(const std::string& id,
                                               const std::string& target = "");
    std::vector<GffEntry*> children_of_feat(const std::string& parent_id,
                                            const std::string& type = "");
    // Methods for ancestors
    std::vector<GffEntry*> ancestors_of_feat(const std::string& id);

    // Methods for parents/ roots
    gff::GffEntry* parent_of_feat(const std::string& id);
    std::vector<GffEntry*> parents_of_feat(const std::string& id);
    std::vector<GffEntry*> find_all_parents();
    GffEntry* root_of_feat(const std::string& id);
    std::vector<GffEntry*> roots();

    // lookup methods
    gff::GffEntry* find(const std::string& id);
    std::vector<GffEntry*> find_all(const std::string& type);
    std::vector<TypeLengths> type_lengths(const std::string& sum_by = "");
    GffEntry* longest_of(const std::vector<GffEntry*>& entries,
                         const std::string& sum_by = "");
    GffEntry* longest_of(const TypeLengths tl, const std::string& child_feature,
                         const std::string& sum_by = "");
    std::vector<GffSelectedEntry>& longest_entries();
    std::vector<gff::GffSelectedEntry>& longest_per_root(
      const std::string& type);
    std::vector<gff::GffSelectedEntry>& shortest_per_root(
      const std::string& type);
    gff::GffSummary summarize() const;
    void print_descendants(const std::string& id,
                           std::ostream& out = std::cout);

  private:
    enum class LengthSelectionMode
    {
      Longest,
      Shortest
    };
    void select_per_root(const std::string& feature, LengthSelectionMode mode);
    int length_by(const GffEntry* e, const std::string& child_feature);
    std::unordered_map<std::string, std::unordered_map<std::string, GffEntry*>>
      longest;
    // id > entry
    std::unordered_map<std::string, GffEntry*> by_id;
    // child id > parent id
    // std::unordered_map<std::string, std::string> parent_of;
    std::unordered_map<std::string, std::vector<std::string>> parent_of;
    // parent id > feature > all children
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<GffEntry*>>>
      children_of;
    std::vector<GffSelectedEntry> selected_entries;
};
void sort_entries(std::vector<GffEntry>& entries);
} // namespace gff
