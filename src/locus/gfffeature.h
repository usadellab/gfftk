/**
 * @file gfffeature.h
 * @author jpb (jpb@members.fsf.org)
 * @date 2022-08-15
 * @copyright Copyright (c) 2022
 */

#pragma once

#include <iostream>

#include "locus/featurepart.h"

namespace gff
{
class Feature
{
  public:
    Feature(gff::GffFeaturePart& fpart);
    Feature(const std::string& id);
    ~Feature();
    std::string id;
    std::string type;
    std::string parent_id;
    std::int_fast32_t start;
    std::int_fast32_t end;
    const std::int_fast32_t length() const;
    void add_entry(gff::GffEntry e);
    void expand(gff::GffFeaturePart& fpart);
    void show_entries();
    void update_coords(gff::GffEntry e);
    void show();
    const std::vector<std::string> get_comment(const std::string& key) const;
    const std::string& sequence() const;
    bool hasComment(const std::string& commentkey);
    bool isSelected = false;

  private:
    std::vector<gff::GffEntry> entries;
    void update_length(std::int_fast32_t entry_len);
    std::int_fast32_t feat_length = 0;
    void update(gff::GffFeaturePart& fpart);
};
} // end namespace gff
