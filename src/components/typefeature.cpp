/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#include "components/typefeature.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

#include "components/gffrow.h"
#include "components/feature.h"

namespace gff
{

  TypeFeature::TypeFeature(const std::string& seqid, const std::string& id,
      const std::string& source, const std::string& type, const float score,
      int strand, int phase, position start, position end, attributemap attribs)
  : Feature{seqid, id, source, type, score, strand, phase, start, end, attribs}
  {
    move_attributes_from_row(attribs);
  }

  TypeFeature::~TypeFeature()
  {  }

  void TypeFeature::get_types(const std::string& type,
                              std::set<gff::Feature*>& container)
  {
    bfs(this, type, container);
  }

  void TypeFeature::bfs(gff::Feature* feat, const std::string& type,
                        std::set<gff::Feature*>& container, int level)
  {
    if(feat->type == type)
    {
      // std::cout << std::string(level, '\t') << feat->id << "\t" << feat->type
                // << "\t" <<  feat->length() << "\t" << feat->size() <<"\n";
      container.insert(feat);
    }
    if(feat->get_children().size())
    {
      ++level;
      for(const auto& i : feat->get_children())
      {
        for(const auto& j : i.second)
        {
          bfs(j.second, type, container, level);
        }
      }
      --level;
    }
  }
}
