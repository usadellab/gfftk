/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include "commands/command.h"
#include "gfffile.h"

#include <algorithm>
#include <climits>
#include <iostream>

namespace gff
{
class Viewer
  : public Command
  , GffFile::Processor
{
  public:
    Viewer();
    ~Viewer();
    int setup(int argc, char** argv);
    void usage();
    void run();
    const std::string& description();
    const std::string& command();
    int process_locus(gff::TypeFeature* locus);

  private:
    const std::string descr = "View compact version of GFF";
    const std::string name = "view";
    std::string source;
    const char* const short_opts = "i:hs:";
    const option long_opts[4] = {
      { "input", required_argument, nullptr, 'i'},
      {  "help",       no_argument, nullptr, 'h'},
      {"source", optional_argument, nullptr, 's'},
      { nullptr,       no_argument, nullptr,   0}
    };
    void compact_view(const gff::Feature* feat, const gff::Feature* locus,
                      int level = 0);
};
} // namespace gff
