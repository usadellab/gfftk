/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 *  head -n 18 genomic.gtf
 */
#include "isoformscanner.h"

#include <iostream>
#include <vector>

#include "gffentry.h"
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
  walk_inorder(root->right);
}

void IsoformScanner::show_tree()
{
  std::cout << "#node\tstart\tend\tmax\tleft\tright\tfeature\n";
  walk_inorder(root);
}

void IsoformScanner::process_entry(gff::GffEntry e)
{
  IntervalNode* ival = new IntervalNode(e);
  root = insert(root, ival);
  features.insert({e.id(), e});
  nodes.push_back(ival);
  assemble_locus(e);
}

/* Collect all features/entries which are part of a locus. A locus is a feature
without a parent feature. The search for a locus is initiated by the parent of
currently examined entry to save one step.
*/
void IsoformScanner::assemble_locus(gff::GffEntry e)
{
  if(!e.hasParent())  // new locus
  {
    gff::Locus locus = gff::Locus(e);
    loci.insert({e.id(), locus});
    // std::cout << "New locus: " << locus.id() << "\n";
  }
  else  //part-of relation
  {
    gff::GffEntry p = get_feature(e.parent()); // dangerous, fix later
    // std::cout << "\tExtending locus with: " << e.id() << " : Part-of: " <<p.id() << "\n";
    p.add_child(e);
    e.add_parent(p);
    // std::cout << "\tGetting locus for entry: " << e.id() << "\n\tparent: " << p.id() << ": "<< &p <<"\n";
    const std::string& lid = get_locus_id(p);
    std::cout << "Locus : " << lid << "\n";
    if(loci.contains(lid))
    {
      gff::Locus& loc = loci.at(lid);
      std::cout << "Locus inst " << loc.id() << "  " << &loc <<"\n";
      std::cout << "Locus for : " << e.id() << " = " << loc.id() << "  " << &loc <<"\n";
      loc.features.push_back(e);
    }
    else
    {
      std::cout << "Error: locus " << lid << "not known\n";
    }
  }
}

const std::string& IsoformScanner::get_locus_id(gff::GffEntry& e)
{
  // std::cout << "\t\t\tstart: " << e.id() << " address "<< &e <<"\n";
  while(e.hasParent())
  {
    // std::cout << "\t\t\tprenode: " << e.id() << "\tparent: " << e.parent() << "\n";
    e = get_feature(e.parent());
    // std::cout << "\t\t\tpostnode: " << e.id() << "\n";
  }
  // std::cout << "\t\t\tlocus: " << e.id() <<"\n";
  return e.id();
}

gff::GffEntry IsoformScanner::get_feature(const std::string& feature_name)
{
  if(features.contains(feature_name)) {return features.at(feature_name);}
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
    std::cout << "Locus: " << i.first << &i << "\n";
    i.second.show();
  }
}

int main(int argc, char **argv)
{
  IsoformScanner isc;
  gff::Parser p;
  p.parse(isc);
  // isc.show_tree();
  isc.show_loci();
  return 0;
}
