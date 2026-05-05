/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#pragma once

#include <climits>
#include <iostream>
#include <string>
#include <unordered_map>

namespace gff
{

struct TypeSummary
{
    std::string type;
    int count = 0;
    int total_length = 0;
    int min_length = INT_MAX;
    int max_length = 0;
    float avg_length = 0.0f;
};

struct SequenceSummary
{
    std::string seqname;
    int total_entries = 0;
    int root_count = 0;
    std::unordered_map<std::string, gff::TypeSummary> by_feature;
};

struct GffSummary
{
    int total_entries = 0;
    int total_roots = 0;
    int total_sequences = 0;
    float avg_roots_per_seq = 0.0f;
    std::unordered_map<std::string, gff::TypeSummary> global_by_feature;
    std::unordered_map<std::string, gff::SequenceSummary> by_sequence;
};

} // namespace gff
