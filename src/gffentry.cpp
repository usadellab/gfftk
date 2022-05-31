/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include "gffentry.h"

#include <iostream>
#include <vector>

#include "utils.h"

namespace gff
{

GffEntry::GffEntry(const std::vector<std::string>& gffcols)
  :feat_seq(gffcols[0]), feat_source(gffcols[1]), feat_type(gffcols[2]),
   feat_start(std::stol(gffcols[3])), feat_end(std::stol(gffcols[4]))
{
  process_comments(gffcols[8]);
}

GffEntry::GffEntry()
  :feat_seq("DUMMY"), feat_source("DUMMY"), feat_type("DUMMY"),
   feat_start(0), feat_end(0)
{
}

GffEntry::~GffEntry()
{ }

bool GffEntry::hasId()
  {return !eid.empty();}
bool GffEntry::hasParent()
  {return !pid.empty();}
const std::string& GffEntry::parent() const
  {return pid;}
const std::string& GffEntry::id() const
  {return eid;}
const std::string& GffEntry::feature() const
  {return feat_type;}
const std::string& GffEntry::sequence() const
  {return feat_seq;}
std::int_fast32_t GffEntry::end()
  {return feat_end;}
std::int_fast32_t GffEntry::start()
  {return feat_start;}
std::int_fast32_t GffEntry::length()
  {return (feat_end - feat_start + 1); }

void GffEntry::process_comments(const std::string& gff_comments)
{
  int comment_count = 0;
  for(auto& i : utils::tokenize(gff_comments, ';'))
  {
    ++comment_count;
    std::vector<std::string> comment = utils::tokenize(utils::strip(i), '=');
    if(comment[0] == "ID")
    {
      // std::cout << comment[0]  << "\n";
      this->eid = utils::trim(comment[1]);
    }
    else if(comment[0] == "Parent")
    {
      this->pid = utils::trim(comment[1]);  // adjust for multiple parents
      // std::cout << this->feat_id << "\tadding parent:" << comment[0]  << "\t" << this->feat_parent << "\n";
    }
    else if(comment.size() < 2)
    {
      std::cerr << "WARNING: Skipping nonvalid key-value comment at coordinates:" <<
        feat_start << " - " << feat_end <<  "\n\tComment nr: " << comment_count <<
        "\n\tComment key: " << comment.front() << "\n\tComment:" << gff_comments << "\n";
    }
    else
    {
      const auto &[it, pass] = comments.try_emplace(comment[0], std::vector<std::string> {comment[1]});
      if(!pass)
      {
        it->second.push_back(comment[1]);
      }
    }
  }
}

void GffEntry::show_children()
{
  for(auto& i : children)
    {i.show();}
}

void GffEntry::show_parent()
{
  for(auto& i : parents)
    {std::cout << i.id() << "\n";}
}

void GffEntry::add_child(gff::GffEntry e)
{
  children.push_back(e);
  // std::cout << "\t\tChild: " << children.size() << "\n";
}

void GffEntry::add_parent(gff::GffEntry e)
{
  // std::cout << "\t\tChild: " << id() << "\tadd parent: " <<e.id() << "\n";
  parents.push_back(e);
}

GffEntry GffEntry::get_parent()
{
  for(auto& i : parents)
  {
    std::cerr << "\t\tparent: " << i.id() << "\n";
  }
   return parents.front();
}

void GffEntry::show()
{
  std::cerr << "\tEntry: " << id() <<  "\tSequence: " << sequence()
            << "\tType: " << feature() << "\tCoords: " << start() << "\t" << end()
            << "\tLength: " << length() << "\tparent: " << parent()
            << "\tChildren: " << children.size() << "\n";
}

const std::vector<std::string> GffEntry::get_comment(const std::string& key) const
{
  if(comments.contains(key))
  {
    return comments.at(key);
  }
  return std::vector<std::string> {};
}
bool GffEntry::hasComment(const std::string& commentkey)
{
  return comments.contains(commentkey);
}

} // namespace gff
