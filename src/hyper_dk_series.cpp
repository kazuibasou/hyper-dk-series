#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include "hypergraph.h"
#include "rewiring.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

HyperGraph randomizing_d_v_zero_d_e_zero(HyperGraph G){

	const int N = int(G.V.size());
	const int M = int(G.E.size());
	int B_M = 0;
	for(int v:G.V){
		B_M += int(G.elist[v].size());
	}

	std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> randN(0, N-1);
  std::uniform_int_distribution<int> randM(0, M-1);

  HyperGraph randG;
	randG.V = std::vector<int>(G.V);
	randG.E = std::vector<int>(G.E);
	for(int v:randG.V){
		randG.elist[v] = std::vector<int>();
	}
	for(int m:randG.E){
		randG.vlist[m] = std::vector<int>();
	}

	int v, m;
	for(int i=0; i<B_M; ++i){
		v = randN(mt);
		m = randM(mt);
		randG.elist[v].push_back(m);
		randG.vlist[m].push_back(v);
	}

	for(int v:randG.V){
		std::sort(randG.elist[v].begin(),randG.elist[v].end());
	}
	for(int m:randG.E){
		std::sort(randG.vlist[m].begin(),randG.vlist[m].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 0).\n");

	return randG;
}

HyperGraph randomizing_d_v_one_d_e_zero(HyperGraph G){

	std::vector<int> stublist;
	for(int v:G.V){
		int d = int(G.elist[v].size());
		for(int i=0; i<d; ++i){
			stublist.push_back(v);
		}
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::shuffle(stublist.begin(), stublist.end(), mt);

	const int M = int(G.E.size());
	std::uniform_int_distribution<int> randM(0, M-1);

	HyperGraph randG;
	randG.V = std::vector<int>(G.V);
	randG.E = std::vector<int>(G.E);
	for(int v:randG.V){
		randG.elist[v] = std::vector<int>();
	}
	for(int m:randG.E){
		randG.vlist[m] = std::vector<int>();
	}

	int v, m;
	while(int(stublist.size()) > 0){
		v = stublist.back();
		stublist.pop_back();
		m = randM(mt);
		randG.elist[v].push_back(m);
		randG.vlist[m].push_back(v);
	}

	for(int v:randG.V){
		std::sort(randG.elist[v].begin(),randG.elist[v].end());
	}
	for(int m:randG.E){
		std::sort(randG.vlist[m].begin(),randG.vlist[m].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 0).\n");

	return randG;
}

HyperGraph randomizing_d_v_two_d_e_zero(HyperGraph G){

	HyperGraph randG;
	
	randG = randomizing_d_v_one_d_e_zero(G);

	randG = targeting_rewiring_d_v_two(G, randG);

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 0).\n");

	return randG;
}

HyperGraph randomizing_d_v_two_five_d_e_zero(HyperGraph G){

	HyperGraph randG;

	randG = randomizing_d_v_two_d_e_zero(G);

	randG = targeting_rewiring_d_v_two_five(G, randG);
	
	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 0).\n");

	return randG;
}

HyperGraph randomizing_d_v_zero_d_e_one(HyperGraph G){

	std::vector<int> stublist;
	for(int m:G.E){
		int s = int(G.vlist[m].size());
		for(int i=0; i<s; ++i){
			stublist.push_back(m);
		}
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::shuffle(stublist.begin(), stublist.end(), mt);

	const int N = int(G.V.size());
	std::uniform_int_distribution<int> randN(0, N-1);

	HyperGraph randG;
	randG.V = std::vector<int>(G.V);
	randG.E = std::vector<int>(G.E);
	for(int v:randG.V){
		randG.elist[v] = std::vector<int>();
	}
	for(int m:randG.E){
		randG.vlist[m] = std::vector<int>();
	}

	int v, m;
	while(int(stublist.size()) > 0){
		v = randN(mt);
		m = stublist.back();
		stublist.pop_back();
		randG.elist[v].push_back(m);
		randG.vlist[m].push_back(v);
	}

	for(int v:randG.V){
		std::sort(randG.elist[v].begin(),randG.elist[v].end());
	}
	for(int m:randG.E){
		std::sort(randG.vlist[m].begin(),randG.vlist[m].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 1).\n");

	return randG;
}

HyperGraph randomizing_d_v_one_d_e_one(HyperGraph G){

	std::vector<int> node_stublist;
	for(int v:G.V){
		int d = int(G.elist[v].size());
		for(int i=0; i<d; ++i){
			node_stublist.push_back(v);
		}
	}

	std::vector<int> edge_stublist;
	for(int m:G.E){
		int s = int(G.vlist[m].size());
		for(int i=0; i<s; ++i){
			edge_stublist.push_back(m);
		}
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::shuffle(node_stublist.begin(), node_stublist.end(), mt);
	std::shuffle(edge_stublist.begin(), edge_stublist.end(), mt);

	HyperGraph randG;
	randG.V = std::vector<int>(G.V);
	randG.E = std::vector<int>(G.E);
	for(int v:randG.V){
		randG.elist[v] = std::vector<int>();
	}
	for(int m:randG.E){
		randG.vlist[m] = std::vector<int>();
	}

	int v, m;
	while(int(node_stublist.size()) > 0 && int(edge_stublist.size()) > 0){
		v = node_stublist.back();
		node_stublist.pop_back();
		m = edge_stublist.back();
		edge_stublist.pop_back();
		randG.elist[v].push_back(m);
		randG.vlist[m].push_back(v);
	}

	for(int v:randG.V){
		std::sort(randG.elist[v].begin(),randG.elist[v].end());
	}
	for(int m:randG.E){
		std::sort(randG.vlist[m].begin(),randG.vlist[m].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 1).\n");

	return randG;
}

HyperGraph randomizing_d_v_two_d_e_one(HyperGraph G){

	HyperGraph randG;

	randG = randomizing_d_v_one_d_e_one(G);

	randG = targeting_rewiring_d_v_two(G, randG);

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 1).\n");

	return randG;
}

HyperGraph randomizing_d_v_two_five_d_e_one(HyperGraph G){

	HyperGraph randG;

	randG = randomizing_d_v_two_d_e_one(G);
	
	randG = targeting_rewiring_d_v_two_five(G, randG);

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 1).\n");

	return randG;
}

HyperGraph randomizing_hypergraph(HyperGraph G, const double d_v, const double d_e){

	HyperGraph randG;

	if(d_v == 0 && d_e == 0){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_zero_d_e_zero(G);
	}
	else if(d_v == 1 && d_e == 0){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_one_d_e_zero(G);
	}
	else if(d_v == 2 && d_e == 0){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_two_d_e_zero(G);
	}
	else if(d_v == 2.5 && d_e == 0){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%2.1lf, %d).\n", d_v, int(d_e));
		randG = randomizing_d_v_two_five_d_e_zero(G);
	}
	else if(d_v == 0 && d_e == 1){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_zero_d_e_one(G);
	}
	else if(d_v == 1 && d_e == 1){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_one_d_e_one(G);
	}
	else if(d_v == 2 && d_e == 1){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_two_d_e_one(G);
	}
	else if(d_v == 2.5 && d_e == 1){
		printf("Randomizing a given hypergraph with (d_v, d_e) = (%d, %d).\n", int(d_v), int(d_e));
		randG = randomizing_d_v_two_five_d_e_one(G);
	}
	else{
		printf("Error: Given pair (d_v, d_e) is not defined.\n");
		printf("The pair (d_v, d_e) should be (0, 0), (1, 0), (2, 0), (2.5, 0), (0, 1), (1, 1), (2, 1), or (2.5, 1).\n");
		exit(0);
	}

	return randG;
}

PYBIND11_MODULE(hyper_dk_series, m) {
  m.doc() = "hyper dK-series";

  pybind11::class_<HyperGraph>(m, "HyperGraph")
  	.def(pybind11::init<>())
  	.def_readwrite("V", &HyperGraph::V)
  	.def_readwrite("E", &HyperGraph::E)
  	.def_readwrite("elist", &HyperGraph::elist)
  	.def_readwrite("vlist", &HyperGraph::vlist)
  	.def("read_hypergraph", &HyperGraph::read_hypergraph)
  	;

  m.def("randomizing_hypergraph", &randomizing_hypergraph, "Randomizing a given hypergraph.");
}
