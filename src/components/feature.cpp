/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/feature.h"

namespace gff
{
  Feature::Feature(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, position start, position end)
    : seqid(seqid), id(id), source(source), type(type), positions{ {start, end}}
  {  }

  Feature::~Feature()
  {  }

  void Feature::add_positions(const position start, const position end)
  {
    positions.emplace(positions.end(), Coordinates{start, end});
  }

  const std::vector<Coordinates>& Feature::coordinates() const
  {
    return positions;
  }

  // void Feature::empty_featuremap()
  // {
  //   std::cout << "Feature : " << id << " : stored subfeatures: " << features.size() << "\n";
  //   for(auto it = features.begin(); it != features.end();)
  //   {
  //     for(auto itt = it->second.begin(); itt != it->second.end();)
  //     {
  //       std::cout << "\tDeleting : " << itt ->second->id << "\n";
  //       delete(itt->second);
  //       itt++;
  //     }
  //     it++;
  //   }
  //   features.clear();
  //   std::cout << "On: " << id << ": postdel feat size: " << features.size() << "\n";
  // }

  bool Feature::is_locus()
  {
    return parents.empty();
  }

  const std::unordered_map<std::string, gff::Feature*>& Feature::get_kids()
  {
    std::unordered_map<std::string, gff::Feature*> kids;
    for(auto& i : children)
    {
      for(auto& j : i.second)
      {
        kids[j.first] = j.second;
      }
    }
    return kids;
  }

  void Feature::add_parent(gff::Feature* parent)
  {
    const auto &[it, inserted] = parents.try_emplace(parent->id, parent);
    if(inserted)
    {
      std::cout << " inserted parent: " << parent->id;
    }
    else
    {
      std::cout << " parent known: " << parent->id;
    }
    parent->add_child(this);
  }

  void Feature::add_child(gff::Feature* child)
  {
    children.try_emplace(child->type, std::unordered_map<std::string, gff::Feature*> {});
    const auto &[it, inserted] = children[child->type].try_emplace(child->id, child);
    if(!inserted)
    {
      std::cout << "[Info] feature " << id << ": child known: " << it->second->id << "\n";
    }
  }

  gff::Feature* Feature::get_parent(const std::string& parent_id) const
  {
    if(parents.count(parent_id))
    {
      return parents.at(parent_id);
    }
    return nullptr;
  }

  const std::unordered_map<std::string, gff::Feature*>& Feature::get_parents()
  {
    return parents;
  }

  bool Feature::is_duplicate(const gff::Feature* rhs) const // ToDo: overload == operator
  {
    if(seqid != rhs->seqid)
    {return false;}
    if(positions.size() != rhs->coordinates().size())
      {return false;}
    if(type != rhs->type)
      {return false;}
    for(long unsigned int i = 0; i < positions.size(); ++i)
    {
      if(positions[i].start != rhs->coordinates()[i].start)
        {return false;}
      if(positions[i].end != rhs->coordinates()[i].end)
        {return false;}
    }
    return true;
  }

  bool Feature::extend_with(const gff::Feature* extender)
  {
    if(id != extender->id)
    {
      std::cout << "[Error] feature " << id << " cannot be extended with " << extender->id << "\n";
      return false;
    }
    for(auto i : extender->coordinates())
    {
      positions.push_back(std::move(i));
    }
    //merge attribute comments somehow
    return true;
  }


  const Feature::childrenmap& Feature::get_children() const
  {
    return children;
  }
} // namespace gff
