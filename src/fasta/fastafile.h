/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include <components/typefeature.h>


namespace fasta
{
  class FastaFile
  {
    public:
      FastaFile(const std::string path);
      ~FastaFile();

    private:
      std::string path;
      std::ifstream fh;
      int open();
      void close();
      void assemble(gff::TypeFeature* feature);

  };
} // namespace fasta
