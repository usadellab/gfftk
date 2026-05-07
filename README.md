# README

## Overview

`gfftk` is a toolkit designed to work with [GFF
files](github.com/The-Sequence-Ontology/Specifications/blob/master/gff3.md), a
file format to store structural information about genomic features. Genes in GFF
files a stored as a hierarchy of components, e.g., a gene may consist of one or
more transcripts, each of which is made up of exons and other element types.
`gfftk` can read and resolve these hierarchical relationships.

`gfftk` identifies isoforms by calculating the total length of a selected
element type (e.g., summing the lengths of all exons belonging to each
isoform) and then selecting the longest or shortest sequence as the isoform
representing that gene.

The user can select which element type is used for this length calculation
(e.g., exon, cDNA, mRNA) and whether the longest or shortest isoform should be
selected. Any element type present in the GFF file can be used.

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
        summarize               Summarize GFF file
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
[TAIR10 *Arabidopsis thaliana* annotation at NCBI](https://www.ncbi.nlm.nih.gov/datasets/genome/GCF_000001735.4/).

### Summarize a GFF file

```bash
$: ./build/bin/gfftk summarize -i ../example/GCF_000001735.4_TAIR10.demo.gff
[ GFF Summary ]
sequence        roots   cds     exon    gene    lnc_rna mirna   mrna    primary_transcript      region
NC_003070.9     17      31      236     16      4       1       32      1       1
total   17      31      236     16      4       1       32      1       1
average 17.00   31.00   236.00  16.00   4.00    1.00    32.00   1.00    1.00
median  17.00   31.00   236.00  16.00   4.00    1.00    32.00   1.00    1.00
Finished summary
```

The output table is printed to the standard output a TSV. It can be redirected
into an output file or pipes.

To get a pretty-print output you can pipe the output into `column`, a Linux tool
which is often available in most Linux distributions. You can download the full
GFF file and genomic sequence from *Arabidopsis thaliana* via FTP and run the
commands with these files.

```bash
$: wget  https://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/001/735/GCF_000001735.4_TAIR10.1/GCF_000001735.4_TAIR10.1_genomic.gff.gz
$: gunzip -k https://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/001/735/GCF_000001735.4_TAIR10.1/GCF_000001735.4_TAIR10.1_genomic.gff.gz
$: ./build/bin/gfftk summarize -i GCF_000001735.4_TAIR10.1_genomic.gff | column -t  -s $'\t' | less -S
[ GFF Summary ]
Finished summary
sequence     roots    antisense_rna  cds      direct_repeat  exon      five_prime_utr  gene     inverted_repeat  lnc_rna  mirna  mrna     ncrna  primary_transcript  pseudogene  region  repeat_region  rrna  sequence_feature  snorna  snrna  three_prime_utr  transcript  trna
NC_003074.8  7644     25             9474     0              62765     0               6544     0                763      67     10353    56     50                  1080        1       0              2     19                47      19     0                384         92
NC_003070.9  9733     20             12653    0              87301     0               8771     0                1098     115    13336    86     89                  930         1       0              0     31                89      22     0                450         238
NC_003075.7  5855     14             7426     0              50105     0               5007     0                559      69     8137     49     55                  835         1       0              0     12                44      10     0                272         77
NC_000932.1  130      0              85       0              156       0               129      0                0        0      85       0      0                   0           1       0              7     0                 0       0      0                0           37
NC_003076.8  8458     12             10995    0              73623     0               7469     0                775      103    11802    40     77                  951         1       0              0     37                61      27     0                317         123
NC_003071.7  6325     21             7599     0              50533     0               5265     0                683      74     8431     55     55                  1047        1       0              2     12                46      4      0                403         95
NC_037304.1  1151     0              33       46             365       25              276      34               0        0      33       254    0                   8           1       29             3     707               0       0      23               0           22
total        39296    92             48265    46             324848    25              33461    34               3878     428    52177    540    326                 4851        7       29             14    818               287     82     23               1826        684
average      5613.71  13.14          6895.00  6.57           46406.86  3.57            4780.14  4.86             554.00   61.14  7453.86  77.14  46.57               693.00      1.00    4.14           2.00  116.86            41.00   11.71  3.29             260.86      97.71
median       6325.00  14.00          7599.00  0.00           50533.00  0.00            5265.00  0.00             683.00   69.00  8431.00  55.00  55.00               930.00      1.00    0.00           2.00  19.00             46.00   10.00  0.00             317.00      92.00
```

```bash
$: ./build/bin/gfftk summarize -i ../example/GCF_000188115.demo.gff | column -s $'\t' | less
```

### Identifying and saving isoforms

#### Identify isoforms based on the longest/shortest CDS sequence for an entry

- This will use the longest (argument `-l`) CDS sequence (argument: `-t CDS`)
  for each entry as its representative isoform
- All isoforms will be saved to `TAIR10.longest.cds.fa` (argument `-o TAIR10.longest.cds.fa`)

```bash
$: ./build/bin/gfftk isoform -i ../example/GCF_000001735.4_TAIR10.demo.gff -f ../example/GCF_000001735.4_TAIR10.demo.fa -l -t CDS  -o TAIR10.longest.cds.fa
```

- This will use the shortest (argument `-s`) CDS sequence (argument: `-t CDS`)
  for each entry as its representative isoform
- All isoforms will be saved to `TAIR10.shortest.cds.fa` (argument `-o TAIR10.shortest.cds.fa`)

```bash
$: ./build/bin/gfftk isoform -i ../example/GCF_000001735.4_TAIR10.demo.gff -f ../example/GCF_000001735.4_TAIR10.demo.fa -s -t CDS  -o TAIR10.shortest.cds.fa
```

#### Identifying and saving isoforms based on the longest exon sequence

- This will use the longest (argument: `-l`) exon (argument: `-t exon`) for each
  entry as its representative isoform
- All isoforms will be saved to `example.longest.cds.fa` (argument `-o TAIR10.longest.exon.fa`)

```bash
$: ./build/bin/gfftk isoform -i ../example/GCF_000001735.4_TAIR10.demo.gff -f ../example/GCF_000001735.4_TAIR10.demo.fa -l -t exon  -o TAIR10.longest.exon.fa
```

<!--
## Testing

### Calculate length of elements between lines (including) 212044 and 212053

```bash
$: sed -n '212044,212053p' ../data/GCF_000188115.5.gff | awk -F'\t' -v OFS="\t" '{len+=($5-$4)+1}END{print len}'

### Check identified genes

```bash
$: awk -F'\t' '($0!~/^#/)&&($3=="gene")' ../data/GCF_000001735.4_TAIR10.1_genomic.gff |cut -f1,4,5,9| sed -r "s/;.*$//g; s/ID=//"|les
``` -->
