/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#pragma once

#include "command/command.h"
#include "gff/gfffile.h"
#include "summaries/summaries.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <string>

namespace gff
{

class Summarizer : public Command
{
  public:
    Summarizer();
    ~Summarizer();
    int setup(int argc, char** argv);
    void usage();
    int run();
    const std::string& description();
    const std::string& command();
    void show_summary(const gff::GffSummary& s, std::ostream&) const;

  private:
    const std::string descr = "Summarize GFF file";
    const std::string name = "summarize";
    const std::string short_opts = "i:h";
    static inline option long_opts[] = {
      {"input", required_argument, nullptr, 'i'},
      { "help",       no_argument, nullptr, 'h'},
      {nullptr,                 0, nullptr,   0}
    };
};
} // namespace gff
