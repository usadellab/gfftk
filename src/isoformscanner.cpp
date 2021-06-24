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
  // std::cout << "Inserting: " << e.id() << "\n";
  features.insert({e.id(), e});
  nodes.push_back(ival);
  assemble_locus(e);
}

void IsoformScanner::assemble_locus(gff::GffEntry& e)
{
  // std::cout << e.id() << "   " << e.parent() << "     " << e.hasParent() <<  "\n";
  if(!e.hasParent())// new locus
  {
    gff::Locus locus = gff::Locus(e);
    loci.insert({e.id(), locus});
    std::cout << "New locus: " << locus.id() << "\n";
  }
  else //part-of relation
  {
    gff::GffEntry& p = get_feature(e.parent());
    std::cout << "\tExtending locus with: " << e.id() << " : Part-of: " <<p.id() << "\n";
    p.add_child(e);
    e.add_parent(p);
    std::cout << "\tGetting locus for entry: " << e.id() << "\n\tparent: " << p.id() << ": "<< &p <<"\n";
    get_locus(p.id());

    // gff::Locus& l = loci.at(e.id());
    // std::cout << "Feature: " << e.id() << ":: parent:" << get_feature(e.parent()).id() << "\n";

    //
    // l.add_feature(e);
  }
}

void IsoformScanner::get_locus(const std::string& nodeid)
{
  gff::GffEntry& node = get_feature(nodeid);
  std::cout << "\t\t\tstart: " << node.id() << " address "<< &node <<"\n";

  while(node.hasParent())
  {
    std::cout << "\t\t\tprenode: " << node.id() << "\tparent: " << node.parent() << "\n";
    node = get_feature(node.parent());
    std::cout << "\t\t\tpostnode: " << node.id() << "\n";
  }
  std::cout << "\t\t\tlocus: " << node.id() <<"\n";
}

gff::GffEntry& IsoformScanner::get_feature(const std::string& feature_name)
{
  if(features.contains(feature_name))
  {
    return features.at(feature_name);
  }
  else
  {
    std::cout << "Error: " << feature_name << " not in storage\n";
  }
}

void IsoformScanner::show_loci()
{
  std::cout << "Listing loci\n";
  for(auto& i: loci)
  {
    std::cout << "Locus: " << i.first << "\n";
    // i.second.show();
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
