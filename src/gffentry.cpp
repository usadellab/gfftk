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
    :seqname(gffcols[0]),
     source(gffcols[1]),
     feature(gffcols[2]),
     start(std::stol(gffcols[3])),
     end(std::stol(gffcols[4]))
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
        this->id = utils::trim(comment[1]);
      }
      if(comment[0] == "Parent")
      {
        // std::cout << comment[0]  << "\n";
        this->parent = utils::trim(comment[1]);
      }
    }
  }
} // namespace gff
