/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 *  head -n 18 genomic.gtf
 */

#include <iostream>
#include "reader.h"
#include "overlap.h"
#include "isoformscanner.h"

IsoformScanner::IsoformScanner()
  { }

IsoformScanner::~IsoformScanner()
{
  for(const auto& [key, value] : loci)
  {
    for(const auto& i:value.nodes)
    {
      delete i;
    }
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
               "\t" << root->max << "\t" << root->left << "\t" << root->right <<"\n";
              //  << "\t" << root->entries[0] << "\n";
  walk_inorder(root->right);
}

void IsoformScanner::show_tree()
{
  std::cout << "#node\tstart\tend\tmax\tleft\tright\tfeature\n";
  walk_inorder(lastnode);
}

void IsoformScanner::show_loci()
{
  for(auto& [key, value] : loci)
  {
    std::cout << key << ":" << value.nodes.size() << "\n";
    value.show();
  }
}

void IsoformScanner::process_entry(struct gtf::GtfEntry& e)
{
  IntervalNode* ival = new IntervalNode(e);
  root = insert(root, ival);
  if(e.feature == "gene")
  {
    loci[e.gene_id] = Locus {e.gene_id};
  }
  if(loci.contains(e.gene_id))
  {
    loci[e.gene_id].nodes.push_back(ival);
  }
  lastnode = root;
}

void Locus::show()
{
  for(const auto &i : nodes)
  {
    std::cout << i << "\n";
  }
}
int main(int argc, char **argv)
{
  IsoformScanner isc;
  gtf::Parser p;
  p.parse(isc);
  isc.show_tree();
  isc.show_loci();
  return 0;
}
