/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#include "gff/gffindex.h"

#include "gff/gffentry.h"
#include "summaries/summaries.h"
#include "utils/stringtools.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

namespace gff
{

void GffIndex::build(std::vector<gff::GffEntry>& entries)
{
  // sort by sequence name and start coord
  sort_entries(entries);
  // index all entries by ID
  for(auto& e : entries)
  {
    by_id[e.id] = &e;
  }
  // build bidirectional links
  for(auto& e : entries)
  {
    if(!e.parent) { continue; }
    auto parents = stringtools::tokenize(*e.parent, ',');
    parent_of[e.id] = parents;
    for(const auto& pid : parents)
    {
      children_of[pid][e.type].push_back(&e);
    }
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
  std::vector<GffEntry*> parents = parents_of_feat(id);
  return parents.empty() ? nullptr : parents.front();
}

std::vector<GffEntry*> GffIndex::parents_of_feat(const std::string& id)
{
  std::vector<GffEntry*> result;
  auto it = parent_of.find(id);
  if(it == parent_of.end()) return {};
  for(const auto& pid : it->second)
    if(auto* e = find(pid)) result.push_back(e);
  return result;
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
  std::unordered_set<std::string> visited;
  std::queue<std::string> queue;
  queue.push(id);

  while(!queue.empty())
  {
    std::string current = queue.front(); // Needs to be called before pop()
    queue.pop();
    if(visited.count(current)) continue;
    visited.insert(current);

    auto it = parent_of.find(current);
    if(it == parent_of.end()) continue;

    for(const auto& pid : it->second) // iterate  parent ids
    {
      if(auto* e = find(pid))
      {
        path.push_back(e);
        queue.push(pid);
      }
    }
  }
  return path;
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

void GffIndex::select_per_root(const std::string& target_feature,
                               LengthSelectionMode mode)
{
  for(auto* root : roots())
  {
    auto targets = descendants_of_type(root->id, target_feature);
    if(targets.empty()) { continue; }

    std::unordered_map<std::string, std::vector<GffEntry*>> by_parent;
    for(auto* t : targets)
    {
      if(t->parent) by_parent[*t->parent].push_back(t);
    }

    // pick best group based on mode (shortest/longest)
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
    if(best_parent.empty()) { continue; }

    std::vector<gff::GffEntry*>& best_parts = by_parent[best_parent];
    std::sort(best_parts.begin(), best_parts.end(),
              [](const GffEntry* a, const GffEntry* b)
              { return a->beg < b->beg; });
    gff::GffEntry* parent_entry = find(best_parent);

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
  GffSummary s;
  s.total_entries = by_id.size();

  for(auto& [id, entry] : by_id)
  {
    int len = entry->length();

    auto& gt = s.global_by_type[entry->type];
    gt.type = entry->type;
    gt.count++;
    gt.total_length += len;
    gt.min_length = std::min(gt.min_length, len);
    gt.max_length = std::max(gt.max_length, len);

    auto& ss = s.by_sequence[entry->seqname];
    ss.seqname = entry->seqname;
    ss.total_entries++;

    auto& st = ss.by_type[entry->type];
    st.type = entry->type;
    st.count++;
    st.total_length += len;
    st.min_length = std::min(st.min_length, len);
    st.max_length = std::max(st.max_length, len);

    // root = not present in parent_of
    if(parent_of.count(id) == 0)
    {
      s.total_roots++;
      ss.root_count++;
    }
  }
  // sorted types
  for(auto& [feat, _] : s.global_by_type)
  {
    s.types.push_back(feat);
  }
  std::sort(s.types.begin(), s.types.end());

  s.total_sequences = s.by_sequence.size();
  s.avg_roots_per_seq
    = s.total_sequences > 0 ? (float)s.total_roots / s.total_sequences : 0.0f;

  for(auto& [feat, fs] : s.global_by_type)
  {
    fs.avg_length = fs.count > 0 ? (float)fs.total_length / fs.count : 0.0f;
  }

  for(auto& [seq, ss] : s.by_sequence)
  {
    s.root_counts_per_seq.push_back(ss.root_count);
    for(auto& feat : s.types)
    {
      auto it = ss.by_type.find(feat);
      s.values_per_seq[feat].push_back(
        it != ss.by_type.end() ? (float)it->second.count : 0.0f);
      if(it != ss.by_type.end())
      {
        it->second.avg_length
          = it->second.count > 0
            ? (float)it->second.total_length / it->second.count
            : 0.0f;
      }
    }
  }
  return s;
}
void GffIndex::print_descendants(const std::string& id, std::ostream& out)
{
  auto* root = find(id);
  if(!root)
  {
    out << "[ Error ] ID not found: " << id << "\n";
    return;
  }

  out << root->type << "\t" << root->id << "\t" << root->seqname << "\t"
      << root->beg << "\t" << root->end << "\n";

  // BFS with depth tracking for indentation
  std::queue<std::pair<std::string, int>> queue;
  queue.push({id, 1});

  while(!queue.empty())
  {
    auto [current, depth] = queue.front();
    queue.pop();

    auto pit = children_of.find(current);
    if(pit == children_of.end()) continue;

    for(auto& [feature, children] : pit->second)
    {
      for(auto* child : children)
      {
        out << std::string(depth * 2, '\t') // indent by depth
            << child->type << "\t" << child->id << "\t" << child->seqname
            << "\t" << child->beg << "\t" << child->end << "\t"
            << child->length() << "\n";
        queue.push({child->id, depth + 1});
      }
    }
  }
}

} // namespace gff
