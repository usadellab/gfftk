
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
#include <map>

namespace gff
{

class GffEntry
{
  public:
    GffEntry(const std::vector<std::string>& gffcols);
    ~GffEntry();
    std::string seqname;
    std::string source;
    std::string feature;
    std::int_fast32_t start;
    std::int_fast32_t end;
    void add_child(gff::GffEntry e);
    void add_parent(gff::GffEntry e);
    GffEntry* next = nullptr;
    GffEntry* prev = nullptr;
    //int strand;
    //float score;
    //int frame;
     //std::map<std::string, std::string> comments;
    bool hasParent();
    const std::string& parent() const;
    const std::string& id() const;
    void show_children();
    void show_parent();
    void show();
    GffEntry get_parent();

  private:
    void process_comments(const std::string& gff_comments);
    std::string pid;
    std::string eid;
    std::unordered_map<std::string, std::vector<gff::GffEntry>> featuresl;
    std::vector<gff::GffEntry> children;
    std::vector<gff::GffEntry> parents;
};

} //end namespace gff
