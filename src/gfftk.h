/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include "command/command.h"
#include "command/extractor.h"
#include "gff/gfffile.h"

#include <iostream>
#include <set>

namespace gff
{

class GffTk
{
  public:
    GffTk();
    ~GffTk();
    void parse_args(int argc, char** argv);
    void setup_commands();

  private:
    void usage();
    void list_commands();
    std::vector<gff::Command*> commands;
    void cleanup();
};
} // namespace gff
