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
  :seqname(gffcols[0]), source(gffcols[1]), feature(gffcols[2]),
   start(std::stol(gffcols[3])), end(std::stol(gffcols[4]))
{
  process_comments(gffcols[8]);
}
GffEntry::~GffEntry()
{
  // up.clear();
  delete next;
  delete prev;
}

void GffEntry::process_comments(const std::string& gff_comments)
{
  for(auto& i : utils::tokenize(gff_comments, ';'))
  {
    std::vector<std::string> comment = utils::tokenize(utils::strip(i), '=');
    // std::cout << comment[0] << "\n";
    if(comment[0] == "ID")
    {
      // std::cout << comment[0]  << "\n";
      this->eid = utils::trim(comment[1]);
    }
    if(comment[0] == "Parent")
    {
      this->pid = utils::trim(comment[1]);
      // std::cout << this->feat_id << "\tadding parent:" << comment[0]  << "\t" << this->feat_parent << "\n";
    }
  }
}

void GffEntry::show_children()
{
  for(auto& i : children)
  {
    std::cout << i.id() << "\n";
  }
}

void GffEntry::show_parent()
{
  for(auto& i : parents)
  {
    std::cout << i.id() << "\n";
  }
}

void GffEntry::add_child(gff::GffEntry e)
{
  std::cout << "\t\tParent: " << id() << "\tadd child: " <<e.id() << "   " << &e << "\n";
  children.push_back(e);
  // std::cout << children.back().id() << "\n";
}
void GffEntry::add_parent(gff::GffEntry e)
{
  std::cout << "\t\tChild: " << id() << "\tadd parent: " <<e.id() << "\n";
  parents.push_back(e);
}

bool GffEntry::hasParent()
{
  return !pid.empty();
}

const std::string& GffEntry::parent () const
{
  return pid;
}
const std::string& GffEntry::id() const
{
  return eid;
}

GffEntry GffEntry::get_parent()
{
  for(auto& i : parents)
  {
    std::cout << "\t\tparent: " << i.id() << "\n";
  }
   return parents.front();
}

void GffEntry::show()
{

  std::cout << "\tSequence: " << seqname << "\tId: " << id() << "\tType: "
            << feature << "\tCoords: " << start << "\t" <<  end
            << "\tparent: " << parent() << "\n";
}


} // namespace gff
