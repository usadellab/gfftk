/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/feature.h"

namespace gff
{
  Feature::Feature(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, std::int_fast32_t start, std::int_fast32_t end)
    : seqid(seqid), id(id), source(source), type(type), positions{ {start, end}}
  {  }

  Feature::~Feature()
  {
    empty_featuremap();
  }

  void Feature::add_positions(std::int_fast32_t start, std::int_fast32_t end)
  {
    positions.emplace(positions.end(), Coordinates{start, end});
  }

  const std::vector<Coordinates>& Feature::coordinates() const
  {
    return positions;
  }

  void Feature::empty_featuremap()
  {
    std::cout << "On: " << id << ": predel feat size: " << features.size() << "\n";
    for(auto it = features.begin(); it != features.end();)
    {
      for(auto itt = it->second.begin(); itt != it->second.end();)
      {
        delete(itt->second);
        itt++;
      }
      it++;
    }
    features.clear();
    std::cout << "On: " << id << ": postdel feat size: " << features.size() << "\n";
  }

  const Feature::typemap& Feature::get_type_features(const std::string& type) const
  {
    if(features.count(type)) // check if feature exist
    {
      return features.at(type);
    }
    return typemap {};
  }

  void Feature::add_parent(gff::Feature* parent)
  {
    const auto &[it, inserted] = parents.try_emplace(parent->id, parent);
    if(!inserted)
    {
      std::cout << "Insertion of parnent: " << parent->id << " failed\n";
      exit(EXIT_FAILURE);
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

  // bool Feature::is_parent(const std::string& parent_id)
  // {
  //   if
  // }
} // namespace gff
