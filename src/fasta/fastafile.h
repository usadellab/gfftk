/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

#include <components/feature.h>
#include <helpers/stringtools.h>


namespace fasta
{
  class FastaFile
  {
    public:
      FastaFile(const std::string path);
      ~FastaFile();
      void assemble(const std::unordered_map<std::string,
                    std::vector<gff::Feature*>>);

    private:
      std::string path;
      std::ifstream fh;
      int open();
      void close();
      void reverse_complement(std::string& seq);
      void print_sequence(std::string& seq, int strand);
      inline static const std::unordered_map<char, char> nuc_complement
        { {'A', 'T'}, {'T', 'A'}, {'G', 'C'}, {'C', 'G'}, {'N', 'N'} };
  };
} // namespace fasta
