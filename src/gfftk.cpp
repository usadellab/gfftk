/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include <iostream>

#include "gfftk.h"

namespace gff
{

  GffTk::GffTk()
  {  }

  GffTk::~GffTk()
  {
    std::cout << "exittest\n";
    cleanup();
  }

  void GffTk::parse_args(int argc, char **argv)
  {
    if(argc < 2)
    {
      std::cerr <<  "Expecting a command\n";
      usage();
      exit(EXIT_FAILURE);
    }
    if(argv[1] == std::string("-h"))
    {
      usage();
      return;
    }
    for(auto i : commands)
    {
      if(i->command() == argv[1])
      {
        if(i->setup(argc, argv))
        {
          std::cerr << "Error: Setting up " << i->command() << "failed\n";
          exit(EXIT_FAILURE);
        }
        i->run();
        return;
      }
    }
    std::cerr <<  "Unknown command: " << argv[1] <<"\n";
    list_commands();
    cleanup();
    exit(EXIT_FAILURE);
  }

  void GffTk::usage()
  {
    std::cout << "usage: gfftk <command> [args]\n\n"
              << "gfftk is a toolkit to analyze GFF files\n";
    list_commands();
    exit(EXIT_SUCCESS);
  }

  void GffTk::cleanup()
  {
    for(auto& i : commands)
    {
      delete i;
    }
  }

  void GffTk::list_commands()
  {
    std::cout << "Available commands:\n";
    for(const auto& i : commands)
    {
      std::cout << "\t" << i->command() << "\t\t" << i->description() << "\n";
    }
  }

  void GffTk::setup_commands()
  {
    commands.push_back(new gff::Extractor());
    commands.push_back(new gff::Viewer());
  }
} // namespace gff

int main(int argc, char **argv)
{
  gff::GffTk gfftk;
  gfftk.setup_commands();
  gfftk.parse_args(argc, argv);
}
