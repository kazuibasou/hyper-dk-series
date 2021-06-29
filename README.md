# Hyper dK-series
Hyper dK-series is a family of randomized reference models for hypergraphs. 
The hyper dK-series produces randomized hypergraphs that preserve up to the individual node’s degree, node’s degree correlation, node’s redundancy coefficient, and/or the hyperedge’s size of the given hypergraph, depending on the parameter values *d<sub>v</sub>* = 0, 1, 2, or 2.5 and *d<sub>e</sub>* = 0 or 1.

## Requirements
Require gcc version 4.2.1 or later.

We have confirmed that our code works in the following environments.

- macOS 11.4
- Ubuntu 16.04.3 LTS

## Build
(i) Clone this repository.

(ii) Go to `hyper-dk-series/src` and run the following command:

	make

This generated the following structure of the directory.

	hyper-dk-series
	├ bin
	├ data
	├ rand_hypergraph
	└ src

If you find `hyper_dk_series` in `hyper-dk-series/bin`, the build has been successfully completed.

## Usage

### Input files

We need to feed two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, where *hypergraph* indicates a name the network and is arbitrary. 
These two files should be placed in `hyper-dk-series/data`.
In each of the two input files, each line contains one integer.
The file format follows that of [Austin R. Benson's data sets](https://github.com/arbenson/ScHoLP-Data).

#### *hypergraph*\_nverts.txt
The *i*th line of this file represents the number of nodes contained in the *i*th hyperedge.

#### *hypergraph*\_hyperedges.txt
This file contains a contiguous list of the nodes comprising the hyperedges, where the ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt.

#### Example
Let's consider a hypergraph, named `example-hypergraph,`that consists of a set of nodes V = {1, 2, 3, 4, 5} and a set of hyperedges E = {{1, 2}, {2, 3}, {1, 2, 3}, {1, 2, 3, 4}, {1, 2, 3, 4, 5}}. 
To read `example-hypergraph,`put the following two files in `hyper-dk-series/data/`:

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

### Generating randomized hypergraphs

Go to `hyper-dk-series/bin/` and run the following command:

	./hyper_dk_series <hypergraph> <dv> <de> <num_gen>

#### `<hypergraph>`
The name of the hypergraph.

#### `<dv>`
The value of *d<sub>v</sub>*, which should be 0, 1, 2, or 2.5.

#### `<de>`
The value of *d<sub>e</sub>*, which should be 0 or 1.

#### `<num_gen>`
The number of hypergraphs to be generated.

#### Example
To generate three randomized hypergraphs with (*d<sub>v</sub>*, *d<sub>e</sub>*) = (0, 1) for the hypergraph named `example-hypergraph,`run the following command:

	./hyper_dk_series example-hypergraph 0 1 3

### Output files
For the n-th (n=1, ..., *num_gen*) randomized hypergraph that has been generated, *hypergraph*\_*d<sub>v</sub>*\_*d<sub>e</sub>*\_*n*\_nverts.txt and *hypergraph*\_*d<sub>v</sub>*\_*d<sub>e</sub>*\_*n*\_hyperedges.txt will be created in `hyper-dk-series/rand_hypergraph/.`

For example, if one runs  

	./hyper_dk_series example-hypergraph 0 1 3

the following six files will be stored in `hyper-dk-series/rand_hypergraph:` 

example-hypergraph_0_1_1_nverts.txt

example-hypergraph_0_1_1_hyperedges.txt

example-hypergraph_0_1_2_nverts.txt

example-hypergraph_0_1_2_hyperedges.txt

example-hypergraph_0_1_3_nverts.txt

example-hypergraph_0_1_3_hyperedges.txt

### Notes
- In general, when *d<sub>v</sub>* <= 1, the generation of a randomized hypergraph is fast. When *d<sub>v</sub>* = 2, it takes longer. When *d<sub>v</sub>* = 2.5, it takes even longer time than when *d<sub>v</sub>* = 2. The value of *d<sub>e</sub>* (= 0 or 1) does not much affect the speed.

## Reference

Kazuki Nakajima, Kazuyuki Shudo, Naoki Masuda. Randomizing hypergraphs preserving degree correlation and local clustering, June 2021. [<a href="https://arxiv.org/abs/2106.12162">arXiv</a>]

## License

This source code is released under the MIT License, see LICENSE.txt.

## Contact
- Kazuki Nakajima (https://kazuibasou.github.io/index_en.html)
- nakajima.k.an[at]m.titech.ac.jp

(Last update: 2021/06/25)