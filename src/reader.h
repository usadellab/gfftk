/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#pragma once

#include <vector>
#include "gffentry.h"

namespace gff
{
  class Parser
  {
    public:
      class Processor
      {
        public:
          virtual void process_entry(gff::GffEntry e) = 0;
      };
      Parser();
      ~Parser();
      void parse(gff::Parser::Processor& proc);

    private:
  };
}//end gff namespace
