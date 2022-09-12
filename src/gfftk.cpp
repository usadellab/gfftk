/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include <iostream>

#include "gfftk.h"


  GffTk::GffTk()
  {  }

  GffTk::~GffTk()
  {
    cleanup();
  }

  void GffTk::parse_args(int argc, char **argv)
  {
    if(argc < 2)
    {
      std::cerr <<  "Expecting a command\n";
      list_commands();
      exit(EXIT_FAILURE);
    }
    for(auto i : commands)
    {
      if(i->command() == argv[1])
      {
        i->setup(argc, argv);
        i->run();
        exit(EXIT_SUCCESS);
      }
    }
    std::cerr <<  "Unknown command: " << argv[1] <<"\n";
    list_commands();
    exit(EXIT_FAILURE);
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
      std::cout << "\t" << i->command() << "\t\t:" << i->description() << "\n";
    }
  }

  void GffTk::setup_commands()
  {
    commands.push_back(new gff::Extractor());
  }

int main(int argc, char **argv)
{
  GffTk gfftk;
  gfftk.setup_commands();
  gfftk.parse_args(argc, argv);
}
