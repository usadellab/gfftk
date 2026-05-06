/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * copyright BDS-HHU 2026
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "gff/gffentry.h"
#include "gff/gffindex.h"
#include "summaries/summaries.h"
#include "utils/stringtools.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace gff
{

std::istream& operator>>(std::istream& is, GffEntry& e);
void parse_attributes(const std::string& attribs, gff::GffEntry& entry);
void make_synthetic_id(gff::GffEntry&);

class GffException : public std::runtime_error
{
  public:
    explicit GffException(const std::string& msg)
      : std::runtime_error(msg)
    {
    }
};

class GffFileNotFound : public GffException
{
  public:
    explicit GffFileNotFound(const std::string& path)
      : GffException("File not found: " + path)
    {
    }
};

class GffOpenError : public GffException
{
  public:
    explicit GffOpenError(const std::string& path)
      : GffException("Failed to open: " + path + " (" + std::strerror(errno)
                     + ")")
    {
    }
};

struct GffEntryDelimiter : std::ctype<char>
{
    GffEntryDelimiter();
    static mask const* get_table();
};

class GffFile
{
  public:
    explicit GffFile(std::string gff_file);
    GffFile(const GffFile&) = delete;
    GffFile& operator=(const GffFile&) = delete;
    GffFile(GffFile&&) = default;
    GffFile& operator=(GffFile&&) = default;
    ~GffFile();
    void close();
    std::filesystem::path path() const;
    void parse();
    void find_by_id(const std::string&);
    std::vector<GffEntry*> find_type_for_id(const std::string&,
                                            const std::string&);
    void find_direct_subtypes_for_id(const std::string&); // only depth 1
    std::vector<gff::GffEntry*> find_all_subtypes_for_id(const std::string& id,
                                                         const std::string& type
                                                         = "");
    void find_root_for_id(const std::string&);
    void show_gffentry(const GffEntry& e);
    std::vector<gff::GffEntry*> find_all_of_type(const std::string&);
    std::vector<gff::GffEntry*> find_all_parents();
    std::vector<gff::GffSelectedEntry>& find_longest_type(
      const std::string& type);
    std::vector<gff::GffSelectedEntry>& find_shortest_type(
      const std::string& type);
    gff::GffSummary summarize() const;
    static std::unordered_map<std::string, int> synth_entries_count;
    GffIndex index;

  private:
    std::string inpath;
    std::locale def_locale;
    std::vector<GffEntry> entries;
    std::ifstream gff_in;
    unsigned int row_num = 0;
    void open();
    void clean_up();
};

} // namespace gff
