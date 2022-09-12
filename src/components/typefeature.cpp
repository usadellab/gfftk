/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#include "components/typefeature.h"

namespace gff
{

  TypeFeature::TypeFeature(const std::string& seqid, const std::string& id,
                           const std::string& source, const std::string& type,
                           position start, position end)
  : Feature{seqid, id, source, type, start, end}
  {  }

  TypeFeature::~TypeFeature()
  {  }

  void TypeFeature::get_types(const std::string& type, std::vector<const gff::Feature*>& container)
  {
    bfs(this, type, container);
  }

  void TypeFeature::bfs(const gff::Feature* feat, const std::string& type, std::vector<const gff::Feature*>& container, int level)
  {
    if(feat->type == type)
    {
      // std::cout << std::string(level, '\t') << feat->id << "\t" << feat->type
                // << "\t" <<  feat->length() << "\t" << feat->size() <<"\n";
      container.push_back(feat);
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
