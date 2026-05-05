/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#pragma once

#include <climits>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace gff
{
struct Coordinates
{
    int beg;
    int end;
    int length() const { return end - beg + 1; }
};

struct GffEntry
{
    std::string seqname; // chromosome / sequence name
    std::string source;
    std::string type; // gene, exon, CDS,
    int beg;          // 1-based, inclusive
    int end;          // 1-based, inclusive
    float score;
    int strand; // '+', '-', '.', '?`
    int phase;  // '0','1','2','.'
    std::unordered_map<std::string, std::string> attributes;
    std::string id;
    std::optional<std::string> parent;
    int length() const { return end - beg + 1; }
};

struct GffSelectedEntry
{
    std::string seqname;
    std::string source;
    std::string type;
    std::vector<gff::Coordinates> coords;
    int strand;
    std::string id;
    std::optional<std::string> parent;
};

struct TypeLengths
{
    std::string seqname;
    std::string root_id;
    std::unordered_map<std::string, int> total; // feature > summed length
    std::unordered_map<std::string, int> count; // feature > counts
};

} // namespace gff
