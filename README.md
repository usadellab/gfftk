# README

## Overview

- Extract the longest transcripts for genes from a GFF file
- Summarize a GFF file

`gfftk` is a toolkit designed to work with [GFF
files](https://github.com/The-Sequence-Ontology/Specifications/blob/master/gff3.md),
a file format to store structural information about genomic features. Genes in
GFF files a stored as a hierarchy of components, e.g., a gene may consist of one
or more transcripts, each of which is made up of exons and other element types.
`gfftk` can read and resolve these hierarchical relationships.

`gfftk` identifies transcript isoforms by calculating the total length of a
selected element type (e.g., summing the lengths of all exons belonging to each
isoform) and then selecting the longest or shortest sequence as the isoform
representing that gene. The element type used for the calculation and whether to
select the longest or shortest isoform can be selected by the user. Any element
type present in the GFF can be selected.

## Build

- Latest precompiled binaries: <https://github.com/usadellab/gfftk/releases/latest>

- Requires Linux:
  - `GCC` >= `11.4.0`
  - `GLIBC` >= `2.34`
  - CPU architecture: `x86_64`

### Compiling

```bash
$: git clone https://github.com/usadellab/gfftk.git
$: cd gfftk && mkdir work && cd $_
$: make -f ../config/Makefile
```

This should compile a `gfftk` executable (`build/bin/gfftk` in your current
working directory). The executable can be moved into any directory. Move the
executable before running `make clean` as it will remove the binary and
intermediary files.

## Usage

`gfftk` has the following available sub commands:

``` bash
$: ./gfftk
Expecting a command
usage: gfftk <command> [args]

gfftk is a toolkit to work with GFF files
Available commands:
        version         show version
        isoform         Extract isoforms from GFF file
        summarize       Summarize GFF file
```

## Commands

### `summarize`

The command takes a GFF file and outputs a TSV table counting feature types
(genes, exons, CDS, mRNA, etc.) per sequence, plus the descriptive statistics
totals, averages, and medians.

```bash
Summarize GFF and print to STDOUT

usage: gfftk summarize --input <GFF>

Mandatory:
        --input, -i <path>  Path to GFF file
Optional:
        --help,  -h         Show this help
```

- [`summarize` examples](#summarize-a-gff-file)

### `isoform`

Many genes can produce multiple slightly different protein variants, called
isoforms, from the same gene. This command lets you pick just one representative
variant per gene from the structural GFF annotation file.

You provide a `GFF` file and the corresponding `FASTA` file, and specify which
type level to compare (e.g., CDS for protein-coding regions, or exon for the
broader transcribed regions). You then choose whether to keep the longest or
shortest variant per gene, and the results are written to a new `FASTA` file.

```bash
Extracting isoforms from GFF

usage: gfftk isoforms --input <GFF> --type [mRNA, CDS, exon, ..] --out <PATH>[OPTIONAL]

Mandatory:
        --input, -i <path>    Path to GFF file
        --type,  -t <type>    Level on which isoforms should be selected, e.g. CDS. Default: CDS
        --fasta, -f <path>    FASTA file from which to extract isoforms
        --out,   -o <path>      FASTA file to which write isoforms
Optional:
        --longest, -l         Longest type (Default)
        --shortest, -s        Shortest type
        --help, -h            Show this help
```

- [`isoform` examples](#identifying-and-saving-isoforms)

## Examples

The directory `example` contains a very short extract from the GFF file from the
[TAIR10 *Arabidopsis thaliana* release](https://www.arabidopsis.org/download/file?path=Genes/TAIR10_genome_release/TAIR10_gff3/TAIR10_GFF3_genes.gff).

### Summarize a GFF file

- The column names are the individual types present in the GFF file
- The column "roots" indicates the entries without a parent and are not part of
  the GFF file
- The column sequence indicates the sequence name

```bash
$: ./build/bin/gfftk summarize -i ../example/TAIR10.demo.gff
[ GFF Summary ]
sequence	roots	cds	chromosome	exon	five_prime_utr	gene	mirna	mrna	ncrna	protein	three_prime_utr	trna
Chr1	218	658	1	728	150	93	2	124	3	124	115	1
total	218	658	1	728	150	93	2	124	3	124	115	1
average	218.00	658.00	1.00	728.00	150.00	93.00	2.00	124.00	3.00	124.00	115.00	1.00
median	218.00	658.00	1.00	728.00	150.00	93.00	2.00	124.00	3.00	124.00	115.00	1.00
Finished summary
```

The output table is printed to the standard output a TSV. It can be redirected
into an output file or pipes.

To get a pretty-print output you can pipe the output into `column`, a Linux tool
which is often available in most Linux distributions. You can download the full
GFF file and genomic sequence from *Arabidopsis thaliana* via FTP and run the
commands with these files.

- Download the full GFF3 file: <https://www.arabidopsis.org/download/file?path=Genes/TAIR10_genome_release/TAIR10_gff3/TAIR10_GFF3_genes.gff>

```bash
$: ./build/bin/gfftk summarize -i TAIR10_GFF3_genes.gff | column -t  -s $'\t' | less -S
[ GFF Summary ]
Finished summary
sequence  roots     cds       chromosome  exon      five_prime_utr  gene     mirna  mrna     mrna_te_gene  ncrna  protein  pseudogene  pseudogenic_exon  pseudogenic_transcript  rrna  snorna  snrna  three_prime_utr  transposable_element_gene  trna
Chr1      17697     53570     1           58048     9243            7509     54     9263     684           144    9263     241         339               241                     0     18      2      8169             683                        240
Chr2      11074     29925     1           33233     5592            4470     29     5560     830           105    5560     217         283               218                     2     15      0      4895             826                        96
Chr3      13639     37578     1           41556     6794            5650     30     6908     880           82     6908     202         266               202                     2     15      7      5916             878                        93
Chr4      10497     30227     1           33215     5189            4308     30     5356     711           75     5356     121         177               122                     0     11      0      4682             711                        79
Chr5      15597     45616     1           49536     7799            6559     37     8089     806           74     8089     143         209               143                     0     12      4      6972             805                        123
ChrC      222       104       1           157       0               133      0      88       0             0      88       0           0                 0                       8     0       0      0                0                          37
ChrM      269       140       1           164       4               146      0      122      0             0      122      0           0                 0                       3     0       0      0                0                          21
total     68995     197160    7           215909    34621           28775    180    35386    3911          480    35386    924         1274              926                     15    71      13     30634            3903                       689
average   9856.43   28165.71  1.00        30844.14  4945.86         4110.71  25.71  5055.14  558.71        68.57  5055.14  132.00      182.00            132.29                  2.14  10.14   1.86   4376.29          557.57                     98.43
median    11074.00  30227.00  1.00        33233.00  5592.00         4470.00  30.00  5560.00  711.00        75.00  5560.00  143.00      209.00            143.00                  2.00  12.00   0.00   4895.00          711.00                     93.00
```

### Identifying and saving isoforms

- Download and decompress the TAIR10 assembly: <https://www.arabidopsis.org/download/file?path=Genes/TAIR10_genome_release/TAIR10_chromosome_files/TAIR10_chr_all.fas.gz>

#### Identify isoforms based on the longest/shortest CDS sequence for an entry

- This will use the longest (argument: `-l`) CDS sequence (argument: `-t CDS`)
  for each entry as its representative isoform
- All isoforms will be saved to `TAIR10.longest.cds.fa` (argument: `-o TAIR10.longest.cds.fa`)

```bash
$: ./build/bin/gfftk isoform -i ../example/TAIR10.demo.gff -f ../example/TAIR10.demo.fa -l -t CDS  -o TAIR10.longest.cds.fa
```

- This will use the shortest (argument: `-s`) CDS sequence (argument: `-t CDS`)
  for each entry as its representative isoform
- All isoforms will be saved to `TAIR10.shortest.cds.fa` (argument: `-o TAIR10.shortest.cds.fa`)

```bash
$: ./build/bin/gfftk isoform -i ../example/TAIR10.demo.gff -f ../example/TAIR10.demo.fa -s -t CDS  -o TAIR10.shortest.cds.fa
```

#### Identifying and saving isoforms based on the longest exon sequence

- This will use the longest (argument: `-l`) exon (argument: `-t exon`) for each
  entry as its representative isoform
- All isoforms will be saved to `example.longest.cds.fa` (argument: `-o TAIR10.longest.exon.fa`)

```bash
$: ./build/bin/gfftk isoform ../example/TAIR10.demo.gff -f ../example/TAIR10.demo.fa -l -t exon  -o TAIR10.longest.exon.fa
```
