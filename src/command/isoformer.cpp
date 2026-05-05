/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
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
  std::cout
    << "Extracting isoforms from GFF\n\n"
    << "usage: gfftk isoforms --input <GFF> --type [mRNA, CDS, exon, ..] "
       "[OPTIONAL]\n\n"
    << "Mandatory:\n"
    << "\t--input, -i <path>    Path to GFF file\n"
    << "\t--type,  -t <type>    Level on which isoforms should be "
       "selected, e.g. CDS. Default: CDS\n"
    << "\t--fasta, -f <path>    FASTA file from which to extract isoforms\n"
    // << "\t--out,   -o <path>      FASTA file to which write isoforms\n"
    << "Optional:\n"
    << "\t--longest, -l         Longest type (Default)\n"
    << "\t--shortest, -s        Shortest type\n"
    << "\t--help, -h            Show this help\n";
  exit(1);
}

int Isoformer::run()
{
  try
  {
    std::cerr << "Opening input FASTA file " << fasta_in << "\n";
    fasta::FastaFile ff(fasta_in);
    std::cerr << "Parsing GFF file " << gff_file << "\n";
    gff::GffFile gff(gff_file);
    gff.parse();
    if(get_longest) { find_longest_isoforms(gff, ff); }
    if(get_shortest) { find_shortest_isoforms(gff, ff); }

    std::cerr << "Finished extracting isoforms\n";
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
  ff.extract(isoforms, fasta_out);
}
void Isoformer::find_shortest_isoforms(gff::GffFile& gf, fasta::FastaFile& ff)
{
  auto isoforms = gf.find_shortest_type(type);
  ff.extract(isoforms, fasta_out);
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

      case 'l':
        get_longest = true;
        break;

      case 's':
        get_shortest = true;
        break;

      case 'f':
        fasta_in = optarg;
        break;

      case 'o':
        fasta_out = optarg;
        break;

      case 't':
        sopt = optarg;
        type = stringtools::lowercase(sopt);
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
  if(fasta_in.empty())
  {
    std::cerr << "Error: path for input FASTA file is required\n";
    usage();
    return EXIT_FAILURE;
  }
  if(fasta_out.empty())
  {
    std::cerr << "Error: Output path to isoform FASTA file required\n";
    usage();
    return EXIT_FAILURE;
  }
  if(!get_shortest)
  {
    get_longest = true;
    std::cerr << "Fetching longest isoforms based on " << type << "\n";
  }
  else
  {
    std::cerr << "Fetching shortest isoforms based on " << type << "\n";
  }
  return EXIT_SUCCESS;
}

} // namespace gff
