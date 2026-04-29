/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <getopt.h>
#include <iostream>

namespace gff
{

class Command
{
  public:
    Command();
    virtual ~Command();
    virtual int setup(int argc, char** argv) = 0;
    virtual void usage() = 0;
    virtual int run() = 0;
    virtual const std::string& description() = 0;
    virtual const std::string& command() = 0;

  protected:
    std::string gff_file;
    int test_input_file();
};
} // namespace gff
