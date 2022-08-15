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
#include "feature/featurepart.h"

namespace gff
{
  GffFile::GffFile(std::string gff_file)
  :path(gff_file)
  {
    if(!open())
    {
      std::cerr << "Error: Opening GFF failed: " << path << "\n";
      exit(EXIT_FAILURE);
    }
    std::cerr << "Info: Opened GFF for parsing: " << path << "\n";
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
      std::cerr << "Error: File not found: " << path << ". Abort\n";
      return(EXIT_FAILURE);
    }
    gff_in.open(path);
    if(!gff_in.is_open())
    {
      return(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
  }

  int GffFile::parse(gff::GffFile::Processor& proc)
  {
    int entry_status;
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
      gff::GffEntry e(linetools::tokenize(line, '\t'));
      std::vector gffcols = linetools::tokenize(line, '\t');
      gff::GffFeaturePart fpart {gffcols[0], gffcols[1]};
      entry_status = proc.process_entry(e, directives);
    }
    gff::GffEntry fake;
    entry_status = proc.process_entry(fake, directives);
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
  void GffFile::parse_attributes(const std::string& attributes)
  {
    int comment_count = 0;
    for(auto& i : linetools::tokenize(gff_comments, ';'))
    {
      ++comment_count;
      std::vector<std::string> comment = linetools::tokenize(linetools::strip(i), '=');
      if(comment[0] == "ID")
      {
        // std::cout << comment[0]  << "\n";
        this->eid = linetools::trim(comment[1]);
      }
      else if(comment[0] == "Parent")
      {
        this->pid = linetools::trim(comment[1]);  // adjust for multiple parents
        // std::cout << this->feat_id << "\tadding parent:" << comment[0]  << "\t" << this->feat_parent << "\n";
      }
      else if(comment.size() < 2)
      {
        std::cerr << "WARNING: Skipping nonvalid key-value comment at coordinates:" <<
          feat_start << " - " << feat_end <<  "\n\tComment nr: " << comment_count <<
          "\n\tComment key: " << comment.front() << "\n\tComment:" << gff_comments << "\n";
      }
      else
      {
        const auto &[it, pass] = comments.try_emplace(comment[0], std::vector<std::string> {comment[1]});
        if(!pass)
        {
          it->second.push_back(comment[1]);
        }
      }
    }
  }
}//end namespace gff
