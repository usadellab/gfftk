
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

      //int strand;
      //float score;
      //int frame;
       //std::map<std::string, std::string> comments;
      bool hasParent();
      const std::string& parent() const;
      const std::string& id() const;
      void show();

    private:
      void process_comments(const std::string gff_comments);
      std::string feat_parent;
      std::string feat_id;
  };
} //end namespace gff
