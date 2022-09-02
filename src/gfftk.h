/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <set>

#include "gfffile.h"
#include "commands/extractor.h"

namespace gff
{
  class GffTk
  {
    public:
      GffTk();
      ~GffTk();
      void parse_args(int argc, char **argv);

    private:
      struct Command
      {
        std::string name;
        gff::Command* cmd;
      };
      void usage();
      void list_commands();
      std::vector<Command> commands;
      void setup_commands();
      void cleanup();
  };
} // namespace gff
