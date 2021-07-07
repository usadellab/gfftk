/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "gffentry.h"

namespace gff
{

class EntryDb
{
  public:
    EntryDb();
    ~EntryDb();
    void add_entry(gff::GffEntry e);
    gff::GffEntry* get_entry(const std::string& id);

  private:
    std::unordered_map<std::string, gff::GffEntry> entries;
};

} // namespace gff



