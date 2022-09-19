/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/feature.h"

namespace gff
{
  Feature::Feature(const std::string& seqid, const std::string& id,
      const std::string& source, const std::string& type, const float score,
      int strand, int phase, position start, position end, attributemap attribs)
    : seqid(seqid), id(id), source(source), type(type), score(score),
      strand(strand), phase(phase), positions{{start, end}}
  {
    move_attributes_from_row(attribs);
  }

  Feature::~Feature()
  {  }

  void Feature::add_positions(const position start, const position end)
  {
    positions.emplace(positions.end(), Coordinates{start, end});
    std::sort(positions.begin(), positions.end(),[](const Coordinates& lhs,
       const Coordinates& rhs)
       {return lhs.start > rhs.start;});
  }

  const unsigned long int Feature::length() const
  {
    unsigned long int length = 0;
    for(auto& i : positions)
    {
      length += i.end - i.start + 1;
    }
    return length;
  }

  const unsigned long int Feature::size() const
  {
    return positions.size();
  }

  const std::vector<Coordinates>& Feature::coordinates() const
  {

    return positions;
  }

  bool Feature::is_locus()
  {
    return parents.empty();
  }

  void Feature::add_parent(gff::Feature* parent)
  {
    const auto &[it, inserted] = parents.try_emplace(parent->id, parent);
    /* if(inserted)
    {
      std::cerr << " inserted parent: " << parent->id;
    }
    else
    {
      std::cerr << " parent known: " << parent->id;
    } */
    parent->add_child(this);
  }

  void Feature::add_child(gff::Feature* child)
  {
    children.try_emplace(child->type, std::unordered_map<std::string, gff::Feature*> {});
    const auto &[it, ins] = children[child->type].try_emplace(child->id, child);
    /* if(!ins)
    {
      std::cerr << "[ Info ] feature " << id << " child known: "
                << it->second->id << "\n";
    } */
  }

  gff::Feature* Feature::locus()
  {
    if(is_locus())
    {
      return this;
    }
    gff::Feature* feat = this;
    while(!feat->is_locus())
    {
      for(auto& i : feat->get_parents())
      {
        feat = i.second;
      }
    }
    return feat;
  }

  gff::Feature* Feature::get_parent(const std::string& parent_id) const
  {
    if(parents.count(parent_id))
    {
      return parents.at(parent_id);
    }
    return nullptr;
  }

  const std::unordered_map<std::string, gff::Feature*>& Feature::get_parents() const
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
      std::cerr << "[ Error ] feature " << id << " cannot be extended with "
                << extender->id << "\n";
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

  bool Feature::has_identical_parent(gff::Feature* feat)
  {
    for(const auto& i : feat->get_parents())
    {
      if(!parents.count(i.second->id ))
      {
        return false;
      }
    }
    return true;
  }
  const position Feature::start() const
  {
    return positions[0].start;
  }
  const position Feature::end() const
  {
    return positions.back().end;
  }

  // try an iterator to avoid vectro intermediate
  void Feature::move_attributes_from_row(attributemap row_attribs)
  {
    std::vector<std::string> keys;
    for(auto& i : row_attribs)
    {
      keys.push_back(i.first);
    }
    for(auto& i : keys)
    {
      auto at = row_attribs.extract(i);
      at.key() = i;
      attributes.insert(std::move(at));
    }
    /* std::cout << row_attribs.size() << "\t" << attributes.size() << "\n";
    for(auto& i : attributes)
    {
      std::cout << i.first << "\n";
    } */

  }
  const attributemap& Feature::comments() const
  {
    return attributes;
  }
  void Feature::sort_coords()
  {
    std::sort(positions.begin(), positions.end(),[](const Coordinates& lhs,
                                                    const Coordinates& rhs)
                                {return lhs.start < rhs.start;});
  }
} // namespace gff
