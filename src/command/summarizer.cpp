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

namespace gff
{
Summarizer::Summarizer() {}

Summarizer::~Summarizer() {}

void Summarizer::usage()
{
  std::cout << "Summarize GFF\n\n"
            << "usage: gfftk " << name
            << " --input <GFF> \n"
               "[OPTIONAL]\n\n"
            << "Mandatory:\n"
            << "\t--input, -i <path>    Path to GFF file\n"
            << "Optional:\n"
            << "\t--help, -h            Show this help\n";
  exit(1);
}

void Summarizer::show_summary(const GffSummary& s,
                              std::ostream& out = std::cout) const
{
  out << "\n[ GFF Summary ]\n"
      << "  total entries   : " << s.total_entries << "\n"
      << "  total parents     : " << s.total_roots << "\n"
      << "  sequences       : " << s.total_sequences << "\n"
      << "  avg roots/seq   : " << s.avg_roots_per_seq << "\n"
      << "\n  global features:\n";

  for(auto& [feat, fs] : s.global_by_feature)
    out << "    " << std::left << std::setw(20) << feat
        << "  count: " << std::setw(8) << fs.count << "  min: " << std::setw(8)
        << fs.min_length << "  max: " << std::setw(8) << fs.max_length
        << "  avg: " << std::fixed << std::setprecision(1) << fs.avg_length
        << "bp\n";

  out << "\n  per sequence:\n";
  for(auto& [seq, ss] : s.by_sequence)
  {
    out << "    " << seq << "  entries: " << ss.total_entries
        << "  parents: " << ss.root_count << "\n";
    for(auto& [feat, fs] : ss.by_feature)
      out << "      " << std::left << std::setw(20) << feat
          << "  count: " << std::setw(6) << fs.count << "  avg: " << std::fixed
          << std::setprecision(1) << fs.avg_length << "bp\n";
  }
}
int Summarizer::run()
{
  try
  {
    gff::GffFile gff(gff_file);
    gff.parse();
    show_summary(gff.summarize());
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

} // namespace gff
