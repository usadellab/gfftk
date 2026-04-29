/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#include "gff/gffindex.h"

#include "gff/gffentry.h"

#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace gff
{

void GffIndex::build(std::vector<gff::GffEntry>& entries)
{
  // index all entries by ID
  for(auto& e : entries)
    by_id[e.id] = &e;

  // build bidirectional links
  for(auto& e : entries)
  {
    if(!e.parent) continue;

    // child -> parent
    parent_of[e.id] = *e.parent;

    // parent -> children (grouped by feature)
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

std::vector<GffEntry*> GffIndex::descendants_of_feat(const std::string& id,
                                                     const std::string& type)
{
  std::vector<GffEntry*> result;
  std::queue<std::string> queue;
  queue.push(id);

  while(!queue.empty())
  {
    auto current = queue.front();
    queue.pop();
    for(auto* child : children_of_feat(current))
    {
      if(type.empty() || child->type == type) result.push_back(child);
      queue.push(child->id); // recurse into grandchildren
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
    if(ptr->type == type) result.push_back(ptr);
  return result;
}

std::vector<GffEntry*> GffIndex::find_all_parents()
{
  std::vector<GffEntry*> result;
  for(auto& [id, ptr] : by_id)
    if(!ptr->parent.has_value()) result.push_back(ptr);
  return result;
}
} // namespace gff
