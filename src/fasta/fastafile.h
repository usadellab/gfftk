/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include <components/feature.h>
#include <helpers/stringtools.h>


namespace fasta
{
  class FastaFile
  {
    public:
      FastaFile(const std::string path);
      ~FastaFile();
      void assemble(const std::unordered_map<std::string, std::vector<gff::Feature*>>);

    private:
      std::string path;
      std::ifstream fh;
      int open();
      void close();


  };
} // namespace fasta
