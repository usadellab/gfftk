/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 *  head -n 18 genomic.gtf
 */

#include <iostream>
#include "reader.h"
#include "isoformscanner.h"

IsoformScanner::IsoformScanner()
  { }
IsoformScanner::~IsoformScanner()
  { }

IntervalNode* IsoformScanner::new_node(gtf::GtfEntry& e)
{
  IntervalNode* node = new IntervalNode;
  node->locus = e;
  node->max = e.end;
  node->left = 0;
  node->right = 0;
  return node;
}

IntervalNode* IsoformScanner::insert(IntervalNode* root, IntervalNode* ival)
{
  if(root == 0)
  {
    return ival;
  }
//  std::cout << "ins\nroot: " << root->locus.start << "\t" << root->locus.comment
//            << "\nival:" << ival->locus.start << "\t" << ival->locus.comment << "\n";
  if(ival->locus.start < root->locus.start)
  {
    root->left = insert(root->left, ival);
  }
  else
  {
    root->right = insert(root->right, ival);
  }
  if(root->max < ival->locus.start)
  {
    root->max = ival->locus.start;
  }
  lastnode = root;
  return root;
}
void IsoformScanner::process_entry(struct gtf::GtfEntry& e)
{
//  std::cout << e.seqname << "\t" << e.source << "\t" << e.start << "\t" << e.end << "\n";
  IntervalNode* nn  = new_node(e);
//  std::cout << root <<"\n";
//  std::cout << "Before:" <<  nn->locus.start << "\n";
  root = insert(root, nn);
//  std::cout << "After\nroot:\t" << root->locus.start << "\t" << root->locus.comment
//            << "\nnn:\t" << nn->locus.start << "\t" << nn->locus.comment << "\n";
}

void IsoformScanner::walk_inorder(IntervalNode* root)
{
//  std::cout << root->max << "9999999999999999999999999999999999999\n";
  if(root == 0)
  {
    return;
  }
  walk_inorder(root->left);
  std::cout << "[" << root->locus.start << ", " << root->locus.end << "]"
        << " max = "<< root->max << "\n";
}

int main(int argc, char **argv)
{
  IsoformScanner isc;
  gtf::Parser p;
  p.parse(isc);
//  for(auto i:isc.nodes)
//  {
//    std::cout << i << "\n";
//  }
  std::cout << "sasasasasas\n";
  isc.walk_inorder(isc.lastnode);
  return 0;
}

