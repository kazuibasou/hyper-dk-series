# Hyper $dK$-series
Hyper dK-series is a family of randomized reference models for hypergraphs.

## Requirements
Require gcc version 4.2.1 or later.

I confirmed that my code works on the following environments.

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

## Usage
Execute the following command at  `hyper-dk-series/bin`.

	./hyper_dk_series <hypergraph> <d_v> <d_e>

### Arguments
#### `<hypergraph>`
Require *hypergraph*\_nverts.txt and *hypergraph*\_hyperedges.txt at `hyper-dk-series/data`.

#### `<d_v>`
Specify 0, 1, 2, or 2.5.

#### `<d_e>`
Specify 0 or 1.

### Output
Two output files, *hypergraph*\_d_v_*d_v*\_d_e\_*d_e*\_nverts.txt and *hypergraph*\_d_v_*d_v*\_d_e\_*d_e*\_hyperedges.txt, are made at `hyper-dk-series/rand_hypergraph`. 
