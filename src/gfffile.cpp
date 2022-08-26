/*
 *------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 *------------------------------------------------------------------------------
 */

#include "gfffile.h"

namespace gff
{
  GffFile::GffFile(std::string gff_file)
  :path(gff_file)
  {
    open();
  }

  GffFile::~GffFile()
  {
    clean_up();
  }

  void GffFile::close()
  {
    if(gff_in.is_open())
    {
      std::cerr << "Info: Closing GFF: " << path << "\n";
      gff_in.close();
    }
    if(!gff_in.is_open())
    {
      std::cerr << "Info: Closed GFF: " << path << "\n";
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
      gff::GffRow row(line, path, row_num);
      // Add row error checks here
      if(row.err_code == 100)
      {
        std::cerr << "[Error] " << path << "::" << row_num << " "
                  << "Unexpected number of columns. Aborting\n";
        exit(EXIT_FAILURE);
      }
      if(row.err_code == 200)
      {
        std::cerr << "[Error] " << path << "::" << row_num << " "
                  << "Invalid GFF entry. Missing ID and Parent attribute. Skipping.\n";
        return row.err_code;
      }
      // std::cout << row.id << "\n";
      assemble_locus(row);
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

  void GffFile::assemble_locus(const gff::GffRow& row)
  {
    if(row.parents.empty())  //  new locus
    {
      std::cerr << "[Info] " << path << "::" << row_num << " " << "New Locus: " << row.id << "\n";
      gff::Locus* locus = add_locus(row);
      locus->show();
    }
    else  //  part-of relation
    {
      for(const auto& i : row.parents)
      {
        if(is_locus(i)) // parent is a locus; add as child feature of locus
        {
          gff::Locus* loc = locus(i);
          loc->add_feature(row);
          // loc->show();

        }
        else  // get parent and add as child feature of feature
        {
          std::cerr << "[Info] " << path << "::" << row_num << " " << "subfeat: " << row.id << "\t" << row.type << "\n";
          for(auto & i: row.parents)
          {
            std::cerr << "\tparent: " << i << "\n";
          }
        }
        // std::cout << "\t" << i << "\n";
      }
      // gff::Locus* loc = locus(row.id);
      // if(!loc)
      // {
      //   std::cerr << "Can't find locus " << row.id << " . Skipping\n";
      //   // ToDo: If this really happens:
      //   // 0. Create temporary locus with this id
      //   // 1. If locus is found later, reorganize locus
      //   // Warn and skip now
      //   return;
      // }
      // loc->add_feature(row);
    }
    //   if(!prevloc.empty())
    //   {
    //     std::cerr << "Assessing\n";
    //     gff::Locus loc = loci.at(prevloc);
    //     // loc.show();
    //     gff::Locus::Feature* lf = loci.at(prevloc).find_longest_feature("CDS");
    //     if(lf)
    //     {
    //       std::cerr << "Found longest " << lf->type << " on " << loc.id() << ": "
    //                 << lf->id   << "\t" << lf->isSelected <<"\n";
    //       // show_feature(lf, loc, header);
    //     }
    //     list_locus_features(loci.at(prevloc), header);
    //   }
    //   gff::Locus locus = gff::Locus(e);
    //   loci.insert({e.id(), locus});
    //   std::cerr << "== New Locus: " << locus.id() << "\n";
    //   prevloc = locus.id();
    // }
    // else  //  part-of relation
    // {
    //   //  Test if parnet feature is known to isoformscanner
    //   if(!hasFeature(e.parent())) {std::exit(EXIT_FAILURE);}
    //   gff::GffEntry p = get_feature(e.parent()); // dangerous, fix later
    //   p.add_child(e);
    //   e.add_parent(p);
    //   const std::string lid = get_locus_id(p);
    //   if(loci.contains(lid)) {loci.at(lid).add_entry(e);}
    //   else {std::cerr << "Error: locus " << lid << "not known\n";}
    // }
  }

  bool GffFile::is_locus(const std::string& locid)
  {
    return (loci.count(locid)) ? true : false;
  }

  gff::Locus* GffFile::locus(const std::string& id)
  {
    if(loci.count(id))
    {
      return loci[id];
    }
    std::cerr << "[Warning] " << path << "::" << row_num << " "
              << "Trying to access unknown locus " << id << "\n";
    return nullptr;
  }

  gff::Locus* GffFile::add_locus(const gff::GffRow& row)
  {
    if(loci.count(row.id))
    {
      loci[row.id]->extend_with_row(row);
      std::cout << "Extended locus " << loci[row.id]->id << "\n";
      return loci[row.id];
    }
    gff::Locus* locus = new gff::Locus(row.seqid, row.id, row.source, row.type, row.start, row.end);
    const auto &[it, inserted] = loci.emplace(locus->id, locus);
    if(inserted)
    {
      std::cout << "Inserted locus " << locus->id << "\n";
      return locus;
    }
    std::cerr << "[Error] " << path << "::" << row_num << " "
              << "Storing locus " << locus->id << "failed. Aborting\n";
    clean_up();
    exit(EXIT_FAILURE);
  }

  void GffFile::clean_up()
  {
    delete_loci();
    close();
  }

  void GffFile::delete_loci()
  {
    std::cout << "Loci size: " << loci.size() << "\n";
    for(auto it = loci.cbegin(); it != loci.cend();)
    {
      delete(it->second);
      it = loci.erase(it++);
    }
    std::cout << "Loci size: " << loci.size() << "\n";
  }
}//end namespace gff
