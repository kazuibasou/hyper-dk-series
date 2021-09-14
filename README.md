# Hyper dK-series
Hyper dK-series is a family of randomization methods for hypergraphs. 
The hyper dK-series produces randomized hypergraphs that preserve up to the individual node’s degree, node’s degree correlation, node’s redundancy coefficient, and/or the hyperedge’s size of the given hypergraph, depending on the parameter values *d<sub>v</sub>* = 0, 1, 2, or 2.5 and *d<sub>e</sub>* = 0 or 1.

We provide the python code and the C++ code for the hyper dK-series.
The python code is easy to use. 
The C++ code is much faster than the python code.

# Python

## Requirements
Require Python 3 series.

We have confirmed that our code works in the following environments.

- macOS 11.4 and 11.5
- Ubuntu 16.04.3 LTS

## Usage

First, clone this repository:

	git clone git@github.com:kazuibasou/hyper-dk-series.git

### Input files

We need to feed two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, where *hypergraph* indicates a name the network and is arbitrary. 
These two files should be placed in `hyper-dk-series/data/`.
In each of the two input files, each line contains one integer.
The file format follows that of [Austin R. Benson's data sets](https://github.com/arbenson/ScHoLP-Data).

#### *hypergraph*\_nverts.txt
The *i* th line of this file represents the number of nodes contained in the *i* th hyperedge.

#### *hypergraph*\_hyperedges.txt
This file contains a contiguous list of the nodes comprising the hyperedges, where the ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt.

#### Example
Let's consider a hypergraph, named `example-hypergraph`, that consists of a set of nodes *V* = {1, 2, 3, 4, 5} and a set of hyperedges *E* = {{1, 2}, {2, 3}, {1, 2, 3}, {1, 2, 3, 4}, {1, 2, 3, 4, 5}}. 
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

Go to `hyper-dk-series/py/` and run the following command:

	python main.py <hypergraph> <dv> <de> <num_gen>

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
To generate three randomized hypergraphs with (*d<sub>v</sub>*, *d<sub>e</sub>*) = (0, 1) for the hypergraph named `example-hypergraph`, go to `hyper-dk-series/py/` and run the following command:

	python main.py example-hypergraph 0 1 3

### Output files
The *n* th (*n*=1, ..., *num_gen*) randomized hypergraph, i.e., *hypergraph*\_*dv*\_*de*\_*n*\_nverts.txt and *hypergraph*\_*dv*\_*de*\_*n*\_hyperedges.txt will be created in the folder `hyper-dk-series/rand_hypergraph/`.

For example, if one runs  

	python main.py example-hypergraph 0 1 3

the following six files will be stored in `hyper-dk-series/rand_hypergraph/`: 

example-hypergraph_0_1_1_nverts.txt

example-hypergraph_0_1_1_hyperedges.txt

example-hypergraph_0_1_2_nverts.txt

example-hypergraph_0_1_2_hyperedges.txt

example-hypergraph_0_1_3_nverts.txt

example-hypergraph_0_1_3_hyperedges.txt

### HyperGraph class
We introduce how to use the HyperGraph class. 
We can perform a basic analysis of a hypergraph by using this class and its functions.

The HyperGraph class composes of three variables:

- V: A list of nodes.
- E: A list of hyperedges.
- elist: A dictionary of lists of indices in the list E of hyperedges to which each node belongs.

Let's represent a hypergraph that consists of a set of nodes *V* = {1, 2, 3, 4, 5} and a set of hyperedges *E* = {{1, 2}, {2, 3}, {1, 2, 3}, {1, 2, 3, 4}, {1, 2, 3, 4, 5}}, as *G*.
First, open a Python interactive session at `hyper-dk-series/py/`:

	python

Then, we initialize *G* as follows:

	>>> import hypergraph
	>>> G = hypergraph.HyperGraph()
	>>> G.V = [1, 2, 3, 4, 5]
	>>> G.E = [[1, 2], [2, 3], [1, 2, 3], [1, 2, 3, 4], [1, 2, 3, 4, 5]]
	>>> G.elist = {1: [0, 2, 3, 4], 2: [0, 1, 2, 3, 4], 3: [1, 2, 3, 4], 4: [3, 4], 5: [4]}

For example, G.elist[1] = [0, 2, 3, 4] implies that node 1 belongs to hyperedges G.E[0], G.E[2], G.E[3], and G.E[4].

In the following, we introduce functions in the HyperGraph class for a basic analysis of a given hypergraph.

### hypergraph.HyperGraph.read_hypergraph(hypergraph_name)

Read a hypergraph named `hypergraph_name` from the corresponding files in `hyper-dk-series/data/`.

For example, if we want to read a hypergraph, named `example-hypergraph`, we just run the following commands:

	>>> import hypergraph
	>>> hypergraph_name = "example-hypergraph"
	>>> G = hypergraph.HyperGraph()
	>>> G.read_hypergraph(hypergraph_name)
	Hypergraph named example-hypergraph was read.
	Number of nodes: 5
	Number of hyperedges: 5

Then, we should obtain

	>>> print(G.V)
	[1, 2, 3, 4, 5]
	>>> print(G.E)
	[[1, 2], [2, 3], [1, 2, 3], [1, 2, 3, 4], [1, 2, 3, 4, 5]]
	>>> print(G.elist)
	{1: [0, 2, 3, 4], 2: [0, 1, 2, 3, 4], 3: [1, 2, 3, 4], 4: [3, 4], 5: [4]}

### hypergraph.HyperGraph.add_node_to_hyperedge(v, e_i)

Add node *v* to hyperedge *E[e_i]*. 

For example, the following command adds node 3 to the hyperedge E[0] in the graph G:

	>>> G.add_node_to_hyperedge(3, 0)
	>>> print(G.E) 
	[[1, 2, 3], [2, 3], [1, 2, 3], [1, 2, 3, 4], [1, 2, 3, 4, 5]]
	>>> print(G.elist)
	{1: [0, 2, 3, 4], 2: [0, 1, 2, 3, 4], 3: [1, 2, 3, 4, 0], 4: [3, 4], 5: [4]}

### hypergraph.HyperGraph.remove_node_from_hyperedge(v, e_i)

Remove node *v* from hyperedge *E[e_i]*. 

For example, the following command adds node 3 to the hyperedge E[0] in the graph G:

	>>> G.remove_node_from_hyperedge(3, 0)
	>>> print(G.E)
	[[1, 2], [2, 3], [1, 2, 3], [1, 2, 3, 4], [1, 2, 3, 4, 5]]
	>>> print(G.elist)
	{1: [0, 2, 3, 4], 2: [0, 1, 2, 3, 4], 3: [1, 2, 3, 4], 4: [3, 4], 5: [4]}

### hypergraph.HyperGraph.node_degree()

Return the degree of each node (i.e., the number of hyperedges to which each node belongs).

Example: 

	>>> G.node_degree()
	{1: 4, 2: 5, 3: 4, 4: 2, 5: 1}
	>>> nd = G.node_degree()
	>>> nd[1]
	4

Note that the key in the dictionary is the node's index.

### hypergraph.HyperGraph.num_jnt_node_deg()

Return the number of hyperedges that nodes with degree k and nodes with degree k' share.

See Section 2.2 in Ref. [1] for the detailed definition.

Example:

	>>> G.num_jnt_node_deg()
	{1: {1: 0, 2: 1, 4: 2, 5: 1}, 2: {1: 1, 2: 0, 4: 4, 5: 2}, 4: {1: 2, 2: 4, 4: 6, 5: 8}, 5: {1: 1, 2: 2, 4: 8, 5: 0}}
	>>> njnd = G.num_jnt_node_deg()
	>>> njnd[1][2]
	1

Note that the key in the dictionary is a pair of node's degrees.

### hypergraph.HyperGraph.node_redundancy_coefficient()

Return the redundancy coefficient of each node.

See Section 2.2 in Ref. [1] for the detailed definition.

Example:

	>>> G.node_redundancy_coefficient()
	{1: 1.0, 2: 0.9, 3: 1.0, 4: 1.0, 5: 0}
	>>> nrc = G.node_redundancy_coefficient()
	>>> nrc[1]
	1.0

Note that the key in the dictionary is the node's index.

### hypergraph.HyperGraph.degree_dependent_node_redundancy_coefficient()

Return the degree-dependent redundancy coefficient of the node (i.e., the average of the redundancy coefficient over the nodes with degree k).

See Section 2.2 in Ref. [1] for the detailed definition.

Example:

	>>> G.degree_dependent_node_redundancy_coefficient()
	{1: 0.0, 2: 1.0, 4: 1.0, 5: 0.9}
	>>> ddnrc = G.degree_dependent_node_redundancy_coefficient()
	>>> ddnrc[1]
	0.0

Note that the key in the dictionary is the node's degree.

### hypergraph.HyperGraph.hyperedge_size()

Return the size of each hyperedge (i.e., the number of nodes that belong to each hyperedge).

Example: 

	>>> G.hyperedge_size()
	{0: 2, 1: 2, 2: 3, 3: 4, 4: 5}
	>>> hs = G.hyperedge_size()
	>>> hs[0]
	2

Note that the key in the dictionary is the index of the hyperedge in the list E.
For example, hs[0] is the size of hyperedge G.E[0].

# C++

## Requirements
Require gcc version 4.2.1 or later.

We have confirmed that our code works in the following environments.

- macOS 11.4 and 11.5
- Ubuntu 16.04.3 LTS

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

We need to feed two files, *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt, where *hypergraph* indicates a name the network and is arbitrary. 
These two files should be placed in `hyper-dk-series/data/`.
In each of the two input files, each line contains one integer.
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

### Notes
- In general, when *d<sub>v</sub>* = 0 or 1, the code runs fast. When *d<sub>v</sub>* = 2, it takes longer. When *d<sub>v</sub>* = 2.5, it takes even longer than when *d<sub>v</sub>* = 2. The value of *d<sub>e</sub>* (= 0 or 1) does not much affect the speed.

## Reference

[1] Kazuki Nakajima, Kazuyuki Shudo, Naoki Masuda. Randomizing hypergraphs preserving degree correlation and local clustering, June 2021. [<a href="https://arxiv.org/abs/2106.12162">arXiv</a>]

## License

This source code is released under the MIT License, see LICENSE.txt.

## Contact
- Kazuki Nakajima (https://kazuibasou.github.io/index_en.html)
- nakajima.k.an[at]m.titech.ac.jp

(Last update: 2021/09/14)