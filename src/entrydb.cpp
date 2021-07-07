/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */
#include "entrydb.h"

#include <iostream>
#include <vector>

#include "gffentry.h"


namespace gff
{

EntryDb::EntryDb()
  { }

EntryDb::~EntryDb()
  { }

void EntryDb::add_entry(gff::GffEntry e)
{
  if(e.hasId())
  {
    const auto& [it, pass] = entries.try_emplace(e.id(), e);
    if(!pass)
    {
      std::cout << "Error: duplicate entry id " << e.id() << "\n";
    }
  }
}

} // namespace gff

