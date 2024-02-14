/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
 */

#include "commands/viewer.h"

#include "commands/command.h"
#include "gfffile.h"

#include <algorithm>
#include <climits>
#include <iostream>

namespace gff
{
Viewer::Viewer() {}

Viewer::~Viewer() {}

void Viewer::usage()
{
  std::cout << "Create compact view of GFF file\n"
            << "\t--input, -i <path>         Path to GFF file\n"
            << "\t--source, -s <STR>         Optional: name of organism / "
               "annotation source\n"
            << "\t--help, -h                 Show help\n";
}

void Viewer::run()
{
  gff::GffFile gff(gff_file);
  gff.parse(*this);
}

const std::string& Viewer::description() { return descr; }

const std::string& Viewer::command() { return name; }

int Viewer::setup(int argc, char** argv)
{
  std::string sopt;
  while(true)
  {
    const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
    if(opt == -1) break;

    switch(opt)
    {
      case 'i':
        gff_file = optarg;
        break;

      case 's':
        source = std::string(optarg);
        break;

      case 'h':
        usage();
        return EXIT_SUCCESS;

      case '?': // Unrecognized option
        return EXIT_FAILURE;

      default:
        usage();
        break;
    }
  }
  test_input_file();
  return EXIT_SUCCESS;
}

int Viewer::process_locus(gff::TypeFeature* locus)
{
  compact_view(locus, locus);
  return EXIT_SUCCESS; // temporary
}

void Viewer::compact_view(const gff::Feature* feat, const gff::Feature* locus,
                          int level)
{
  // std::cout << std::string(level, '\t') << feat->type << ":" << feat->id
  //           << "\n";
  std::cout << locus->id << "\t" << feat->type << "\t" << feat->id << "\t"
            << feat->start() << "\t" << feat->end() << "\t"
            << feat->start() - locus->start() << "\t"
            << feat->start() - locus->start()
                 + (feat->end() - feat->start() + 1);
  if(!source.empty()) { std::cout << "\t" << source; }
  std::cout << "\n";
  if(!feat->get_children().empty())
  {
    ++level;
    for(const auto& i : feat->get_children())
    {
      // std::sort(i.second.begin(), i.second.end(),[](const gff::Feature* lhs,
      // const gff::Feature* rhs)
      // {return lhs->length() > rhs->length();});
      for(const auto& j : i.second)
      {
        compact_view(j.second, locus, level);
      }
    }
    --level;
  }
}

} // namespace gff
