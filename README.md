# Hyper dK-series
Hyper dK-series is a family of randomized reference models for hypergraphs. 
The hyper dK-series produces randomized hypergraphs that preserve up to the individual node’s degree, node’s degree correlation, node’s redundancy coefficient, and/or the hyperedge’s size of the given hypergraph, depending on the parameter values *dv* = 0, 1, 2, or 2.5 and *de* = 0 or 1.

## Requirements
Require gcc version 4.2.1 or later.

We have confirmed that our code works on the following environments.

- macOS 11.4
- Ubuntu 16.04.3 LTS

## Build
Clone this repository and then execute `make` command at `hyper-dk-series/src`.

The structure of the directory will be as follows:

	hyper-dk-series
	├ bin
	├ data
	├ rand_hypergraph
	└ src

If you find `hyper_dk_series` in `hyper-dk-series/bin`, the build has been successfully completed.

## Usage

### Input files

Require to put two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, in `hyper-dk-series/data`, where *hypergraph* indicates a name of hypergraph data to read.
All two files represent a vector of integers. 
There is one integer per line.
These file formats follow the formats of [Benson's datasets](https://github.com/arbenson/ScHoLP-Data).

#### *hypergraph*\_nverts.txt
Write the number of nodes contained in each hyperedge on each line of this file.

#### *hypergraph*\_hyperedges.txt
Write a contiguous list of the nodes comprising the hyperedges, where the ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt.

#### Example
Let's consider a hypergraph, named `example-hypergraph`, that consists of a set of nodes V = {1, 2, 3, 4, 5} and a set of hyperedges E = {{1, 2}, {2, 3}, {1, 2, 3}, {1, 2, 3, 4}, {1, 2, 3, 4, 5}}. 
To read the hypergraph `example-hypergraph`, put the following two files in `hyper-dk-series/data`:

example-hypergraph_nverts.txt
``` text:
2
2
3
4
5
```

example-hypergraph_hyperedges.txt
```text:
1
2
2
3
1
2
3
1
2
3
4
1
2
3
4
5
```

### Randomizing input hypergraph

Execute the following command at  `hyper-dk-series/bin`.

	./hyper_dk_series <hypergraph> <dv> <de> <num_gen>

#### `<hypergraph>`
A name of hypergraph data to read.
Require to put *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt in `hyper-dk-series/data`.

#### `<dv>`
Specify 0, 1, 2, or 2.5.

#### `<de>`
Specify 0 or 1.

#### `<num_gen>`
Specify the number of generatations for randomized hypergraphs.

#### Example
To generate 3 randomized hypergraphs with (*dv*, *de*) = (0, 1) for the hypergraph named `example-hypergraph`, run the following command:

	./hyper_dk_series example-hypergraph 0 1 3

### Output files
For the n-th (n=1, ..., *num_gen*) randomized hypergraph with given (*dv*, *de*), *hypergraph*\_*dv*\_*de*\_*n*\_nverts.txt and *hypergraph*\_*dv*\_*de*\_*n*\_hyperedges.txt will be generated in `hyper-dk-series/rand_hypergraph`.

For example, if one runs the following command, 

	./hyper_dk_series example-hypergraph 0 1 3

the following six files will be generated in `hyper-dk-series/rand_hypergraph`: 

example-hypergraph_0_1_1_nverts.txt

example-hypergraph_0_1_1_hyperedges.txt

example-hypergraph_0_1_2_nverts.txt

example-hypergraph_0_1_2_hyperedges.txt

example-hypergraph_0_1_3_nverts.txt

example-hypergraph_0_1_3_hyperedges.txt

Then, for example, you can use 1st randomized hypergraph data for your purposes such as research or analysis by reading example-hypergraph_0_1_1_nverts.txt and example-hypergraph_0_1_1_nverts.txt.

### Notes
- In general, when *dv* <= 1, a generation of a randomized hypergraph is very fast. When *dv* = 2, a generation relatively takes time. When *dv* = 2.5, a generation further takes a longer time than that when *dv* = 2. The value of *de* (= 0 or 1) has no significant effect on the generation time.

## Reference

Kazuki Nakajima, Kazuyuki Shudo, Naoki Masuda. Randomizing hypergraphs preserving degree correlation and local clustering, June 2021. [<a href="https://arxiv.org/abs/2106.12162">arXiv</a>]

## License

This source code is released under the MIT License, see LICENSE.txt.

## Contact
- Kazuki Nakajima (https://kazuibasou.github.io/index_en.html)
- nakajima.k.an[at]m.titech.ac.jp

(Last update: 2021/06/25)