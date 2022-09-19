/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "fasta/fastafile.h"

#include <algorithm>
#include <filesystem>
#include <unordered_map>

#include <components/feature.h>
#include <helpers/stringtools.h>

namespace fasta
{
  FastaFile::FastaFile(const std::string path)
  : path(path)
  {
    open();
  }

  FastaFile::~FastaFile()
  {
    close();
  }

  int FastaFile::open()
  {
    std::filesystem::file_status fstat = std::filesystem::file_status {};
    // ToDo: Not wroking as expected. Doe snot exit when given invalid path
    if(!std::filesystem::status_known(fstat) ? std::filesystem::exists(fstat) : std::filesystem::exists(path))
    {
      std::cerr << "[ Error ] " << path << " : File not found\n";
      return(EXIT_FAILURE);
    }
    fh.open(path);
    if(!fh.is_open())
    {
      std::cerr << "[ Error ] " << path << " : failed to open\n";
      exit(EXIT_FAILURE);
    }
    std::cerr << "[ Info ] " << path << " : open\n";
    return EXIT_SUCCESS;
  }

  void FastaFile::close()
  {
    if(fh.is_open())
    {
      std::cerr << "[ Info ] Closing FASTA: " << path << "\t";
      fh.close();
    }
    if(!fh.is_open())
    {
      std::cerr << "OK\n";
    }
    else
    {
      std::cerr << "failed\n";
    }
  }

  void FastaFile::assemble(const std::unordered_map<std::string,
                           std::vector<gff::Feature*>> extractions)
  {
    std::string header;
    std::string sequence;
    for(std::string line; std::getline(fh, line);)
    {
      if(line[0] == '>')
      {
        if(!header.empty())
        {
          if(extractions.count(header))
          {
            std::string seqpart;
            for(auto& i : extractions.at(header))
            {
              std::cout << ">" << header << "\t" << i->strand << "\n";
              for(auto& j : i->coordinates())
              {
                seqpart += sequence.substr(j.start-1, (j.end-j.start+1));
              }
              if(i->strand == 1)
              {
                reverse_complement(seqpart);
              }
              else
              {
                std::cout << seqpart;
              }
              // std::cout << seqpart.length() / 3 << "\t" << "\n";
              // add checks: len % 3 == 0; len seqpart == len from gff
              std::cout << "\n";
              seqpart.erase();
            }
            std::cout << "\n";
          }
          sequence.erase();
        }
        line = line.substr(1);
        header = stringtools::tokenize(line, ' ')[0];
      }
      else
      {
        sequence += line;
      }
    }
    // std::cout << header << "\n" << sequence << "\n";
  }


  void FastaFile::reverse_complement(std::string& seq)
  {
    std::reverse(seq.begin(), seq.end());
    for(const auto& i : seq)
    {
      std::cout << nuc_complement.at(i);
    }
  }
} // namespace fasta
