/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/gffrow.h"

#include <iostream>

namespace gff
{
  GffRow::GffRow(const std::string& gff_file, int row_number)
    : gff_file(gff_file),row_number(row_number)
  { }

  GffRow::~GffRow()
  { }

  const gff::GffFeaturePart GffRow::parse(std::string& gffrow)
  {
    int error = 0;  // for later use
    columns gffcols = linetools::tokenize(gffrow, '\t');
    if(gffcols.size() != expected_columns)
    {
      std::cerr << "[Error] " << gff_file << "::" << row_number << " "
                << "Unexpected number of columns: " << gffcols.size()
                << ". Expected: " << expected_columns << "\n";
      gff::GffFeaturePart gfp;
      gfp.error = 1;
      return gfp;
    }

    const float score = (gffcols[5] == ".") ? -1.0 : std::stof(gffcols[5]);
    const int strand = convert_strand(gffcols[6]);
    if(strand < 0)
      {std::cerr << "Warning: Bad value for strand\n";}

    const int phase = (gffcols[7] == ".") ? -1 : std::stoi(gffcols[7]);
    if(phase < -1 || phase > 2)
      {std::cerr << "Warning: Bad value for phase\n";}
    parse_attributes(gffcols[8]);
    return gff::GffFeaturePart
    {
      (attributes.count(id_key) > 0) ? attributes[id_key][0] : std::string(),                 // featurepart ID
      (attributes.count(parent_key) > 0) ? attributes[id_key] : std::vector<std::string> {},  // parents
      gffcols[0],                                                                             // seqid
      gffcols[1],                                                                             // source
      gffcols[2],                                                                             // type
      std::stol(gffcols[3]),                                                                  // start
      std::stol(gffcols[4]),                                                                  // end
      score,
      strand,
      phase
    };
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
        std::cerr << "WARNING: Skipping invalid key-value comment on line :" << row_number <<
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
