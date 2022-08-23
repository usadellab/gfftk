/**
 * @file isoformscanner.cpp
 * @author jpb (jpb@members.fsf.org)
 * @brief
 * @version 0.1
 * @date 2022-08-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "isoformscanner.h"

/**
 * @brief Construct a new Isoform Scanner:: Isoform Scanner object
 *
 * @param gffsource
 * @param taxid
 */
IsoformScanner::IsoformScanner(std::string gffsource, int taxid)
  : gffsource(gffsource), taxid(taxid)
{

}
/**
 * @brief Destroy the Isoform Scanner:: Isoform Scanner object
 *
 */
IsoformScanner::~IsoformScanner()
{
  /* for(auto i : nodes)
  {
    delete i;
  } */
}

/* IntervalNode* IsoformScanner::insert(IntervalNode* root, IntervalNode* ival)
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
} */

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

/* void IsoformScanner::walk_inorder(IntervalNode* root)
{
  if(root == 0){ return; }
  walk_inorder(root->left);
  std::cout << root << "\t" << root->start << "\t" << root->end
            << "\t" << root->max << "\t" << root->left << "\t" << root->right
            << "\t" << root->entries[0].feature() <<  "\n";
  walk_inorder(root->right);
} */

/* void IsoformScanner::show_tree()
{
  std::cout << "#node\tstart\tend\tmax\tleft\tright\tfeature\n";
  walk_inorder(root);
} */

int IsoformScanner::process_entry(gff::GffEntry e, std::unordered_map<std::string, std::vector<std::string>>& header)
{
  // std::cout << e.id() <<  "\n";
  // for(auto& i : header)
  // {
  //   std::cout << "\t" << i.first << "\n";
  // }
  // IntervalNode* ival = new IntervalNode(e);
  // root = insert(root, ival);
  //entrydb.add_entry(e);
  // nodes.push_back(ival);
  // features.insert({e.id(), e});
  // assemble_locus(e, header);
  // don#t forget last entry
}

/* Collect all features/entries which are part of a locus. A locus is a feature
without a parent feature. The search for a locus is initiated by the parent of
the currently examined entry to save one iteration.
*/
/* void IsoformScanner::assemble_locus(gff::GffEntry e, std::unordered_map<std::string, std::vector<std::string>>& header)
{
  if(!e.hasParent())  //  new locus
  {
    if(!prevloc.empty())
    {
      std::cerr << "Assessing\n";
      gff::Locus loc = loci.at(prevloc);
      // loc.show();
      gff::Locus::Feature* lf = loci.at(prevloc).find_longest_feature("CDS");
      if(lf)
      {
        std::cerr << "Found longest " << lf->type << " on " << loc.id() << ": "
                  << lf->id   << "\t" << lf->isSelected <<"\n";
        // show_feature(lf, loc, header);
      }
      list_locus_features(loci.at(prevloc), header);
    }
    gff::Locus locus = gff::Locus(e);
    loci.insert({e.id(), locus});
    std::cerr << "== New Locus: " << locus.id() << "\n";
    prevloc = locus.id();
  }
  else  //  part-of relation
  {
    //  Test if parnet feature is known to isoformscanner
    if(!hasFeature(e.parent())) {std::exit(EXIT_FAILURE);}
    gff::GffEntry p = get_feature(e.parent()); // dangerous, fix later
    p.add_child(e);
    e.add_parent(p);
    const std::string lid = get_locus_id(p);
    if(loci.contains(lid)) {loci.at(lid).add_entry(e);}
    else {std::cerr << "Error: locus " << lid << "not known\n";}
  }
} */

/* const std::string IsoformScanner::get_locus_id(gff::GffEntry e)
{
  while(e.hasParent())
  {
    //  Test if parent feature is known to isoformscanner
    if(!hasFeature(e.parent()))
    {
      std::cerr << "Error for entry " << e.id() << ": Missing parent " << e.parent() << ". Abort.";
      std::exit(EXIT_FAILURE);
    }
    e = get_feature(e.parent());
  }
  return e.id();
} */
/* bool IsoformScanner::hasFeature(const std::string& feature_name)
{
  if(features.contains(feature_name)){return true;}
  std::cerr << "Error: " << feature_name << " not in storage\n";
  return false;
} */

