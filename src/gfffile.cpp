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

#include "utils.h"

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
      std::cerr << "Error: File not found: " << path << ". Abort\n";
     exit(EXIT_FAILURE);
    }
    gff_in.open(path);
    if(!gff_in.is_open())
    {
      std::cerr << "Error: Opening GFF failed: " << path << "\n";
      exit(EXIT_FAILURE);
    }
    std::cerr << "Info: Opened GFF for parsing: " << path << "\n";
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
      gff::GffEntry e(utils::tokenize(line, '\t'));
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
            const auto &[it, pass] = directives.try_emplace(utils::strip(directive), std::vector<std::string>{utils::strip(val)});
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
}//end namespace gff
