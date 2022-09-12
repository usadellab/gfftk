/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>

#include "commands/command.h"
#include "gfffile.h"

namespace gff
{
  class Extractor : public Command, GffFile::Processor
  {
    public:
      Extractor();
      ~Extractor();
      int setup(int argc, char **argv);
      void usage();
      void run();
      const std::string& description();
      const std::string& command();
      int process_entry(gff::Feature* locus);

    private:
      const std::string descr = "Extract features from GFF file";
      const std::string name = "extract";
      const char* const short_opts = "i:th";
      const option long_opts[4] =
      {
            {"input", required_argument, nullptr, 'i'},
            {"taxid", required_argument, nullptr, 't'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
      };
      unsigned long int taxid;
  };
} // namespace gff
