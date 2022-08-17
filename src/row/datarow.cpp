/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "row/datatrow.h"

#include <iostream>

#include "locus/featurepart.h"
#include "helpers/linetools.h"

namespace gff
{
  DataRow::DataRow()
  { }

  DataRow::~DataRow()
  { }

  const DataRow::gffattributes DataRow::parse_attributes(std::string attribute_line)
  {
    std::unordered_map<std::string, std::vector<std::string>> atrributes;
    int comment_count = 0;
    for(auto& i : linetools::tokenize(attribute_line, ';'))
    {
      ++comment_count;
      const std::vector<std::string> comment = linetools::tokenize(linetools::strip(i), '=');
      if(comment.size() < 2)
      {
        std::cerr << "WARNING: Skipping invalid key-value comment on line :" << line_num <<
                      "\n\tComment nr: " << comment_count     <<
                      "\n\tComment key: " << comment.front()  << "\n";
      }
      else
      {
        const auto &[it, pass] = atrributes.try_emplace(comment[0], std::vector<std::string> {comment[1]});
        if(!pass)
        {
          it->second.push_back(comment[1]);
        }
      }
    }
    return atrributes;
  }
} // end namespace gff
