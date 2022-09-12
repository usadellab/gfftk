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
  {  }

  void Extractor::usage()
  {
    std::cout <<  "Extracting features from GFF, e.g. CDS features\n"
              <<  "\t--input, -i <path>         Path to GFF file\n"
              <<  "\t--taxid, -x <NCBI taxid>   NCBI taxid\n"
              <<  "\t--type, -t  <type>         GFF type, e.g. mRNA\n"
              <<  "\t--longest, -l              longest type\n"
              <<  "\t--shortest, -s             shortest type\n"
              // <<  "\t--min                      min lentgth\n"
              // <<  "\t--max                      max lentgth\n"
              <<  "\t--help, -h                 Show help\n";
    exit(1);
  }

  void Extractor::run()
  {
    gff::GffFile gff(gff_file);
    gff.parse(*this);
  }

  const std::string& Extractor::description()
  {
    return descr;
  }

  const std::string& Extractor::command()
  {
    return name;
  }

  int Extractor::setup(int argc, char **argv)
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

        case 'x':
          taxid = atol(optarg);
          break;

        case 't':
          type = optarg;
          break;

        case 'l':
          get_longest = true;
          break;

        case 's':
          get_shortest = true;
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
    test_input_file();
    return EXIT_SUCCESS;
  }

  int Extractor::process_locus(gff::TypeFeature* locus)
  {
    std::vector<const gff::Feature*> results;
    if(get_longest)
    {
      longest_type(locus, results);
    }
    else if(get_shortest)
    {
      /* code */
    }
    else
    {

    }


    for(auto& i : results)
    {
      std::cout << i->id << "\t" << i->type << "\t" << i->length() << "\n";
    }
    return EXIT_SUCCESS;
  }

  void Extractor::longest_type(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results)
  {
    std::vector<const gff::Feature*> types;
    locus->get_types(type, types);
    if(types.empty())
    {
      std::cerr << "[ Info ]\tType not found: " << type <<  "\n";
      return;
    }
    const gff::Feature* longest = types[0];
    for(const auto i : types)
    {
      // std::cerr << locus->id << "\t" << i->id << "\t" << i->type << "\t" << i->length() << "\n";
      if(i->length() > longest->length())
      {
        longest = i;
      }
      // std::cout << i->id << "\t" <<i->type << "\t"  << LONG_MAX << "\n";
    }
    results.push_back(longest);
  }
} // namespace gff