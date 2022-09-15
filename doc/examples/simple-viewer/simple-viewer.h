/*
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright 2022"
 */

#pragma once

#include <iostream>

#include "gfffile.h"

class SimpleViewer : public gff::GffFile::Processor
{
  public:
    SimpleViewer();
    ~SimpleViewer();
    int process_locus(gff::TypeFeature* locus);
    void view(const gff::Feature* feat, int level = 0);
};
