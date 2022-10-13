/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "components/gffrow.h"

#include <iostream>
#include <unordered_map>

#include "helpers/stringtools.h"
#include "gfftypes.h"

namespace gff
{
  std::int_fast32_t GffRow::anon_feat_count = 0;

  GffRow::GffRow(std::string& gff_row, const std::string& gff_file, int rownum)
    : rownum(rownum), gff_file(gff_file)
  {
    parse(gff_row);
  }

  GffRow::~GffRow()
  { }

  // Unhappy about the approach assinging attributes via vector indices.
  // 1. Store the row vector and create getters ?
  void GffRow::parse(std::string& gffrow)
  {
    columns gffcols = stringtools::tokenize(gffrow, '\t');
    if(gffcols.size() != expected_columns)
    {
      std::cerr << "[ Error ] " << gff_file << "::" << rownum << " "
                << "Unexpected number of columns: " << gffcols.size()
                << ". Expected: " << expected_columns << "\n";
      err_code = 100;
      return;
    }

    seqid = gffcols[0];
    source = gffcols[1];
    type = stringtools::lowercase(gffcols[2]);
    start = std::stol(gffcols[3]);
    end = std::stol(gffcols[4]);
    score = (gffcols[5] == ".") ? -1.0 : std::stof(gffcols[5]);
    strand = convert_strand(gffcols[6]);
    if(strand < 0)
      {std::cerr << "[ Warning ]: Bad value for strand\n";}

    phase = (gffcols[7] == ".") ? -1 : std::stoi(gffcols[7]);
    if(phase < -1 || phase > 2)
      {std::cerr << "[ Warning ]: Bad value for phase\n";}

    parse_attributes(gffcols[8]);
    if(!attributes.count(id_key) && !attributes.count(parent_key))
    {
      std::cerr << "[ Warning ] " << gff_file << "::" << rownum << " "
                << "Invalid GFF entry. Missing ID and Parent attribute\n";
      err_code = 200;
      return;
    }
    if(attributes.count(id_key))
    {
      auto nh = attributes.extract(id_key);
      id = nh.mapped()[0];
      //std::hash<std::string>{}(seqid+source+type+std::to_string(positions[0].start)+std::to_string(positions[-1].end));
    }
    else
    {
      id = std::to_string(anon_feat_count++);
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
    for(std::string& i : stringtools::tokenize(attribute_line, ';'))
    {
      ++comment_count;
      std::vector<std::string> comment = stringtools::tokenize(stringtools::strip(i), '=');
      if(comment.size() != 2)
      {
        std::cerr << "[ Warning ] Skipping invalid key-value comment on line :"
                   << rownum << "\tComment nr: " << comment_count
                   << "\tComment key: " << comment.front()  << "\n";
      }
      else
      {
        const std::vector<std::string> values = stringtools::tokenize(comment[1], ',');
        const auto &[it, pass] = attributes.try_emplace(comment[0], values);
        if(!pass)
        {
          it->second.insert(it->second.end(),
                            std::make_move_iterator(values.cbegin()),
                            std::make_move_iterator(values.cend()));
        }
      }
    }
  }
} // end namespace gff
