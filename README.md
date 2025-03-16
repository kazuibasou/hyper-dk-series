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

The hyper dK-series is a family of randomization methods for hypergraphs [K. Nakajima et al. *IEEE TNSE* (2021)](https://doi.org/10.1109/TNSE.2021.3133380). 
The hyper dK-series produces randomized hypergraphs that preserve up to the individual node’s degree, node’s degree correlation, node’s redundancy coefficient, and/or the hyperedge’s size of the given hypergraph, depending on the parameter values *d<sub>v</sub>* = 0, 1, 2, or 2.5 and *d<sub>e</sub>* = 0 or 1.
In general, when *d<sub>v</sub>* = 0 or 1, the code runs fast. When *d<sub>v</sub>* = 2, it takes longer. When *d<sub>v</sub>* = 2.5, it takes even longer than when *d<sub>v</sub>* = 2. The value of *d<sub>e</sub>* (= 0 or 1) does not much affect the speed. 

We provide code for the hyper dK-series in Python and C++.
The python code is easier to use. 

If you use this code, please cite

- [Kazuki Nakajima, Kazuyuki Shudo, Naoki Masuda. Randomizing Hypergraphs Preserving Degree Correlation and Local Clustering. *IEEE TNSE*. vol. 9, pp. 1139-1153 (2021).](https://doi.org/10.1109/TNSE.2021.3133380)
- [Kazuki Nakajima, Kazuyuki Shudo, Naoki Masuda. Higher-order rich-club phenomenon in collaborative research grant networks. *Scientometrics*. vol. 128, pp. 2429–2446 (2023).](https://doi.org/10.1007/s11192-022-04621-1)

# What's new

- March 2025: We implemented Python code for the detection of higher-order rich clubs [K. Nakajima et al. *Scientometrics* (2023)](https://doi.org/10.1007/s11192-022-04621-1).

- September 2024: We implemented Python code with [Numba](https://numba.pydata.org/) for the hyper dK-series, achieving a **20–30× speed up** over the previous version for *d<sub>v</sub>* = 2 and 2.5 and comparable runtime to the C++ implementation. Randomized hypergraph instances generated by the hyper dK-series in Python can be converted into a Hypergraph object in [HyperNetX](https://github.com/pnnl/HyperNetX).


# Python

## Requirements

We have confirmed that our code works in the following environment.

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
This file contains a contiguous list of indices of the nodes comprising the hyperedges, one node per line. The ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt. See the following example.

**Note that, in these two files, the node index has to start at 0 and be contiguous (i.e., 0, 1, 2, ...).**

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

Please see the notebooks [1_basics.ipynb](https://github.com/kazuibasou/hyper-dk-series/blob/main/py/1_basics.ipynb) and [2_randomization.ipynb](https://github.com/kazuibasou/hyper-dk-series/blob/main/py/2_randomization.ipynb) located in the folder `hyper-dk-series/py/` for instructions on using the hyper dk-series in Python.

## Benchmark (October 2024)

We measured the runtime (in seconds) for generating a single randomized instance using the hyper dK-series implemented in Python with Numba.  
We used the same four empirical hypergraph datasets as in Ref. [K. Nakajima et al. *IEEE TNSE* (2021)](https://doi.org/10.1109/TNSE.2021.3133380): `drug`, `Enron`, `primary-school`, and `high-school`. 

While the hyper dK-series with *d<sub>v</sub>* = 2 or 2.5 requires more time due to frequent hyperedge rewiring attempts, **the current version is 20–30 times faster than the previous version, which did not employ Numba.**

| (*d<sub>v</sub>*, *d<sub>e</sub>*) | drug | Enron | primary-school | high-school |
| ---- | ---- | ---- | ---- | ---- |
| (0, 0) | 0.02 | 0.008 | 0.06 | 0.05 |
| (1, 0) | 0.008 | 0.005 | 0.03 | 0.02 |
| (2, 0) | 5.6 | 3.1 | 23.6 | 9.7 |
| (2.5, 0) | 101 | 42.7 | 709 | 188 |
| (0, 1) | 0.02 | 0.008 | 0.16 | 0.05 |
| (1, 1) | 0.008 | 0.005 | 0.03 | 0.02 |
| (2, 1) | 5.1 | 1.9 | 21.1 | 7.7 |
| (2.5, 1) | 97 | 40.0 | 605 | 166 |

## Application: Rich club detection

We define a higher-order rich club in which the nodes with the largest degrees are densely interconnected by hyperedges. 
Please see the notebook [3_rich_club.ipynb](https://github.com/kazuibasou/hyper-dk-series/blob/main/py/3_rich_club.ipynb) located in the folder `hyper-dk-series/py/` for instructions on the detection of higher-order rich clubs in Python.
Please also see Ref. [K. Nakajima et al. *Scientometrics* (2023)](https://doi.org/10.1007/s11192-022-04621-1) for details on the detection method.

# C++

## Requirements
Require gcc version 4.2.1 or later.

We have confirmed that our code works in the following environment.

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
See the [Input files section](https://github.com/kazuibasou/hyper-dk-series?tab=readme-ov-file#input-files) for the Python code.

#### *hypergraph*\_nverts.txt
The *i* th line of this file represents the number of nodes contained in the *i* th hyperedge.

#### *hypergraph*\_hyperedges.txt
This file contains a contiguous list of indices of the nodes comprising the hyperedges, one node per line. The ordering of the hyperedges is the same as in *hypergraph*\_nverts.txt. 

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

# License

This source code is released under the MIT License, see LICENSE.txt.

# Contact
- Kazuki Nakajima (https://kazuibasou.github.io/)

(Last update: March 2025)
