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
    // ToDo: Not wroking as expected. Doe snot exit when given invalid path
    if(!std::filesystem::status_known(fstat) ? std::filesystem::exists(fstat) : std::filesystem::exists(path))
    {
      std::cerr << "[Error] " << path << " : File not found\n";
      return(EXIT_FAILURE);
    }
    gff_in.open(path);
    if(!gff_in.is_open())
    {
      std::cerr << "[Error] " << path << " : failed to open\n";
      exit(EXIT_FAILURE);
    }
    std::cerr << "[Info] " << path << " : open for parsing\n";
    return EXIT_SUCCESS;
  }

  int GffFile::parse(gff::GffFile::Processor& proc)
  {
    int entry_status = 0;
    for(std::string line; std::getline(gff_in, line);)
    {
      ++row_num;
      if(line.empty())
      {
        continue;
      }
      if(line[0] == '#')
      {
        parse_directive(line);
        continue;
      }
      gff::GffRow row(path, row_num);
      const gff::GffFeaturePart part = row.parse(line);
      // Add row error checks here
      if(part.error == 1)
      {
        std::cerr << "[Error] " << path << "::" << row_num << " "
                  << "Unexpected number of columns. Aborting\n";
        exit(EXIT_FAILURE);
      }
      std::cout << part.seqid << "\n";
      assemble_locus(part);
      // entry_status = proc.process_entry(e, directives);
    }
    // gff::GffEntry fake;
    // entry_status = proc.process_entry(fake, directives);
    return entry_status;
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
  void GffFile::assemble_locus(const gff::GffFeaturePart& part)
  {
    /* if(!e.hasParent())  //  new locus
    {
      if(!prevloc.empty())
      {
        std::cerr << "Assessing\n";
        gff::Locus loc = loci.at(prevloc);
        // loc.show();
        gff::Locus::Feature* lf = loci.at(prevloc).find_longest_feature("CDS");
        if(lf)
        {
          std::cerr << "Found longest " << lf->type << " on " << loc.id() << ": "
                    << lf->id   << "\t" << lf->isSelected <<"\n";
          // show_feature(lf, loc, header);
        }
        list_locus_features(loci.at(prevloc), header);
      }
      gff::Locus locus = gff::Locus(e);
      loci.insert({e.id(), locus});
      std::cerr << "== New Locus: " << locus.id() << "\n";
      prevloc = locus.id();
    }
    else  //  part-of relation
    {
      //  Test if parnet feature is known to isoformscanner
      if(!hasFeature(e.parent())) {std::exit(EXIT_FAILURE);}
      gff::GffEntry p = get_feature(e.parent()); // dangerous, fix later
      p.add_child(e);
      e.add_parent(p);
      const std::string lid = get_locus_id(p);
      if(loci.contains(lid)) {loci.at(lid).add_entry(e);}
      else {std::cerr << "Error: locus " << lid << "not known\n";}
    } */
  }
}//end namespace gff
