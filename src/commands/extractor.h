/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <climits>

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
      int process_locus(gff::TypeFeature* locus);

    private:
      const std::string descr = "Extract features from GFF file";
      const std::string name = "extract";
      const char* const short_opts = "i:t:xlsh";
      const option long_opts[7] =
      {
            {"input", required_argument, nullptr, 'i'},
            {"taxid", required_argument, nullptr, 'x'},
            {"type", required_argument, nullptr, 't'},
            // {"min", required_argument, nullptr, ''},
            // {"min", required_argument, nullptr, ''},
            {"shortest", no_argument, nullptr, 's'},
            {"longest", no_argument, nullptr, 'l'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
      };
      unsigned long int taxid;
      std::string type;
      bool get_longest = false;
      bool get_shortest = false;
      void longest_type(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results);
      void shortest_type(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results);
      void type_by_length(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results, unsigned long min = 0, unsigned long max = LONG_MAX);
  };
} // namespace gff
