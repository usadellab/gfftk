/*
 *------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 *------------------------------------------------------------------------------
 */

#include "gfffile.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "helpers/linetools.h"
#include "locus/featurepart.h"

namespace gff
{
  GffFile::GffFile(std::string gff_file)
  :path(gff_file)
  {
    open();
  }

  GffFile::~GffFile()
  {

  }

  void GffFile::close()
  {
    if(gff_in.is_open())
    {
      std::cerr << "Info: Closing GFF: " << path << "\n";
      gff_in.close();
    }
  }

  int GffFile::open()
  {
    std::filesystem::file_status fstat = std::filesystem::file_status {};
    if(! std::filesystem::status_known(fstat) ? std::filesystem::exists(fstat) : std::filesystem::exists(path))
    {
      std::cerr << "[Error] " << path << " :File not found\n";
      return(EXIT_FAILURE);
    }
    gff_in.open(path);
    if(!gff_in.is_open())
    {
      std::cerr << "[Error] " << path << " :failed to open\n";
      exit(EXIT_FAILURE);
    }
    std::cerr << "[Info] " << path << " :open for parsing\n";
    return EXIT_SUCCESS;
  }

  int GffFile::parse(gff::GffFile::Processor& proc)
  {
    int entry_status = 0;
    for(std::string line; std::getline(gff_in, line);)
    {
      ++line_num;
      if(line.empty())
      {
        continue;
      }
      if(line[0] == '#')
      {
        parse_directive(line);
        continue;
      }
      gff::GffFeaturePart gfp = row_to_featurepart(linetools::tokenize(line, '\t'));
      std::cout << gfp.seqid << "\n";
      // entry_status = proc.process_entry(e, directives);
    }
    // gff::GffEntry fake;
    // entry_status = proc.process_entry(fake, directives);
    return entry_status;
  }

  gff::GffFeaturePart GffFile::row_to_featurepart(const std::vector<std::string>& gffcols)
  {
    float score = -1.0;
    if(gffcols[5] != ".")
    {
      score = std::stof(gffcols[5]);
    }
    const std::unordered_map<std::string, std::vector<std::string>> attribs = parse_attributes(gffcols[8]);
    return gff::GffFeaturePart {gffcols[0],
                                gffcols[1],
                                gffcols[2],
                                std::stol(gffcols[3]),
                                std::stol(gffcols[4]),
                                score,
                                strand_to_int(gffcols[6]),
                                phase_to_int(gffcols[7])};
  }

  int GffFile::strand_to_int(const std::string& strandval) const
  {
    if(strandval == "+"){return 0;}
    if(strandval == "-"){return 1;}
    if(strandval == "."){return 2;}
    std::cerr << "Warning: Bad vavlue for strand\n";
    return -1; // Error
  }

  int GffFile::phase_to_int(const std::string& phaseval) const
  {
    if(phaseval == "."){return -1;}
    return std::stoi(phaseval);
  }



  void GffFile::parse_directive(const std::string& line)
  {
    int status = 0;
    int directive_beg;
    for(long unsigned int i = 0; i < line.size(); ++i)
    {
      // std::cout << i << " : " << line[i] << " " << status << "\n";
      switch(status)
      {
        case(0):
          if(line[i] != '#' )
          {
            status = 1;
            break;
          }
          ++i;

        case(1):
          if(line[i] != '#' || line[i] != '!')
          {
            ++i;
            directive_beg = i;
            status = 2;
            break;
          }
          ++i;

        case(2):
          if(line[i] == ' ')
          {
            std::string directive = line.substr(directive_beg, i-1);
            std::string val = line.substr(i+1);
            const auto &[it, pass] = directives.try_emplace(linetools::strip(directive), std::vector<std::string>{linetools::strip(val)});
            if(!pass)
              {it->second.push_back(line.substr(i+1));}
            return;
          }
          continue;
        default:
          return;
      }
    }
  }
  const std::unordered_map<std::string, std::vector<std::string>> GffFile::parse_attributes(std::string attribute_line)
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
}//end namespace gff
