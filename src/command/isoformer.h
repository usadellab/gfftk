/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#pragma once

#include "command/command.h"
#include "fasta/fastafile.h"
#include "gff/gfffile.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <string>

namespace gff
{

class Isoformer : public Command
{
  public:
    Isoformer();
    ~Isoformer();
    int setup(int argc, char** argv);
    void usage();
    int run();
    const std::string& description();
    const std::string& command();
    void find_longest_isoforms(gff::GffFile& gf, fasta::FastaFile& ff);
    void find_shortest_isoforms(gff::GffFile& gf, fasta::FastaFile& ff);

  private:
    const std::string descr = "Extract isoforms from GFF file";
    const std::string name = "isoform";
    const std::string short_opts = "i:t:m:M:a:f:o:slh";
    static inline option long_opts[] = {
      {   "input", required_argument, nullptr, 'i'},
      {    "type", required_argument, nullptr, 't'},
      {   "fasta", required_argument, nullptr, 'f'},
      {     "out", required_argument, nullptr, 'f'},
      {"shortest",       no_argument, nullptr, 's'},
      { "longest",       no_argument, nullptr, 'l'},
      {    "help",       no_argument, nullptr, 'h'},
      {   nullptr,                 0, nullptr,   0}
    };
    std::string type;
    std::string attribute;
    std::string fasta_in;
    std::string fasta_out;
    bool get_longest = false;
    bool get_shortest = false;
};
} // namespace gff
