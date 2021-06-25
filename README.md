# Hyper dK-series
Hyper dK-series is a family of randomized reference models for hypergraphs.

## Requirements
Require gcc version 4.2.1 or later.

I have confirmed that my code works on the following environments.

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

If you find `hyper_dk_series` at `hyper-dk-series/bin`, the build has been successfully completed.

## Usage

### Input files

Require to put two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, at `hyper-dk-series/data`, where *hypergraph* indicates a name of hypergraph data to read.
All two files represent a vector of integers. 
There is one integer per line.
These file formats follow the formats of [Benson's datasets](https://github.com/arbenson/ScHoLP-Data).

#### *hypergraph*\_nverts.txt
Write the number of nodes contained in each hyperedge of the original hypergraph on each line of this file.

#### *hypergraph*\_hyperedges.txt
Write a contiguous list of the nodes comprising the hyperedges, where the ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt.

#### Example
Let's consider a hypergraph, named `example_hypergraph`, that consists of a set of nodes V = {1, 2, 3} and a set of hyperedges E = {(1, 2), (1, 2, 3)}. 
To read the hypergraph `example_hypergraph`, one puts the following two files at `hyper-dk-series/data`.

example_hypergraph_nverts.txt
```
2
3
```

```txt:example_hypergraph_hyperedges.txt
1
2
1
2
3
```

### Randomizing input hypergraph

Execute the following command at  `hyper-dk-series/bin`.

	./hyper_dk_series <hypergraph> <d_v> <d_e> <num_gen>

#### `<hypergraph>`
Require *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt at `hyper-dk-series/data`.

#### `<d_v>`
Specify 0, 1, 2, or 2.5.

#### `<d_e>`
Specify 0 or 1.

#### `<num_gen>`
The number of generatations for randomized hypergraphs.

### Output files

## Reference

Preprint: [<a href="https://arxiv.org/abs/2106.12162">arXiv</a>]

## License

This source code is released under the MIT License, see LICENSE.txt.

## Contact
- Kazuki Nakajima (https://kazuibasou.github.io/index_en.html)
- nakajima.k.an[at]m.titech.ac.jp

(Last update: 2021/06/24)