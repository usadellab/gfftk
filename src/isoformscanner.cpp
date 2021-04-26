/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#include <iostream>
#include "reader.h"
#include "isoformscanner.h"

IsoformScanner::IsoformScanner()
  { }
IsoformScanner::~IsoformScanner()
  { }

void IsoformScanner::process_entry(struct gtf::GtfEntry& e)
{

}



int main(int argc, char **argv)
{
  gtf::Parser p;
  p.parse();
  return 0;
}

