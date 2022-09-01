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
      // ToDo: Add row error checks here
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
      gff::Feature* feat = add_feature(row);
      assemble_locus(feat);
      // entry_status = proc.process_entry(e, directives);
    }
    assemble_locus(prev_loc);

    // std::cout << "====== Feature summary ======\n";
    // for(const auto &i : features)
    // {
    //   if(i.second->is_locus())
    //   {
    //     walk_features(i.second, 0);
    //   }
    // }
    return entry_status;
  }

  void GffFile::walk_features(const gff::Feature* feat, int level)
  {
    std::cout << std::string(level, '\t') << feat->id << "\t" << feat->type <<  "\n";
    if(feat->get_children().size())
    {
      ++level;
      for(auto& i : feat->get_children())
      {
        for(auto& j : i.second)
        {
          walk_features(j.second, level);
        }
      }
      --level;
    }
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

  gff::TypeFeature* GffFile::add_feature(const gff::GffRow& row)
  {
    gff::TypeFeature* feat = new gff::TypeFeature(row.seqid, row.id, row.source, row.type, row.start, row.end);
    std::cout << "[Info] " << feat->id << "\t" << feat->type << "\t";
    for(const auto &i : row.parents)
    {
      feat->add_parent(get_feature(i));
    }

    const auto &[it, inserted] = features.try_emplace(feat->id, feat);
    if(inserted)  // feature does not exist at locus
    {
      std::cout << "\tinserted\n";
      return feat;
    }
    if(feat->is_duplicate(it->second)) // check for duplicate entry
    {
      std::cerr << "[Warning] identified likely identical features" << path << "::" << row_num
                << " " << it->second->id << " and " << feat->id << "\n";
      delete feat;
      return nullptr;
    }
    std::cerr << "\textending with " << it->second->id << "\n";
    if(!it->second->extend_with(feat))
    {
      std::cerr << "[Warning] Extending " << feat->id << " with " << it->second
                << "failed\n";
      return nullptr;
    }
    delete feat;
    return it->second;
  }

  gff::TypeFeature* GffFile::get_feature(const std::string& id)
  {
    if(features.count(id))
    {
      return features[id];
    }
    std::cerr << "Warning: feature " << id << " not found\n";
    return nullptr;
  }

  gff::Feature* GffFile::assemble_locus(gff::Feature* feat)
  {
    if(feat->is_locus())
    {
      if(prev_loc)
      {
        walk_features(prev_loc, 0);
      }
      prev_loc = feat;
    }



      // locus->show();
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


  void GffFile::clean_up()
  {
    empty_features();
    close();
  }

  void GffFile::empty_features()
  {
    std::cout << "Stored features: " << features.size() << "\n";
    for(auto it = features.cbegin(); it != features.cend();)
    {
      std::cerr << "[Info] Deleting " << it->first << "\n";
      delete(it->second);
      it = features.erase(it++);
    }
    std::cout << "Remaining features: " << features.size() << "\n";
  }
}//end namespace gff
