<h1 align="center">
Hyper dK-series <br/>  
<i>Randomizing Hypergraphs Preserving Degree Correlation and Local Clustering</i>
</h1>

<p align="center">
<i>A family of reference models for hypergraphs</i>
</p>

<p align="center">
<a href="https://github.com/kazuibasou/hyper-dk-series/blob/main/LICENSE" target="_blank">
<img alt="License: MIT" src="https://img.shields.io/github/license/kazuibasou/hyperneo">
</a>

<a href="https://arxiv.org/abs/2106.12162" target="_blank">
<img alt="ARXIV: 2106.12162" src="https://img.shields.io/badge/arXiv-2106.12162-red.svg">
</a>

</p>

Hyper dK-series is a family of randomization methods for hypergraphs. 
The hyper dK-series produces randomized hypergraphs that preserve up to the individual node’s degree, node’s degree correlation, node’s redundancy coefficient, and/or the hyperedge’s size of the given hypergraph, depending on the parameter values *d<sub>v</sub>* = 0, 1, 2, or 2.5 and *d<sub>e</sub>* = 0 or 1.
In general, when *d<sub>v</sub>* = 0 or 1, the code runs fast. When *d<sub>v</sub>* = 2, it takes longer. When *d<sub>v</sub>* = 2.5, it takes even longer than when *d<sub>v</sub>* = 2. The value of *d<sub>e</sub>* (= 0 or 1) does not much affect the speed. 

We provide code for the hyper dK-series in Python and C++.
The python code is easier to use. 
The C++ code is much faster than the Python code.

# What's new

- 2024/09/28: Implemented Python code with [numba](https://numba.pydata.org/) for the hyper dk-series, which considerably reduces generation time for the parameter *d<sub>v</sub>* = 2 and 2.5.

# Python

## Requirements

We have confirmed that our code works in the following environments.

### Python and its libraries

- Python 3.10.15
- numpy 2.0.2
- numba 0.60.0
- matplotlib 3.9.2
- hypernetx 2.3.0

### OS
- macOS 14.4

## Usage

First, clone this repository:

	git clone git@github.com:kazuibasou/hyper-dk-series.git

### Input files

We need to feed two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, where *hypergraph* indicates the name of the network and is arbitrary. 
These two files should be placed in `hyper-dk-series/data/`.
In each of the two files, each line contains one integer.
The file format follows that of [Austin R. Benson's data sets](https://github.com/arbenson/ScHoLP-Data).

#### *hypergraph*\_nverts.txt
The *i* th line of this file represents the number of nodes contained in the *i* th hyperedge.

#### *hypergraph*\_hyperedges.txt
This file contains a contiguous list of indices of the nodes comprising the hyperedges, where the ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt.

**Note that our code successfully works as long as the node index starts at 0 and increments by 1.**

#### Example
Let's consider a hypergraph, named `example-hypergraph`, that consists of a set of nodes *V* = {0, 1, 2, 3, 4} and a set of hyperedges *E* = {{0, 1}, {1, 2}, {0, 1, 2}, {0, 1, 2, 3}, {0, 1, 2, 3, 4}}. 
Then, the two input files are as follows:

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
0
1
1
2
0
1
2
0
1
2
3
0
1
2
3
4
```

### Generating randomized hypergraphs

Please see the notebooks `1_basics.ipynb` and `2_randomization.ipynb` in the folder `hyper-dk-series/py/` for instructions on using the hyper dk-series in Python.

# C++

## Requirements
Require gcc version 4.2.1 or later.

We have confirmed that our code works in the following environments.

- macOS 14.4

## Build
(i) Clone this repository:

	git clone git@github.com:kazuibasou/hyper-dk-series.git

(ii) Go to `hyper-dk-series/cpp/src/`:

	cd hyper-dk-series/cpp/src

(iii) Run the make command:

	make

This generates the following structure of the directory.

	hyper-dk-series/
	├ cpp/
	   ├ bin/
	   └ src/
	├ data/
	├ py/
	└ rand_hypergraph/

If you find a file `hyper_dk_series` in the folder `hyper-dk-series/cpp/bin/`, the build has been successfully completed.

## Usage

### Input files

We need to feed two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, where *hypergraph* indicates the name of the network and is arbitrary. 
These two files should be placed in `hyper-dk-series/data/`.
In each of the two files, each line contains one integer.
The file format follows that of [Austin R. Benson's data sets](https://github.com/arbenson/ScHoLP-Data).

#### *hypergraph*\_nverts.txt
The *i* th line of this file represents the number of nodes contained in the *i* th hyperedge.

#### *hypergraph*\_hyperedges.txt
This file contains a contiguous list of the nodes comprising the hyperedges, where the ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt.

### Generating randomized hypergraphs

Go to `hyper-dk-series/cpp/bin/` and run the following command:

	./hyper_dk_series <hypergraph> <dv> <de> <num_gen>

The four arguments are as follows.

#### `<hypergraph>`
The name of the hypergraph.

#### `<dv>`
The value of *d<sub>v</sub>*, which should be 0, 1, 2, or 2.5.

#### `<de>`
The value of *d<sub>e</sub>*, which should be 0 or 1.

#### `<num_gen>`
The number of hypergraphs to be generated.

#### Example
To generate three randomized hypergraphs with (*d<sub>v</sub>*, *d<sub>e</sub>*) = (0, 1) for the hypergraph named `example-hypergraph`, go to `hyper-dk-series/cpp/bin/` and run the following command:

	./hyper_dk_series example-hypergraph 0 1 3

### Output files
The *n* th (*n*=1, ..., *num_gen*) randomized hypergraph, i.e., *hypergraph*\_*dv*\_*de*\_*n*\_nverts.txt and *hypergraph*\_*dv*\_*de*\_*n*\_hyperedges.txt will be created in the folder `hyper-dk-series/rand_hypergraph/`.

# Reference

[1] Kazuki Nakajima, Kazuyuki Shudo, Naoki Masuda. Randomizing Hypergraphs Preserving Degree Correlation and Local Clustering. December 2021. [<a href="https://doi.org/10.1109/TNSE.2021.3133380">paper</a>]

# License

This source code is released under the MIT License, see LICENSE.txt.

# Contact
- Kazuki Nakajima (https://kazuibasou.github.io/)

(Last update: 2024/09/28)