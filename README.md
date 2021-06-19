# hyper-dk-series
Hyper dK-series is a family of randomized reference models for hypergraphs.

## Requirements
Require gcc version 4.2.1 or later.

We have confirmed that our source code works on the following environments.

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

	./hyper_dk_series <hypergraph_name> <d_v_value> <d_e_value>

### `<hypergraph_name>`
Require *hypergraph_name*\_nverts.txt and *hypergraph_name*\_hyperedges.txt at `hyper-dk-series/data`.

### `<d_v_value>`
Specify 0, 1, 2, or 2.5.

### `<d_e_value>`
Specify 0 or 1.