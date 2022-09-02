/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include <fstream>
#include <iostream>

#include "commands/command.h"

namespace gff
{
  Command::Command()
  { }

  Command::~Command()
  {  }

  int Command::test_input_file()
  {
    if(gff_file.empty())
    {
      std::cerr << "Error: Path to file is required\n";
      usage();
      return EXIT_FAILURE;
    }
  }
} // namespace gff
