# README

## Overview

`gfftk` is a toolkit to examine GFF files and extract specific entries. The core
functions to parse GFF file can be used in own projects. A simple example is
given in the [`examples`](doc/examples) directory.

## Requirements

- GCC >= 15.2.1

## Build

```bash
$: git clone https://github.com/usadellab/gfftk.git
$: cd gfftk && mkdir work && cd $_
$: make -f ../config/Makefile
```

This should compile a `gfftk` executable (`build/bin/gfftk`). The executable can
be moved into any directory. Move the executable before running `make clean` as
it will remove all binaries and intermediary files.

## Current functions

- Identify and extract isoforms. Requires a `GFF` file and the corresponding
  `FASTA` file.

- Summarize a GFF file: how many types in total, per sequence, etc.

## Usage

`gfftk` has the following available sub commands:

``` bash
$: ./bin/gfftk
Expecting a command
usage: gfftk <command> [args]

gfftk is a toolkit to work with GFF files
Available commands:
        version         show version
        isoform         Extract isoforms from GFF file
        summarize               Summarize GFF file
```

### isoform

```bash
Extracting isoforms from GFF

usage: gfftk isoforms --input <GFF> --type [mRNA, CDS, exon, ..] [OPTIONAL]

Mandatory:
        --input, -i <path>    Path to GFF file
        --type,  -t <type>    Level on which isoforms should be selected, e.g. CDS. Default: CDS
        --fasta, -f <path>    FASTA file from which to extract isoforms
Optional:
        --longest, -l         Longest type (Default)
        --shortest, -s        Shortest type
        --help, -h            Show this help
```

## Examples

### Summarize a GFF file

```bash
$: ./build/bin/gfftk summarize -i ../example/GCF_000188115.demo.gff
[ GFF Summary ]
  total entries   : 191
  total parents     : 13
  sequences       : 1
  avg roots/seq   : 13

  global features:
    region                count: 1         min: 98455869  max: 98455869  avg: 98455872.0bp
    lnc_rna               count: 1         min: 2523      max: 2523      avg: 2523.0bp
    cds                   count: 18        min: 15        max: 1197      avg: 390.7bp
    mrna                  count: 19        min: 1022      max: 13216     avg: 4338.2bp
    gene                  count: 12        min: 1022      max: 13216     avg: 4002.1bp
    exon                  count: 140       min: 41        max: 1649      avg: 288.0bp

  per sequence:
    NC_015438.3  entries: 191  parents: 13
      region                count: 1       avg: 98455872.0bp
      lnc_rna               count: 1       avg: 2523.0bp
      cds                   count: 18      avg: 390.7bp
      mrna                  count: 19      avg: 4338.2bp
      gene                  count: 12      avg: 4002.1bp
      exon                  count: 140     avg: 288.0bp
Finished summary
```

### Identifying and saving isoforms based on the longest CDS sequence

- This will write all isoforms into the file `example.longest.cds.fa`

```bash
$: ./build/bin/gfftk isoform  -i ../example/GCF_000188115.demo.gff -f ../example/NC_015438.3.demo.fa -t CDS -l -o example.longest.cds.fa
```

### Identifying and saving isoforms based on the shortest CDS sequence

- This will write all isoforms into the file `example.shortest.cds.fa`

```bash
$: ./build/bin/gfftk isoform  -i ../example/GCF_000188115.demo.gff -f ../example/NC_015438.3.demo.fa -s -t CDS  -o TAIR10.shortest.cds.fa
```

<!--
## Testing

### Calculate length of elements between lines (including) 212044 and 212053

```bash
$: sed -n '212044,212053p' ../data/GCF_000188115.5.gff | awk -F'\t' -v OFS="\t" '{len+=($5-$4)+1}END{print len}'
``` -->
