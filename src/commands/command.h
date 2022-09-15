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
      virtual ~Command();
      virtual int setup(int argc, char **argv) = 0;
      virtual void usage() = 0;
      virtual void run() = 0;
      virtual const std::string& description() = 0;
      virtual const std::string& command() = 0;

    protected:
      std::string gff_file;
      int test_input_file();

    private:
      const char* const short_opts = {};
      const option long_opts[0] = {};
  };
} // namespace gff
