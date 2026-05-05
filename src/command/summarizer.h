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
#include <numeric>
#include <string>

namespace gff
{

enum class SummaryMode
{
  Count,
  AvgLength,
  TotalLength
};

class Summarizer : public Command
{
  public:
    explicit Summarizer();
    Summarizer(const Summarizer&) = delete;
    Summarizer& operator=(const Summarizer&) = delete;
    Summarizer(Summarizer&&) = delete;
    Summarizer& operator=(Summarizer&&) = delete;
    int setup(int argc, char** argv);
    void usage();
    int run();
    const std::string& description();
    const std::string& command();
    void print_transposed_summary(const GffSummary& s,
                                  SummaryMode mode = SummaryMode::Count) const;
    static float average(std::vector<float> v);
    static float median(std::vector<float> v);
    static float average(std::vector<int> v);
    static float median(std::vector<int> v);

  private:
    const std::string descr = "Summarize GFF file";
    const std::string name = "summarize";
    const std::string short_opts = "i:h";
    static inline option long_opts[] = {
      {"input", required_argument, nullptr, 'i'},
      { "help",       no_argument, nullptr, 'h'},
      {nullptr,                 0, nullptr,   0}
    };
    std::ostream& outstr;
    const int col_width = 12;
    const int seq_width = 20;
};

} // namespace gff
