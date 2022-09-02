/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include <iostream>

#include "commands/extractor.h"

namespace gff
{
  Extractor::Extractor()
  {  }

  Extractor::~Extractor()
  {
  }

  void Extractor::usage()
  {
    std::cout <<  "Extracting features from GFF, e.g. CDS features\n"
              <<  "\t--input, -i <path>           Path to GFF file\n"
              <<  "\t--taxid, -t <NCBI taxid>:    NCBI taxid\n"
              <<  "\t--help, -h:                  Show help\n";
    exit(1);
  }

  int Extractor::parse(int argc, char **argv)
  {
    while(true)
    {
      const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

      if (-1 == opt)
        break;

      switch (opt)
      {
        case 'i':
          gff_file = optarg;
          break;

        case 't':
          taxid = atol(optarg);
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
  }
} // namespace gff
