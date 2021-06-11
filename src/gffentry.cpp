/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include <iostream>
#include <vector>

#include "gffentry.h"
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
   {}

  void GffEntry::process_comments(const std::string gff_comments)
  {
    for(auto& i : utils::tokenize(gff_comments, ';'))
    {
      std::vector<std::string> comment = utils::tokenize(utils::strip(i), '=');
      // std::cout << comment[0] << "\n";
      if(comment[0] == "ID")
      {
        // std::cout << comment[0]  << "\n";
        this->feat_id = utils::trim(comment[1]);
      }
      if(comment[0] == "Parent")
      {
        // std::cout << comment[0]  << "\n";
        this->feat_parent = utils::trim(comment[1]);
      }
    }
  }

  bool GffEntry::hasParent()
  {
    return !feat_parent.empty();
  }
  const std::string& GffEntry::parent() const
  {
    return feat_parent;
  }
  const std::string& GffEntry::id() const
  {
    return feat_id;
  }

  void GffEntry::show()
  {
    std::cout << "Sequence: " << seqname << " Locus: " << id() << "Type: " << feature <<
                 "Coords: " << start << "\t" <<  end << "\n";
  }
} // namespace gff
