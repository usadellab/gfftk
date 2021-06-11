/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 *  head -n 18 genomic.gtf
 */

#include <iostream>
#include <vector>

#include "gffentry.h"
#include "isoformscanner.h"
#include "locus.h"
#include "overlap.h"
#include "reader.h"

IsoformScanner::IsoformScanner()
  { }

IsoformScanner::~IsoformScanner()
{

  for(auto i : nodes)
  {
    delete i;
  }
}

IntervalNode* IsoformScanner::insert(IntervalNode* root, IntervalNode* ival)
{
  if(root == 0)
  {
    return ival;
  }
  if(ival->start <= root->start)
  {
    root->left = insert(root->left, ival);
  }
  else
  {
    root->right = insert(root->right, ival);
  }
  if(root->max < ival->end)
  {
    root->max = ival->end;
  }
  return root;
}

/* Overlap& IsoformScanner::overlap(const IntervalNode* subject, const IntervalNode* query)
{
  overlap = Overlap()
  if(subject == 0)
  {
    return overlap;
  }

  if(subject->locus.start <= root->locus.start)
  {
    root->left = insert(root->left, ival);
  }
  else
  {
    root->right = insert(root->right, ival);
  }
  if(root->max < ival->locus.end)
  {
    root->max = ival->locus.end;
  }
  return root;
} */

void IsoformScanner::walk_inorder(IntervalNode* root)
{
  if(root == 0)
  {
    return;
  }
  walk_inorder(root->left);
  std::cout << root << "\t" << root->start << "\t" << root->end <<
               "\t" << root->max << "\t" << root->left << "\t" << root->right <<
               "\t" << root->entries[0].feature <<  "\n";
  // root->show_entries();
  walk_inorder(root->right);
}

void IsoformScanner::show_tree()
{
  std::cout << "#node\tstart\tend\tmax\tleft\tright\tfeature\n";
  // std::cout << lastnode->start << "\n";
  walk_inorder(root);
}

void IsoformScanner::process_entry(gff::GffEntry e)
{
  IntervalNode* ival = new IntervalNode(e);
  // std::cout << ival->start << "\n";
  root = insert(root, ival);
  nodes.push_back(ival); // dirty way to track IntervalNodes
  assemble_loci(e);
}

void IsoformScanner::assemble_loci(gff::GffEntry e)
{
  // std::cout << e.id() << "   " << e.parent() << "     " << e.hasParent() <<  "\n";
  if( (e.hasParent()) && (loci.contains(e.id())) ) //part-of relation
  {

    // loci[e.id()] = Locus {e.id()};
    std::cout << e.id() << "   " <<e.parent() << "\n";
  }
  else  // new locus
  {
    if(!loci.contains(e.id()))
    {
      gff::Locus locus = gff::Locus(e);
      loci.insert({e.id(), locus});
    }
  }
}

void IsoformScanner::show_loci()
{
  for(auto& i: loci)
  {
    std::cout << "Locus: " << i.first << "\n";
    i.second.show();
  }

}

int main(int argc, char **argv)
{
  IsoformScanner isc;
  gff::Parser p;
  p.parse(isc);
  isc.show_tree();
  isc.show_loci();
  return 0;
}
