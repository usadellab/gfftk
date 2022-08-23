/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/feature.h"

namespace gff
{
  Feature::Feature(const std::string& id, const std::string& type, std::int_fast32_t start, std::int_fast32_t end)
    : id(id), type(type), positions{ {start, end} }
  {  }

  Feature::~Feature()
  {  }

  void Feature::add_positions(std::int_fast32_t start, std::int_fast32_t end)
  {
    positions.emplace(positions.end(), Coordinates{start, end});
  }

  const std::vector<Coordinates>& Feature::coordinates() const
  {
    return positions;
  }

  gff::Feature* Feature::add_feature(gff::Feature*)
  {

  }

} // namespace gff
