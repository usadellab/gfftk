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
#include "components/gffrow.h"


namespace gff
{
  class Feature
  {
    public:
      Feature(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, std::int_fast32_t start, std::int_fast32_t end);
      virtual ~Feature();
      std::string seqid;
      std::string id;
      std::string source;
      std::string type;
      const std::vector<Coordinates>& coordinates() const;
      void add_parent(gff::Feature* parent);
      void add_child(gff::Feature* child);
      gff::Feature* get_parent(const std::string& parent_id) const;
      const std::unordered_map<std::string, gff::Feature*>& get_parents();

    protected:
      std::vector<Coordinates> positions;
      std::unordered_map<std::string, gff::Feature*> parents;
      std::unordered_map<std::string, gff::Feature*> children;
      //                                    feattype                            featid        feat
      using featuremap = std::unordered_map<std::string, std::unordered_map<std::string, gff::Feature*>>;
      featuremap features;
      using typemap = std::unordered_map<std::string, gff::Feature*>;
      void add_positions(std::int_fast32_t start, std::int_fast32_t end);
      virtual gff::Feature* add_feature(const gff::GffRow& row) = 0;
      const typemap& get_type_features(const std::string& type) const;
      // void add_parent(s);
      void empty_featuremap();
  };
} // end namespace gff
