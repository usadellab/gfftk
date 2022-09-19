/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "fasta/fastafile.h"

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

  void FastaFile::assemble(const std::unordered_map<std::string, std::vector<gff::Feature*>>)
  {
    std::string header;
    std::string sequence;
    for(std::string line; std::getline(fh, line);)
    {
      if(line[0] == '>')
      {
        if(!header.empty())
        {
          // std::cout << header << "\n" << sequence << "\n";
          std::cout << header << "\n";
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
    std::cout << header << "\n" << sequence << "\n";
  }
} // namespace fasta
