/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <climits>
#include <algorithm>

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
      const char* const short_opts = "i:t:x:m:M:a:slh";
      const option long_opts[10] =
      {
        {"input", required_argument, nullptr, 'i'},
        {"type", required_argument, nullptr, 't'},
        {"taxid", required_argument, nullptr, 'x'},
        {"min", required_argument, nullptr, 'm'},
        {"max", required_argument, nullptr, 'M'},
        {"attribute", required_argument, nullptr, 'a'},
        {"shortest", no_argument, nullptr, 's'},
        {"longest", no_argument, nullptr, 'l'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
      };
      unsigned long int taxid;
      std::string type;
      std::string attribute;
      bool get_longest = false;
      bool get_shortest = false;
      unsigned long minlen = 0;
      unsigned long maxlen = LONG_MAX;
      bool type_by_length(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results,
                          unsigned long min = 0, unsigned long max = LONG_MAX);
      void show_results(const gff::TypeFeature* locus, std::vector<const gff::Feature*>& results) const;
      void show_feature(const gff::Feature* feature) const;
  };
} // namespace gff
