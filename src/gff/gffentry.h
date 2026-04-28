/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace gff
{
struct GffEntry
{
    std::string seqname; // chromosome / sequence name
    std::string source;
    std::string feature; // gene, exon, CDS,
    int beg;             // 1-based, inclusive
    int end;             // 1-based, inclusive
    float score;
    int strand; // '+', '-', '.'
    int phase;  // '0','1','2','.'
    std::unordered_map<std::string, std::string> attributes;
    std::string id;
    std::optional<std::string> parent;

    // bool overlaps(int lo, int hi) const { return start <= hi && end >= lo; }
};

} // namespace gff
