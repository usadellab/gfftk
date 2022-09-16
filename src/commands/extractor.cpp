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
    std::cout <<  "Extracting features from GFF, e.g. CDS features\n\n"
              <<  "usage: gfftk extract --input <GFF> --type [mRNA, CDS,..] [OPTIONAL]\n\n"
              <<  "Mandatory:\n"
              <<  "\t--input, -i <path>         Path to GFF file\n"
              <<  "\t--type, -t  <type>         GFF type, e.g. mRNA\n"
              <<  "Optional:\n"
              <<  "\t--taxid, -x <NCBI taxid>   NCBI taxid\n"
              <<  "\t--longest, -l              longest type\n"
              <<  "\t--shortest, -s             shortest type\n"
              <<  "\t--min, -m <int>            min lentgth\n"
              <<  "\t--max, -M <int>            max lentgth\n"
              <<  "\t--attribute, -a <str>      select attribute\n"
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
      if (opt == -1)
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

        case 'm':
          minlen = atol(optarg);
          break;

        case 'M':
          maxlen = atol(optarg);
          break;

        case 'a':
          attribute = optarg;
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
    if(type.empty())
    {
      std::cerr << "Need to know the type, e.g. mRNA.\n";
      usage();
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  int Extractor::process_locus(gff::TypeFeature* locus)
  {
    std::vector<const gff::Feature*> results;
    if(!type_by_length(locus, results, minlen, maxlen))
    {
      return EXIT_SUCCESS;
    }

    std::sort(results.begin(), results.end(),[](const gff::Feature* lhs,
       const gff::Feature* rhs)
       {return lhs->length() > rhs->length();});
    if(results.empty())
    {
      std::cerr << "[ Info ]\t" << locus->id << ": no matches\n";
      return EXIT_SUCCESS;
    }

    show_results(locus, results);
    return EXIT_SUCCESS; // temporary
  }

  bool Extractor::type_by_length(gff::TypeFeature* locus, std::vector<const gff::Feature*>& results, unsigned long min, unsigned long max)
  {
    std::set<const gff::Feature*> types;
    locus->get_types(type, types);
    if(types.empty())
    {
      std::cerr << "[ Info ]\t"<< locus->id << ": no " << type <<  " found\n";
      return false;
    }
    for(const auto& i : types)
    {
      if(i->length() >= min &&  i->length() <= max)
      {
        results.push_back(i);
      }
    }
    return true;
  }

  void Extractor::show_results(const gff::TypeFeature* locus, std::vector<const gff::Feature*>& results) const
  {
    if(results.empty())
    {
      std::cerr << "[ Info ]\t" << locus->id << ": no matches\n";
      return;
    }
    if(results.size() == 1)
    {
      show_feature(results.front());
    }
    else if(get_longest)
    {
      show_feature(results.front());
    }
    else if(get_shortest)
    {
      show_feature(results.back());
    }
    else
    {
      for(const auto& i : results)
      {
        /* std::string parents;
        for(const auto& j : i->get_parents())
        {
          parents += j.first;
        }
        std::cout << i->seqid << "\t" << i->id << "\t" << i->type << "\t"
        << i->length() << "\t" << i->start() << "\t" << i->end() << "\t" << parents << "\n"; */
        show_feature(i);
      }
    }
  }

  void Extractor::show_feature(const gff::Feature* feature) const
  {
    std::cout << feature->seqid << "\t" << feature->id << "\t" << feature->type
              << "\t" << feature->length() << "\t" << feature->start() << "\t"
              << feature->end() << "\t" << feature->size();
    if(!attribute.empty())
    {
      if(feature->comments().count(attribute))
      {
        std::cout << "\t" << feature->comments().at(attribute)[0];
      }
    }
    std::cout << "\n";
  }
} // namespace gff
