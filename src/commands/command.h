/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>
#include <getopt.h>

namespace gff
{

  class Command
  {
    public:
      Command();
      ~Command();
      virtual int parse(int argc, char **argv) = 0;
      virtual void usage() = 0;
      virtual void run() = 0;
      const std::string description;

    protected:
      const char* const short_opts = {};
      const option long_opts[0] = {};
      std::string gff_file;
      int test_input_file();
  };
} // namespace gff
