/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
 */

#include "command/extractor.h"

#include "gff/gfffile.h"

#include <algorithm>
#include <climits>
#include <cstring>
// #include "fasta/fastafile.h"
#include "command/command.h"

namespace gff
{
Extractor::Extractor() {}

Extractor::~Extractor() {}

void Extractor::usage()
{
  std::cout
    << "Extracting features from GFF, e.g. CDS features\n\n"
    << "usage: gfftk extract --input <GFF> --type [mRNA, CDS,..] [OPTIONAL]\n\n"
    << "Mandatory:\n"
    << "\t--input, -i <path>         Path to GFF file\n"
    << "\t--type, -t  <type>         GFF type, e.g. mRNA\n"
    << "Optional:\n"
    << "\t--longest, -l              longest type\n"
    << "\t--shortest, -s             shortest type\n"
    << "\t--min, -m <int>            min lentgth\n"
    << "\t--max, -M <int>            max lentgth\n"
    << "\t--attribute, -a <str>      select attribute\n"
    << "\t--fasta, -f <path>         FASTA file with sequences\n"
    << "\t--help, -h                 Show help\n";
  exit(1);
}

int Extractor::run()
{
  try
  {
    gff::GffFile gff(gff_file);
    gff.parse();
    if(get_longest) { find_longest_types(gff); }
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

void Extractor::find_longest_types(gff::GffFile& gf)
{

  // gf.find_all_subtypes_for_id("gene-GAD3");
  std::vector<gff::GffEntry*> parents = gf.find_all_parents();
  for(auto& p : parents)
  {
    std::cout << p->id << "\n";
    struct longest_type
    {
        std::string parent;
        std::string id;
        std::vector<gff::GffEntry*> parts;
    };
    std::vector<gff::GffEntry*> types
      = gf.find_all_subtypes_for_id(p->id, type);
    if(types.size() == 0) { continue; }
    if(types.size() == 1)
    {
      std::cout << types.at(0)->id << "\t" << types.at(0)->beg << "\t"
                << types.at(0)->end << "\t"
                << "\n";
      continue;
    }
    longest_type prev
      = {types.at(0)->parent.value_or("None"), types.at(0)->id, {}};
    for(auto& t : types)
    {
      if(t->parent != prev.parent)
      {
        if(!prev.parent.empty())
        {
          std::cout << prev.id << "\n";
          for(auto& c : prev.parts)
          {
            std::cout << c->beg << "\t" << c->end << "\t" << "\n";
          }
        }
        prev.parent = *(t->parent);
        prev.id = t->id;
        prev.parts.push_back(t);
      }
      else
      {
        prev.parts.push_back(t);
      }
    }
    if(!prev.parent.empty())
    {
      std::cout << prev.id << "\n";
      for(auto& c : prev.parts)
      {
        std::cout << c->beg << "\t" << c->end << "\t" << "\n";
      }
    }
  }
}

const std::string& Extractor::description() { return descr; }

const std::string& Extractor::command() { return name; }

int Extractor::setup(int argc, char** argv)
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

      case 'm':
        minlen = atol(optarg);
        break;

      case 'M':
        maxlen = atol(optarg);
        break;

      case 'a':
        attribute = optarg;
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
    std::cerr << "Need to know the type, e.g. mRNA.\n";
    usage();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

} // namespace gff
