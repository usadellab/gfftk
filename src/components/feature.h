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
      Feature(const std::string& seqid, const std::string& id, const std::string& source, const std::string& type, position start, position end);
      virtual ~Feature();
      std::string seqid;
      std::string id;
      std::string source;
      std::string type;
      using childrenmap = std::unordered_map<std::string, std::unordered_map<std::string, gff::Feature*>>;
      const childrenmap& get_children() const;
      const std::vector<Coordinates>& coordinates() const;
      void add_child(gff::Feature* child);
      void add_parent(gff::Feature* parent);
      gff::Feature* get_parent(const std::string& parent_id) const;
      const std::unordered_map<std::string, gff::Feature*>& get_parents();
      bool is_duplicate(const gff::Feature*) const;
      bool extend_with(const gff::Feature*);
      bool is_locus();
      gff::Feature* locus();
      const unsigned long int length() const;
      const unsigned long int size() const;

    protected:
      std::vector<Coordinates> positions;
      std::unordered_map<std::string, gff::Feature*> parents;
      childrenmap children;
      void add_positions(const position start, const position end);

  };
} // end namespace gff
