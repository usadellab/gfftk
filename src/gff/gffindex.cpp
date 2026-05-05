/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#include "gff/gffindex.h"

#include "gff/gffentry.h"
#include "summaries/summaries.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace gff
{

void GffIndex::build(std::vector<gff::GffEntry>& entries)
{
  // sort by sequence name and start coord
  sort_entries(entries);
  // index all entries by ID
  for(auto& e : entries)
    by_id[e.id] = &e;

  // build bidirectional links
  for(auto& e : entries)
  {
    if(!e.parent) continue;
    // child > parent
    parent_of[e.id] = *e.parent;
    // parent > children (grouped by feature)
    children_of[*e.parent][e.type].push_back(&e);
  }
}

// lookup singel by ID
GffEntry* GffIndex::find(const std::string& id)
{
  auto it = by_id.find(id);
  return it != by_id.end() ? it->second : nullptr;
}

// direct parent of a node
GffEntry* GffIndex::parent_of_feat(const std::string& id)
{
  auto it = parent_of.find(id);
  if(it == parent_of.end()) return nullptr;
  return find(it->second);
}

// get children of parent by feature type
// direct children, optionally filtered by feature type
std::vector<GffEntry*> GffIndex::children_of_feat(const std::string& id,
                                                  const std::string& type)
{
  auto pit = children_of.find(id);
  if(pit == children_of.end()) return {};

  if(!type.empty())
  {
    auto fit = pit->second.find(type);
    return fit != pit->second.end() ? fit->second : std::vector<GffEntry*>{};
  }

  // no filter > return all children flattened
  std::vector<GffEntry*> all;
  for(auto& [feat, vec] : pit->second)
    all.insert(all.end(), vec.begin(), vec.end());
  return all;
}

std::vector<GffEntry*> GffIndex::descendants_of_type(const std::string& id,
                                                     const std::string& target)
{
  std::vector<GffEntry*> result;
  std::queue<std::string> queue;
  queue.push(id);

  while(!queue.empty())
  {
    auto current = queue.front();
    queue.pop();

    auto pit = children_of.find(current);
    if(pit == children_of.end()) continue;

    for(auto& [feature, children] : pit->second)
    {
      for(auto* child : children)
      {
        if(child->type == target) result.push_back(child);
        queue.push(child->id); // always recurse regardless of feature
      }
    }
  }
  return result;
}

std::vector<GffEntry*> GffIndex::ancestors_of_feat(const std::string& id)
{
  std::vector<GffEntry*> path;
  std::string current = id;

  while(true)
  {
    auto* parent = parent_of_feat(current);
    if(!parent) break;
    path.push_back(parent);
    current = parent->id;
  }
  return path; // ordered nearest > root
}
// root of the tree a node belongs to
GffEntry* GffIndex::root_of_feat(const std::string& id)
{
  auto path = ancestors_of_feat(id);
  return path.empty() ? find(id) : path.back();
}

std::vector<GffEntry*> GffIndex::find_all(const std::string& type)
{
  std::vector<GffEntry*> result;
  for(auto& [id, ptr] : by_id)
  {
    if(ptr->type == type) { result.push_back(ptr); }
  }
  return result;
}

std::vector<GffEntry*> GffIndex::find_all_parents()
{
  std::vector<GffEntry*> result;
  for(auto& [id, ptr] : by_id)
  {
    if(!ptr->parent.has_value()) { result.push_back(ptr); }
  }
  return result;
}

int GffIndex::total_length(const GffSelectedEntry& le)
{
  int total = 0;
  for(const auto& c : le.coords)
  {
    total += c.length();
  }
  return total;
}

int GffIndex::total_length(const GffEntry* e, const std::string& sum_by)
{
  return sum_by.empty() ? e->length() : summed_length(e->id, sum_by);
}

int GffIndex::total_length(const std::string& id, const std::string& sum_by)
{
  return summed_length(id, sum_by);
}

int GffIndex::summed_length(const std::string& id,
                            const std::string& child_feature)
{
  auto children = children_of_feat(id, child_feature);
  if(children.empty())
  {
    if(auto* e = find(id)) return e->length();
  }
  else
  {
    return 0;
  }

  int total = 0;
  for(auto* c : children)
  {
    total += c->length();
  }
  return total;
}

int GffIndex::spliced_length(const std::string& id)
{
  return summed_length(id, "exon");
}

int GffIndex::coding_length(const std::string& id)
{
  return summed_length(id, "cds");
}

int GffIndex::length_by(const GffEntry* e, const std::string& sum_by)
{
  if(sum_by.empty()) { return e->length(); }
  if(sum_by == e->type) { return e->length(); }
  // find sum_by features at any depth
  auto parts = descendants_of_type(e->id, sum_by);
  if(parts.empty()) { return e->length(); }
  int total = 0;
  for(auto* p : parts)
  {
    total += p->length();
  }
  return total;
}

void GffIndex::compute_longest(
  const std::unordered_map<std::string, std::string>& sum_by)
{
  for(auto& [parent_id, feature_map] : children_of)
  {
    for(auto& [feature, children] : feature_map)
    {
      std::string child_feature;
      if(auto it = sum_by.find(feature); it != sum_by.end())
        child_feature = it->second;

      longest[parent_id][feature] = *std::max_element(
        children.begin(), children.end(),
        [&](const GffEntry* a, const GffEntry* b)
        { return length_by(a, child_feature) < length_by(b, child_feature); });
    }
  }
}

void GffIndex::select_per_root(const std::string& target_feature,
                               LengthSelectionMode mode)
{
  for(auto* root : roots())
  {
    auto targets = descendants_of_type(root->id, target_feature);
    if(targets.empty()) continue;

    std::unordered_map<std::string, std::vector<GffEntry*>> by_parent;
    for(auto* t : targets)
      if(t->parent) by_parent[*t->parent].push_back(t);

    // pick best group based on mode
    std::string best_parent;
    int best_len = (mode == LengthSelectionMode::Longest) ? -1 : INT_MAX;
    for(auto& [pid, parts] : by_parent)
    {
      int len = 0;
      for(auto* p : parts)
        len += p->length();

      bool better = (mode == LengthSelectionMode::Longest) ? len > best_len
                                                           : len < best_len;
      if(better)
      {
        best_len = len;
        best_parent = pid;
      }
    }

    auto& best_parts = by_parent[best_parent];
    std::sort(best_parts.begin(), best_parts.end(),
              [](const GffEntry* a, const GffEntry* b)
              { return a->beg < b->beg; });
    auto* parent_entry = find(best_parent);

    GffSelectedEntry entry;
    entry.seqname = best_parts.front()->seqname;
    entry.source = best_parts.front()->source;
    entry.type = target_feature;
    entry.strand = best_parts.front()->strand;
    entry.id = parent_entry ? parent_entry->id : best_parent;
    entry.parent = parent_entry ? parent_entry->parent : std::nullopt;

    for(auto* c : best_parts)
    {
      entry.coords.push_back({c->beg, c->end});
    }
    selected_entries.push_back(std::move(entry));
  }
}

std::vector<gff::GffSelectedEntry>& GffIndex::longest_per_root(
  const std::string& type)
{
  select_per_root(type, LengthSelectionMode::Longest);
  return selected_entries;
}

std::vector<gff::GffSelectedEntry>& GffIndex::shortest_per_root(
  const std::string& type)
{
  select_per_root(type, LengthSelectionMode::Shortest);
  return selected_entries;
}

std::vector<GffEntry*> GffIndex::roots()
{
  std::vector<GffEntry*> result;
  for(auto& [id, entry] : by_id)
  {
    if(parent_of.count(id) == 0) { result.push_back(entry); }
  }
  return result;
}

std::vector<TypeLengths> GffIndex::type_lengths(const std::string& sum_by)
{
  std::vector<TypeLengths> result;

  for(auto* root : roots())
  {
    TypeLengths tl;
    tl.seqname = root->seqname;
    tl.root_id = root->id;

    // BFS over descendants
    std::queue<std::string> queue;
    queue.push(root->id);

    while(!queue.empty())
    {
      auto current = queue.front();
      queue.pop();

      auto pit = children_of.find(current);
      if(pit == children_of.end()) { continue; }

      for(auto& [type, children] : pit->second)
      {
        for(auto* child : children)
        {
          // sum over grandchildren if using sum_by
          int len = sum_by.empty() ? child->length()
                                   : summed_length(child->id, sum_by);
          tl.total[type] += len;
          tl.count[type]++;
          queue.push(child->id); // do recurse
        }
      }
    }
    result.push_back(std::move(tl));
  }
  return result;
}

GffEntry* GffIndex::longest_of(const TypeLengths tl,
                               const std::string& child_feature,
                               const std::string& sum_by)
{
  auto children = descendants_of_type(tl.root_id, child_feature);
  return longest_of(children, sum_by);
}

GffEntry* GffIndex::longest_of(const std::vector<GffEntry*>& entries,
                               const std::string& sum_by)
{
  if(entries.empty()) { return nullptr; }

  return *std::max_element(
    entries.begin(), entries.end(), [&](const GffEntry* a, const GffEntry* b)
    { return length_by(a, sum_by) < length_by(b, sum_by); });
}

void sort_entries(std::vector<GffEntry>& entries)
{
  std::sort(entries.begin(), entries.end(),
            [](const GffEntry& a, const GffEntry& b)
            {
              if(a.seqname != b.seqname) { return a.seqname < b.seqname; }
              return a.beg < b.beg;
            });
}

std::vector<GffSelectedEntry>& GffIndex::longest_entries()
{
  return selected_entries;
}

GffSummary GffIndex::summarize() const
{
  gff::GffSummary summary;
  summary.total_entries = by_id.size();

  // per entry
  for(auto& [id, entry] : by_id)
  {
    int len = entry->length();

    // global feature summary
    auto& gf = summary.global_by_feature[entry->type];
    gf.type = entry->type;
    gf.count++;
    gf.total_length += len;
    gf.min_length = std::min(gf.min_length, len);
    gf.max_length = std::max(gf.max_length, len);

    // per sequence summary
    auto& ss = summary.by_sequence[entry->seqname];
    ss.seqname = entry->seqname;
    ss.total_entries++;

    auto& sf = ss.by_feature[entry->type];
    sf.type = entry->type;
    sf.count++;
    sf.total_length += len;
    sf.min_length = std::min(sf.min_length, len);
    sf.max_length = std::max(sf.max_length, len);

    // root entries (entries without a parent)
    if(parent_of.count(id) == 0)
    {
      summary.total_roots++;
      ss.root_count++;
    }
  }

  // averages
  summary.total_sequences = summary.by_sequence.size();
  summary.avg_roots_per_seq
    = summary.total_sequences > 0
      ? (float)summary.total_roots / summary.total_sequences
      : 0.0f;

  for(auto& [feat, fs] : summary.global_by_feature)
    fs.avg_length = fs.count > 0 ? (float)fs.total_length / fs.count : 0.0f;

  for(auto& [seq, ss] : summary.by_sequence)
    for(auto& [feat, fs] : ss.by_feature)
      fs.avg_length = fs.count > 0 ? (float)fs.total_length / fs.count : 0.0f;

  return summary;
}

} // namespace gff
