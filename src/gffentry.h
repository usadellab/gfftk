
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
  struct GffEntry
  {
    std::string seqname;
    std::string source;
    std::string feature;
    std::int_fast32_t start;
    std::int_fast32_t end;
    std::string gene_id;
    std::string transcript_id;
    //int strand;
    //float score;
    //int frame;
     //std::map<std::string, std::string> comments;
  };
} //end namespace gff
