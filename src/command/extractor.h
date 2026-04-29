/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include "command/command.h"
#include "gff/gfffile.h"

#include <algorithm>
#include <climits>
#include <iostream>
// #include "fasta/fastafile.h"

namespace gff
{
class Extractor : public Command
{
  public:
    Extractor();
    ~Extractor();
    int setup(int argc, char** argv);
    void usage();
    int run();
    const std::string& description();
    const std::string& command();
    void find_longest_types(gff::GffFile& gf);

  private:
    const std::string descr = "Extract features from GFF file";
    const std::string name = "extract";
    const std::string short_opts = "i:t:m:M:a:f:slh";
    static inline option long_opts[] = {
      {    "input", required_argument, nullptr, 'i'},
      {     "type", required_argument, nullptr, 't'},
      {      "min", required_argument, nullptr, 'm'},
      {      "max", required_argument, nullptr, 'M'},
      {"attribute", required_argument, nullptr, 'a'},
      {    "fasta", required_argument, nullptr, 'f'},
      { "shortest",       no_argument, nullptr, 's'},
      {  "longest",       no_argument, nullptr, 'l'},
      {     "help",       no_argument, nullptr, 'h'},
      {    nullptr,                 0, nullptr,   0}
    };
    std::string type;
    std::string attribute;
    std::string fasta_in;
    bool get_longest = false;
    bool get_shortest = false;
    unsigned long minlen = 0;
    unsigned long maxlen = ULONG_MAX;
};
} // namespace gff
