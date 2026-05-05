/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
 */

#include "command/isoformer.h"

#include "fasta/fastafile.h"
#include "gff/gfffile.h"

#include <algorithm>
#include <climits>
#include <cstring>
// #include "fasta/fastafile.h"
#include "command/command.h"

namespace gff
{
Isoformer::Isoformer() {}

Isoformer::~Isoformer() {}

void Isoformer::usage()
{
  std::cout << "Extracting isoforms from GFF\n\n"
            << "usage: gfftk isoforms --input <GFF> --type [mRNA, CDS,..] "
               "[OPTIONAL]\n\n"
            << "Mandatory:\n"
            << "\t--input, -i <path>         Path to GFF file\n"
            << "\t--type, -t  <type>         Level on which isoforms should be "
               "selected, e.g. CDS\n"
            << "Optional:\n"
            << "\t--longest, -l              Longest type\n"
            << "\t--shortest, -s             Shortest type\n"
            << "\t--fasta, -f <path>         FASTA file with sequences\n"
            << "\t--help, -h                 Show help\n";
  exit(1);
}

int Isoformer::run()
{
  try
  {
    fasta::FastaFile ff(fasta_in);
    gff::GffFile gff(gff_file);
    gff.parse();
    if(get_longest) { find_longest_isoforms(gff, ff); }
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

void Isoformer::find_longest_isoforms(gff::GffFile& gf, fasta::FastaFile& ff)
{
  auto isoforms = gf.find_longest_type(type);
  // for(const auto& e : isoforms)
  // {
  //   std::cout << e.seqname << "\t" << e.parent.value_or("None") << "\t" <<
  //   e.id
  //             << "\t" << e.coords.size() << " parts\n";
  //   for(const auto& c : e.coords)
  //   {
  //     std::cout << c.beg << "\t" << c.end << "\n";
  //   }
  // }
  ff.extract(isoforms, "isoforms.fa");
}

const std::string& Isoformer::description() { return descr; }

const std::string& Isoformer::command() { return name; }

int Isoformer::setup(int argc, char** argv)
{
  std::string sopt;
  while(true)
  {
    const auto opt
      = getopt_long(argc, argv, short_opts.c_str(), long_opts, nullptr);
    if(opt == -1) break;
    switch(opt)
    {
      case 'i':
        gff_file = optarg;
        break;

      case 't':
        sopt = optarg;
        type = stringtools::lowercase(sopt);
        break;

      case 'l':
        get_longest = true;
        break;

      case 's':
        get_shortest = true;
        break;

      case 'f':
        fasta_in = optarg;
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
  // test_input_file();
  if(type.empty())
  {
    type = "cds";
    std::cerr << "Fetching isoforms based on CDS.\n";
  }
  if(!get_shortest)
  {
    get_longest = true;
    std::cerr << "Fetching longest isoforms based on " << type << "\n";
  }
  return EXIT_SUCCESS;
}

} // namespace gff
