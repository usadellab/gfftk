/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include <iostream>

#include "gfftk.h"

namespace gff
{
  GffTk::GffTk()
  { }

  GffTk::~GffTk()
  { }

  void GffTk::parse_args(int argc, char **argv)
  {
    bool cmd_exists = false;
    for(auto& i : commands)
    {
      if(i.name == argv[1])
      {
        cmd_exists = true;
        i.cmd->run();
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
      delete i.cmd;
    }
  }

  void GffTk::list_commands()
  {
    for(const auto& i : commands)
    {
      std::cout << i.name << "\t" << i.cmd->description << "\n";
    }
  }

  void GffTk::setup_commands()
  {
    commands.push_back(
      Command{"extract", new gff::Extractor}

      );
  };
} // namespace gff
