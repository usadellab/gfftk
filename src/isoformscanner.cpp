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

IntervalNode* IsoformScanner::new_node(gtf::GtfEntry& e)
{
  IntervalNode* node = new IntervalNode;
  node->locus = e;
  node->max = e.end;
  node->left = 0;
  node->right = 0;
  return node;
}

IntervalNode* IsoformScanner::insert(IntervalNode* root, gtf::GtfEntry& e)
{
  if(root == 0)
  {
    return new_node(e);
  }
  if(e.start < root->locus.start)
  {
    root->left = insert(root->left, e);
  }
  else
  {
    root->right = insert(root->right, e);
  }
  if(root->max < e.start)
  {
    root->max = e.start;
  }
  return root;
}
void IsoformScanner::process_entry(struct gtf::GtfEntry& e)
{

}



int main(int argc, char **argv)
{
  gtf::Parser p;
  p.parse();
  return 0;
}

