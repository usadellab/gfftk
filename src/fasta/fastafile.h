/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include "gff/gffentry.h"
#include "utils/stringtools.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace fasta
{

class FastaFile
{
  public:
    FastaFile();
    explicit FastaFile(const std::string& path);
    void extract(const std::vector<gff::GffLongestEntry>& entries,
                 const std::string& out_path, int line_len = 60);

  private:
    std::string path;
    std::ifstream fasta;
    void index();
    std::string fetch(const std::string& seqname, int beg, int end);
    std::string reverse_complement(const std::string& seq);
    inline static const std::unordered_map<char, char> nuc_complement{
      {'A', 'T'},
      {'T', 'A'},
      {'G', 'C'},
      {'C', 'G'},
      {'a', 't'},
      {'t', 'a'},
      {'g', 'c'},
      {'c', 'g'},
      {'N', 'N'},
      {'n', 'n'}
    };
    // seqname:: offset of first base in file
    struct SeqInfo
    {
        std::streampos offset; // file offset of first base
        int line_len;          // num bases / line
        int full_len;          // length of full lines, including \n
    };
    std::unordered_map<std::string, SeqInfo> seqindex;
};
} // namespace fasta
