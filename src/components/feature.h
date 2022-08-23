/**
 * @file gfffeature.h
 * @author jpb (jpb@members.fsf.org)
 * @date 2022-08-15
 * @copyright Copyright (c) 2022
 */

#pragma once

#include <iostream>
#include <set>

#include "gfftypes.h"


namespace gff
{
  class Feature
  {
    public:
      Feature(const std::string& id, const std::string& type, std::int_fast32_t start, std::int_fast32_t end);
      ~Feature();
      std::string id;
      std::string type;
      const std::vector<Coordinates>& coordinates() const;

    protected:
      std::vector<Coordinates> positions;
      std::set<gff::Feature*> parents;
      std::set<gff::Feature*> children;
      void add_positions(std::int_fast32_t start, std::int_fast32_t end);
      gff::Feature* add_feature(gff::Feature*);
      using featuremap = std::unordered_map<std::string, std::unordered_map<std::string, gff::Feature*>>;
      // void add_parent(s);

  };
} // end namespace gff
