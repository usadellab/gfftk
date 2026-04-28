/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2026"
 */

#include "gff/gfffile.h"
// #include "utils/stringtools.h"

#include <iostream>

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <file.gff>\n";
    return 1;
  }
  try
  {
    gff::GffFile gff(argv[1]);
    gff.parse();
  }
  catch(const gff::GffFileNotFound& e)
  {
    std::cerr << "[ Error ] " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch(const gff::GffException& e)
  {
    std::cerr << "[ Error ] " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
