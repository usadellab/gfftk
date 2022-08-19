/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/gffrow.h"

#include <iostream>

namespace gff
{
  GffRow::GffRow(std::string& gff_row, const std::string& gff_file, int rownum)
    : gff_file(gff_file),rownum(rownum)
  {
    parse(gff_row);
  }

  GffRow::~GffRow()
  { }

  // Unhappy about the approach assinging attributes via vector indices.
  // 1. Store the row vector and create getters ?
  void GffRow::parse(std::string& gffrow)
  {
    int error = 0;  // for later use
    columns gffcols = linetools::tokenize(gffrow, '\t');
    if(gffcols.size() != expected_columns)
    {
      std::cerr << "[Error] " << gff_file << "::" << rownum << " "
                << "Unexpected number of columns: " << gffcols.size()
                << ". Expected: " << expected_columns << "\n";
    }

    seqid = gffcols[0];
    source = gffcols[1];
    type = gffcols[2];
    start = std::stol(gffcols[3]);
    end = std::stol(gffcols[4]);
    score = (gffcols[5] == ".") ? -1.0 : std::stof(gffcols[5]);
    strand = convert_strand(gffcols[6]);
    if(strand < 0)
      {std::cerr << "Warning: Bad value for strand\n";}

    phase = (gffcols[7] == ".") ? -1 : std::stoi(gffcols[7]);
    if(phase < -1 || phase > 2)
      {std::cerr << "Warning: Bad value for phase\n";}

    parse_attributes(gffcols[8]);
    if(attributes.count(id_key))
    {
      auto nh = attributes.extract(id_key);
      id = nh.mapped()[0];
    }
    if(attributes.count(parent_key))
    {
      auto nh = attributes.extract(parent_key);
      parents = nh.mapped();
    }
  }

  int GffRow::convert_strand(const std::string& strandstr) const
  {
    if(strandstr == "+"){return 0;}
    if(strandstr == "-"){return 1;}
    if(strandstr == "."){return 2;}
    return -1; // Error
  }

  void GffRow::parse_attributes(std::string& attribute_line)
  {
    int comment_count = 0;
    for(std::string& i : linetools::tokenize(attribute_line, ';'))
    {
      ++comment_count;
      const std::vector<std::string> comment = linetools::tokenize(linetools::strip(i), '=');
      if(comment.size() < 2)
      {
        std::cerr << "WARNING: Skipping invalid key-value comment on line :" << rownum <<
                      "\n\tComment nr: " << comment_count     <<
                      "\n\tComment key: " << comment.front()  << "\n";
      }
      else
      {
        const auto &[it, pass] = attributes.try_emplace(comment[0], std::vector<std::string> {comment[1]});
        if(!pass)
        {
          it->second.push_back(comment[1]);
        }
      }
    }
  }
} // end namespace gff
