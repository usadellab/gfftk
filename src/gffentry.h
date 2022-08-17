
/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>

namespace gff
{

class GffEntry
{
  public:
    GffEntry(std::vector<std::string>& gffcols);
    GffEntry();
    ~GffEntry();
    const std::string& sequence() const;
    const std::string& source() const;
    const std::string& parent() const;
    const std::string& id() const;
    const std::string& feature() const;
    std::int_fast32_t start();
    std::int_fast32_t end();
    std::int_fast32_t length();
    void add_child(gff::GffEntry e);
    void add_parent(gff::GffEntry e);
    const std::vector<std::string> get_comment(const std::string& key) const;
    bool hasComment(const std::string& commentkey);
    bool hasParent();
    bool hasId();
    void show_children();
    void show_parent();
    void show();
    GffEntry get_parent();

  private:
    std::string pid;
    std::string eid;
    std::string feat_seq;
    std::string feat_source;
    std::string feat_type;
    std::int_fast32_t feat_start;
    std::int_fast32_t feat_end;
    std::vector<gff::GffEntry> children;
    std::vector<gff::GffEntry> parents;
    std::unordered_map<std::string, std::vector<std::string>> comments;
    void process_comments(std::string& gff_comments);
};

} //end namespace gff
