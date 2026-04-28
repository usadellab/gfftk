/*
 * -------------------------------------------------------------------------------
 * \author Jan Piotr Buchmann <jpb@members.fsf.org>
 * \copyright 2021
 * -------------------------------------------------------------------------------
 */

#pragma once

// #include "components/gffrow.h"
// #include "components/typefeature.h"
// #include "gfftypes.h"
#include "gff/gffentry.h"
#include "utils/stringtools.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace gff
{
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
    int parse();

  private:
    std::string inpath;
    std::ifstream gff_in;
    unsigned int row_num = 0;
    void open();
    void clean_up();
};
std::istream& operator>>(std::istream& is, GffEntry& e);
void parse_attributes(const std::string& attribs, gff::GffEntry& entry);
} // namespace gff
