# README

## Overview

`gfftk` is a toolkit to examine GFF files and extract specific entries. The core
functions to parse GFF file can be used in own projects. A simple example is
given in the [`examples`](doc/examples) directory.

## Currently ongoing

- Implementing the extraction of specific entries directly from the source FASTA file
- Code documentation

## Requirements

- GCC >= 9.0.0

## Build

```bash
$: git clone https://github.com/usadellab/gfftk.git
$: cd gfftk && mkdir build && cd $_ && ln -s ../config/Makefile
$: make gfftk
```

This should compile a `gfftk` executable.

## Examples

```bash
$: ./gfftk
$: ./gfftk extract -h
# Extract longest CDS and the corresponding prtotein_id attribute
$: ./gfftk extract -i GCF_000188115.4.ncbi.gff -t cds -l -a protein_id
```
