/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 */

#include "command/summarizer.h"

#include "command/command.h"
#include "gff/gfffile.h"
#include "summaries/summaries.h"

#include <algorithm>
#include <climits>
#include <cstring>
#include <numeric>

namespace gff
{
Summarizer::Summarizer()
  : outstr(std::cout)
{
}

void Summarizer::usage()
{
  std::cout << "Summarize GFF and print to STDOUT\n\n"
            << "usage: gfftk " << name << " --input <GFF> \n\n"
            << "Mandatory:\n"
            << "\t--input, -i <path>  Path to GFF file\n"
            << "Optional:\n"
            << "\t--help,  -h         Show this help\n";
  exit(1);
}

int Summarizer::run()
{
  try
  {
    gff::GffFile gff(gff_file);
    gff.parse();
    print_transposed_summary(gff.summarize());
    std::cerr << "Finished summary\n";
  }
  catch(const gff::GffFileNotFound& e)
  {
    std::cerr << "[ Error ] " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch(const gff::GffException& e)
  {
    std::cerr << "[ Error ] " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return 0;
}

const std::string& Summarizer::description() { return descr; }

const std::string& Summarizer::command() { return name; }

int Summarizer::setup(int argc, char** argv)
{
  while(true)
  {
    const auto opt
      = getopt_long(argc, argv, short_opts.c_str(), long_opts, nullptr);
    if(opt == -1) { break; }
    switch(opt)
    {
      case 'i':
        gff_file = optarg;
        break;

      case 'h': // -h or --help
        usage();
        return EXIT_SUCCESS;

      case '?': // Unrecognized option
        return EXIT_FAILURE;

      default:
        usage();
        break;
    }
  }
  if(gff_file.empty())
  {
    std::cerr << "Require a GFF file as input\n";
    usage();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void Summarizer::print_transposed_summary(const GffSummary& s,
                                          SummaryMode mode) const
{
  std::cerr << "[ GFF Summary ]\n";
  outstr << "sequence\troots";
  for(const auto& feat : s.types)
    outstr << "\t" << feat;
  outstr << "\n";

  // per sequence rows
  for(auto& [seqname, ss] : s.by_sequence)
  {
    outstr << seqname << "\t" << ss.root_count;
    for(const auto& feat : s.types)
    {
      auto it = ss.by_type.find(feat);
      float val = 0.0f;
      if(it != ss.by_type.end())
      {
        switch(mode)
        {
          case SummaryMode::Count:
            val = it->second.count;
            break;
          case SummaryMode::AvgLength:
            val = it->second.avg_length;
            break;
          case SummaryMode::TotalLength:
            val = it->second.total_length;
            break;
        }
      }
      outstr << "\t" << val;
    }
    outstr << "\n";
  }

  outstr << std::fixed << std::setprecision(2);

  // add total row
  outstr << "total\t" << s.total_roots;
  for(const auto& feat : s.types)
  {
    const auto& fs = s.global_by_type.at(feat);
    switch(mode)
    {
      case SummaryMode::Count:
        outstr << "\t" << fs.count;
        break;
      case SummaryMode::AvgLength:
        outstr << "\t" << fs.avg_length;
        break;
      case SummaryMode::TotalLength:
        outstr << "\t" << fs.total_length;
        break;
    }
  }
  outstr << "\n";

  // average row
  outstr << "average\t" << average(s.root_counts_per_seq);
  for(const auto& feat : s.types)
    outstr << "\t" << average(s.values_per_seq.at(feat));
  outstr << "\n";

  // median row
  outstr << "median\t" << median(s.root_counts_per_seq);
  for(const auto& feat : s.types)
    outstr << "\t" << median(s.values_per_seq.at(feat));
  outstr << "\n";
}

float Summarizer::average(std::vector<float> v)
{
  if(v.empty()) return 0.0f;
  return std::accumulate(v.begin(), v.end(), 0.0f) / v.size();
}

float Summarizer::median(std::vector<float> v)
{
  if(v.empty()) return 0.0f;
  std::sort(v.begin(), v.end());
  int mid = v.size() / 2;
  return v.size() % 2 == 0 ? (v[mid - 1] + v[mid]) / 2.0f : v[mid];
}

float Summarizer::average(std::vector<int> v)
{
  if(v.empty()) return 0.0f;
  return std::accumulate(v.begin(), v.end(), 0.0f) / v.size();
}

float Summarizer::median(std::vector<int> v)
{
  if(v.empty()) return 0.0f;
  std::sort(v.begin(), v.end());
  int mid = v.size() / 2;
  return v.size() % 2 == 0 ? (v[mid - 1] + v[mid]) / 2.0f : v[mid];
}

} // namespace gff
