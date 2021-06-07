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
{
  for(const auto& [key, value] : loci)
  {
    for(const auto& i:value.nodes)
    {
      delete i;
    }
  }
}

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
  if(ival->locus.start <= root->locus.start)
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
  lastnode = root;
  return root;
}

bool IsoformScanner::isFeature(IntervalNode* node, std::string feature)
{
  return (node->locus.feature == feature) ? true : false;
}

void IsoformScanner::walk_inorder(IntervalNode* root)
{
  if(root == 0)
  {
    return;
  }
  walk_inorder(root->left);
  std::cout << root << "\t" << root->locus.start << "\t" << root->locus.end <<
               "\t" << root->max << "\t" << root->left << "\t" << root->right <<
               "\t" << root->locus.feature << "\n";
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
  IntervalNode* ival = new_node(e);
  root = insert(root, ival);
  if(isFeature(ival, "gene"))
  {
    loci[ival->locus.gene_id] = Locus {root->locus.gene_id};
  }
  if(loci.contains(ival->locus.gene_id))
  {
    loci[ival->locus.gene_id].nodes.push_back(ival);
  }
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
  //isc.show_tree();
  isc.show_loci();
  return 0;
}