/* gff::GffEntry IsoformScanner::get_feature(const std::string& feature_name)
{return features.at(feature_name);} */

/* void IsoformScanner::show_loci()
{
  std::cout << "Listing loci\n";
  for(auto& i: loci){ i.second.show(); }
} */

/* void IsoformScanner::show_feature(gff::Locus::Feature* f, gff::Locus loc,
  std::unordered_map<std::string, std::vector<std::string>>& header)
{
    f->show();
    std::cout << taxid << "\t" << gffsource << "\t" <<loc.id()       << "\t"
              << f->id << "\t" << f->parent_id << "\t" << f->type   << "\t" << f->sequence() << "\t"
              << f->start << "\t" << f->end << "\t" << f->length() << "\t" << f-> isSelected;
    // std::vector<std::string> req_comments = {"protein_id", "locus_tag"};
    for(auto& i : req_comments)
    {
      if(f->hasComment(i))
      {
        for(auto& i : f->get_comment(i)){ std::cout << "\t" << i; }
      }
      else{ std::cout << "\t#missing:" << i; }
    }
    for(auto& i : directives)
    {
      if(header.contains(i)){ std::cout << "\t" << header[i].front(); }
    }
    std::cout << "\n";
} */

/* void IsoformScanner::list_locus_features(gff::Locus& loc, std::unordered_map<std::string, std::vector<std::string>>& header) const
{
  const std::unordered_map<std::string, std::unordered_map<std::string, gff::Locus::Feature>>& fm = loc.featuremap();
  for(const auto& i : fm.at("CDS"))
  {
    // for(const auto& j : i.second)
    // {
      std::cout << taxid << "\t" << gffsource << "\t" << loc.id() << "\t"
                << i.second.id << "\t" << i.second.parent_id << "\t" << i.second.type  << "\t" << i.second.sequence() << "\t"
                << i.second.start << "\t" << i.second.end << "\t" << i.second.length() << "\t" << i.second.isSelected;
      for(auto& k : directives)
      {
        if(header.contains(k)){ std::cout << "\t" << header[k].front(); }
      }
      gff::Locus::Feature feat = i.second;
      for(auto& k : req_comments)
      {
        if(feat.hasComment(k))
        {
          for(auto& i : feat.get_comment(k)){ std::cout << "\t" << i; }
        }
        else{ std::cout << "\t#missing:" << k; }
      }
    // }
    std::cout << "\n";
  }
} */

void usage()
{
  std::cout <<  "--input, -i <path>           Path to GFF file\n" <<
                "--taxid, -t <NCBI taxid>:    NCBI taxid\n"       <<
                "--help, -h:                  Show help\n";
  exit(1);
}

int main(int argc, char **argv)
{
  const char* const short_opts = "i:t:h";
  const option long_opts[] =
  {
            {"input", required_argument, nullptr, 'i'},
            {"taxid", required_argument, nullptr, 't'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
  };
  std::string gff_file;
  int taxid = -1;
  while (true)
  {
    const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

    if (-1 == opt)
      break;

    switch (opt)
    {
      case 'i':
        gff_file = optarg;
        break;

      case 't':
        taxid = atol(optarg);
        break;

      case 'h': // -h or --help
        usage();
        return EXIT_SUCCESS;

      case '?': // Unrecognized option
        return EXIT_FAILURE;
      default:
        usage();
        break;
    }
  }

  if(gff_file.empty())
  {
    std::cerr << "Error: Path to file is required\n";
    usage();
    return EXIT_FAILURE;
  }

  if(taxid < 0)
  {
    std::cerr << "Error: NCBI taxid required\n";
    usage();
    return EXIT_FAILURE;
  }
  gff::GffFile gff(gff_file);
  IsoformScanner isc(gff_file, taxid);
  gff.parse(isc);
  std::cout << "#Done\n";
  // isc.show_tree();
  // isc.show_loci();
  return 0;
}
