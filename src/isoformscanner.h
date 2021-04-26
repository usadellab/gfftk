/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */


#include <iostream>
#include "reader.h"

class IsoformScanner : public gtf::Parser::Processor
{
  public:
    IsoformScanner();
    ~IsoformScanner();
    void process_entry(struct gtf::GtfEntry& e);
};

