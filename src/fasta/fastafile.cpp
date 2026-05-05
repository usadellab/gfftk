/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
 */

#include "fasta/fastafile.h"

#include "gff/gffentry.h"
#include "utils/stringtools.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace fasta
{
FastaFile::FastaFile() {}
FastaFile::FastaFile(const std::string& path)
  : path(path)
{
  index();
}

void FastaFile::index()
{
  fasta.open(path);
  if(!fasta.is_open()) throw std::runtime_error("Cannot open FASTA: " + path);

  std::string line;
  std::string current;
  std::streampos offset = 0;

  while(std::getline(fasta, line))
  {
    if(line.empty())
    {
      offset = fasta.tellg();
      continue;
    }

    if(line[0] == '>')
    {
      // parse seqname: up to first space
      current = line.substr(1, line.find(' ') - 1);
      offset = fasta.tellg();
    }
    else if(!current.empty() && seqindex.find(current) == seqindex.end())
    {
      // first sequence line. Record offset and line length
      seqindex[current] = {
        offset, (int)line.size(),
        (int)line.size() + 1 // +1 for \n DO NOT FORGET
      };
    }
    offset = fasta.tellg();
  }
}

std::string FastaFile::fetch(const std::string& seqname, int beg, int end)
{
  auto it = seqindex.find(seqname);
  if(it == seqindex.end())
    throw std::runtime_error("Sequence not found: " + seqname);

  auto& info = it->second;
  int len = end - beg + 1; // 1-based, inclusive

  // calculate byte offset of beg
  int zero_beg = beg - 1; // convert to 0-based
  int full_lines = zero_beg / info.line_len;
  int remainder = zero_beg % info.line_len;
  std::streampos pos
    = info.offset + (std::streampos)(full_lines * info.full_len + remainder);

  fasta.clear();
  fasta.seekg(pos);

  // read len bases. Skip \n
  std::string seq;
  seq.reserve(len);
  char c;
  while((int)seq.size() < len && fasta.get(c))
  {
    if(c != '\n' && c != '\r') seq += c;
  }
  return seq;
}

std::string FastaFile::reverse_complement(const std::string& seq)
{
  std::string rc(seq.rbegin(), seq.rend());
  for(char& c : rc)
  {
    auto it = nuc_complement.find(c);
    if(it != nuc_complement.end()) { c = it->second; }
  }
  return rc;
}

void FastaFile::extract(const std::vector<gff::GffSelectedEntry>& entries,
                        const std::string& out_path, int line_len)
{
  std::ofstream out(out_path);
  if(!out.is_open())
  {
    throw std::runtime_error("Cannot open output: " + out_path);
  }

  for(const auto& e : entries)
  {
    // concatenate all parts
    std::string seq;
    for(const auto& coord : e.coords)
    {
      seq += fetch(e.seqname, coord.beg, coord.end);
    }

    // reverse complement if on minus strand
    if(e.strand == 1) { seq = reverse_complement(seq); }

    // write FASTA enty
    out << ">" << e.id;
    if(e.parent) { out << " parent=" << *e.parent; }
    out << " " << e.seqname << " strand=" << e.strand
        << " length=" << seq.size() << "\n";

    // wrap chars to look nice
    for(std::size_t i = 0; i < seq.size(); i += line_len)
    {
      out << seq.substr(i, line_len) << "\n";
    }
  }
  std::cerr << "Wrote isoforms to " << out_path << "\n";
}
} // namespace fasta
