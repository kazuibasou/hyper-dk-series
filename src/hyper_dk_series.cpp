#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <set>
#include <random>
#include <unordered_map>
#include "basic_function.h"
#include "bipartite_graph.h"
#include "bipartite_generator.h"
#include "bipartite_rewiring.h"

int main(int argc,char *argv[]){
	if(argc != 4){
		printf("Please input following:\n./hyper_dk_series (name of hypergraph) (value of d_v) (value of d_e)\n");
		exit(0);
	}
	const char *graphname = argv[1];
	const char *d_v = argv[2];
	const char *d_e = argv[3];

	BipartiteGraph B;
	B.read_hypergraph(graphname);

	return 0;
}