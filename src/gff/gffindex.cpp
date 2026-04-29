/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#include "gff/gffindex.h"

#include "gff/gffentry.h"

#include <iostream>
#include <vector>

namespace gff
{

void GffIndex::build(std::vector<gff::GffEntry>& entries)
{
  // index on ID
  for(auto& e : entries)
    if(!e.id.empty()) by_id[e.id] = &e;

  // group children by parent + feature
  for(auto& e : entries)
    if(e.parent) children[*e.parent][e.feature].push_back(&e);
}

// lookup singel by ID
gff::GffEntry* GffIndex::find(const std::string& id)
{
  auto it = by_id.find(id);
  return it != by_id.end() ? it->second : nullptr;
}

// get children of parent by feature type
std::vector<GffEntry*>* GffIndex::children_of(const std::string& parent_id,
                                              const std::string& feature)
{
  auto pit = children.find(parent_id);
  if(pit == children.end()) return nullptr;
  auto fit = pit->second.find(feature);
  if(fit == pit->second.end()) return nullptr;
  return &fit->second;
}
} // namespace gff
