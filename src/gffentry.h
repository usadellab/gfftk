
/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>

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
      std::string parent;
      std::string id;
      //int strand;
      //float score;
      //int frame;
       //std::map<std::string, std::string> comments;

    private:
      void process_comments(const std::string gff_comments);


  };
} //end namespace gff
