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
    std::unordered_map<std::string, TypeSummary> by_type;
};

struct GffSummary
{
    int total_entries = 0;
    int total_roots = 0;
    int total_sequences = 0;
    float avg_roots_per_seq = 0.0f;
    std::vector<std::string> types; // sorted feature names
    std::unordered_map<std::string, TypeSummary> global_by_type;
    std::unordered_map<std::string, SequenceSummary> by_sequence;

    // per-feature value vectors — precomputed for stats
    std::unordered_map<std::string, std::vector<float>> values_per_seq;
    std::vector<int> root_counts_per_seq;
};

} // namespace gff
