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
  IntervalNode* nn  = new_node(e);
  root = insert(root, nn);
}

void IsoformScanner::walk_inorder(IntervalNode* root)
{
  if(root == 0)
  {
    return;
  }
  walk_inorder(root->left);
  std::cout << root << "\t" << root->locus.start << "\t" << root->locus.end
        << "\t" << root->max << "\t" << root->left << "\t" << root->right << "\n";
  walk_inorder(root->right);
}

void IsoformScanner::show_tree()
{
  std::cout << "#node\tstart\tend\tmax\tleft\tright\n";
  walk_inorder(lastnode);
}

int main(int argc, char **argv)
{
  IsoformScanner isc;
  gtf::Parser p;
  p.parse(isc);
  isc.show_tree();
  return 0;
}

