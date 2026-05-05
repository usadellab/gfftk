# README

## Overview

`gfftk` is a toolkit to examine GFF files and extract specific entries. The core
functions to parse GFF file can be used in own projects. A simple example is
given in the [`examples`](doc/examples) directory.

## Requirements

- GCC >= 9.0.0

## Build

```bash
$: git clone https://github.com/usadellab/gfftk.git
$: cd gfftk && mkdir build && cd $_ && ln -s ../config/Makefile
$: make gfftk
```

This should compile a `gfftk` executable in the directory build. This executable
can be moved into any directory.

Current functions:

- Identify and extract isoforms. Requires a `GFF` file and the corresponding
  `FASTA` file.

## Usage

`gfftk` has the follwoing available subcommands so far:

``` bash
$: ./bin/gfftk
Expecting a command
usage: gfftk <command> [args]

gfftk is a toolkit to work with GFF files
Available commands:
        version         show version
        extract         Extract features from GFF file
        isoform         Extract isoforms from GFF file
```

### extract

```bash
Extracting features from GFF, e.g. CDS features

usage: gfftk extract --input <GFF> --type [mRNA, CDS,..] [OPTIONAL]

Mandatory:
        --input, -i <path>         Path to GFF file
        --type, -t  <type>         GFF type, e.g. mRNA
Optional:
        --longest, -l              longest type
        --shortest, -s             shortest type
        --min, -m <int>            min lentgth
        --max, -M <int>            max lentgth
        --attribute, -a <str>      select attribute
        --fasta, -f <path>         FASTA file with sequences
        --help, -h                 Show help
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

### Identifying and saving isoforms based on the longest CDS sequence

```bash
$: gfftk isoform  -i ../data/GCF_000001735.4_TAIR10.1_genomic.gff -t CDS -l -o TAIR10.longest.cds.fa
```

### Identifying and saving isoforms based on the shortest CDS sequence

```bash
$: gfftk isoform  -i ../data/GCF_000001735.4_TAIR10.1_genomic.gff -t CDS -s -o TAIR10.shortest.cds.fa
```

## Testing

### Calculate length of elements between lines (including) 212044 and 212053

```bash
$: sed -n '212044,212053p' ../data/GCF_000188115.5.gff | awk -F'\t' -v OFS="\t" '{len+=($5-$4)+1}END{print len}'
```
