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
    std::string subseq;
    int strand;
    for(std::string line; std::getline(fh, line);)
    {
      if(line[0] == '>')
      {
        if(!header.empty())
        {
          if(extractions.count(header))
          {
            for(auto& i : extractions.at(header))
            {
              std::cout << ">" << i->id << "\t" << i->strand << "\t" << i->start() << "\t" << i->end() << "\t" << i->length() << "\n";
              for(auto& j : i->coordinates())
              {
                unsigned long  len = (j.end-j.start+1);
                if(j.start + len > sequence.size())
                {
                  std::cout << "### too long: " << header << "\t" << i->id << "\t" << i->start() << "\t" << i->end() << "\n";
                }
                subseq += sequence.substr(j.start-1, len);
              }
              // std::cout << "## " << header << "\t" << i->id << "\t" << i->start() << "\t" << i->end() << "\n";
              strand = i->strand;
              print_sequence(subseq, i->strand);
              // std::cout << subseq.length() / 3 << "\t" << "\n";
              // add checks: len % 3 == 0; len seqpart == len from gff
              std::cout << "\n";
              subseq.erase();
            }
            std::cout << "\n";
          }
          sequence.erase();
          header.erase();
        }
        header = stringtools::tokenize(line.erase(0,1), ' ')[0];
      }
      else
      {
        sequence += line;
      }
    }
    print_sequence(subseq, strand);
    // std::cout << header << "\n" << sequence << "\n";
  }


  void FastaFile::reverse_complement(std::string& seq)
  {
    std::reverse(seq.begin(), seq.end());
    for(const auto& i : seq)
    {
      if(!nuc_complement.count(i))
      {
        std::cout << "### missing complement for " << i << "\n";
        return;
      }
      std::cout << nuc_complement.at(i);
    }
  }

  void FastaFile::print_sequence(std::string& seq, int strand)
  {
    if(strand == 1)
    {
      reverse_complement(seq);
      return;
    }
    std::cout << seq;
  }
} // namespace fasta
