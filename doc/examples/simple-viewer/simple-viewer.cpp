/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2022
*/

#include <iostream>

#include "simple-viewer.h"

SimpleViewer::SimpleViewer()
{ }

SimpleViewer::~SimpleViewer()
{ }

int SimpleViewer::process_locus(gff::TypeFeature* locus)
{
  view(locus);
  return EXIT_SUCCESS; // temporary
}

void SimpleViewer::view(const gff::Feature* feat, int level)
{
  std::cout << std::string(level, '\t') << feat->type << ":" << feat->id <<"\n";
  if(!feat->get_children().empty())
  {
    ++level;
    for(const auto& i : feat->get_children())
    {
      for(const auto& j : i.second)
      {
        view(j.second, level);
      }
    }
    --level;
  }
}

int main(int argc, char **argv)
{
  SimpleViewer v;
  gff::GffFile gff(argv[1]);
  gff.parse(v);
  return 0;
}
