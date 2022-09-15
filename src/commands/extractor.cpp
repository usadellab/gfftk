/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include "commands/extractor.h"

#include <iostream>

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
    std::string sopt;
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
          sopt = optarg;
          type = stringtools::lowercase(sopt);
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
    type_by_length(locus, results);
    if(results.empty())
    {
      return EXIT_SUCCESS;
    }
    std::sort(results.begin(), results.end(),[](const gff::Feature* lhs,
       const gff::Feature* rhs)
       {return lhs->length() > rhs->length();});

    if(get_longest && get_shortest && results.size() == 1)
    {
      std::cout << results[0]->seqid << "\t" << results[0]->id << "\t" << results[0]->type << "\t"
                << results[0]->length() << "\t" << results[0]->start() << "\t" << results[0]->end() << "\t" << results[0]->size() <<"\n";

      return EXIT_SUCCESS; // temporary
    }
    if(get_longest)
    {
      std::cout << results[0]->seqid << "\t" << results[0]->id << "\t" << results[0]->type << "\t"
      << results[0]->length() << "\t" << results[0]->start() << "\t" << results[0]->end() << "\t" << results[0]->size() <<"l\n";
    }
    if(get_shortest)
    {
      std::cout << results.back()->seqid << "\t" << results.back()->id << "\t"
                << results.back()->type << "\t" << results.back()->length()
                << "\t" << results.back()->start() << "\t"
                << results.back()->end() << "\t" << results.back()->size() <<"s\n";
    }

/*     for(const auto& i : results)
    {
      std::string parents;
      for(const auto& j : i->get_parents())
      {
        parents += j.first;
      }

      std::cout << i->seqid << "\t" << i->id << "\t" << i->type << "\t"
      << i->length() << "\t" << i->start() << "\t" << i->end() << "\t" << parents << "\n";
    } */
    return EXIT_SUCCESS; // temporary
  }

  void Extractor::type_by_length(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results, unsigned long min, unsigned long max)
  {
    std::set<const gff::Feature*> types;
    locus->get_types(type, types);
    if(types.empty())
    {
      std::cerr << "[ Info ]\t"<< locus->id << ":" << type <<  " not found\n";
      return;
    }
    for(const auto& i : types)
    {
      // std::cout << "\t" << locus->id << "\t" << i->id << "\t" << i->type << "\t" << i->length() << "\n";
      if(i->length() >= min &&  i->length() <= max)
      {
        results.push_back(i);
      }
    }
  }
} // namespace gff
