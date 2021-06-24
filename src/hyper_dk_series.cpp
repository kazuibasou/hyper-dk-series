#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include "basic_function.h"
#include "hypergraph.h"
#include "rewiring.h"

int write_hypergraph(const char *graphname, const std::string d_v, const std::string d_e, 
	const int k, HyperGraph rand_G){

	const char *dir = "../rand_hypergraph/";

	FILE *f1;
	std::string f1path = std::string(dir) + graphname + "_d_v_" + d_v + "_d_e_" + d_e + "_n_" + std::to_string(k) + "_nverts.txt";
	f1 = fopen(f1path.c_str(), "w");
	if(f1 == NULL) {
		printf("Error: Could not open file named %s_nverts.txt.\n", graphname);
		exit(0);
	}

	for(int m=0; m<rand_G.M; ++m){
		fprintf(f1, "%d\n", int(rand_G.vlist[m].size()));
	}

	fclose(f1);

	FILE *f2;
	std::string f2path = std::string(dir) + graphname + "_d_v_" + d_v + "_d_e_" + d_e + "_n_" + std::to_string(k) + "_hyperedges.txt";
	f2 = fopen(f2path.c_str(), "w");
	if(f2 == NULL) {
		printf("Error: Could not open file named %s_hyperedges.txt.\n", graphname);
		exit(0);
	}

	for(int m=0; m<rand_G.M; ++m){
		for(int v:rand_G.vlist[m]){
			fprintf(f2, "%d\n", v);
		}
	}

	fclose(f2);

	printf("Wrote the randomized hypergraph with (d_v, d_e) = (%s, %s) at ./hyper_dk_series/rand_hypergraph/.\n\n", d_v.c_str(), d_e.c_str());

	return 0;
}

HyperGraph randomizing_d_v_zero_d_e_zero(HyperGraph G){

	int N = G.N;
	int M = G.M;

	int B_M = 0;
	for(int v=0; v<N; ++v){
		B_M += int(G.elist[v].size());
	}

	int v, m;
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randN(0, N-1);
    std::uniform_int_distribution<int> randM(0, M-1);

    HyperGraph rand_G;
	rand_G.N = N;
	rand_G.M = M;
	rand_G.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	rand_G.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	for(int i=0; i<B_M; ++i){
		v = randN(mt);
		m = randM(mt);
		rand_G.elist[v].push_back(m);
		rand_G.vlist[m].push_back(v);
	}

	for(int i=0; i<rand_G.N; ++i){
		std::sort(rand_G.elist[i].begin(),rand_G.elist[i].end());
	}
	for(int i=0; i<rand_G.M; ++i){
		std::sort(rand_G.vlist[i].begin(),rand_G.vlist[i].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 0).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_one_d_e_zero(HyperGraph G){

	int N = G.N;
	int M = G.M;

	std::vector<int> stublist;
	for(int v=0; v<N; ++v){
		int d = int(G.elist[v].size());
		for(int i=0; i<d; ++i){
			stublist.push_back(v);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(stublist.begin(), stublist.end(), mt);

	std::uniform_int_distribution<int> randM(0, M-1);

	HyperGraph rand_G;
	rand_G.N = N;
	rand_G.M = M;
	rand_G.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	rand_G.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(stublist.size()) > 0){
		v = stublist.back();
		stublist.pop_back();
		m = randM(mt);
		rand_G.elist[v].push_back(m);
		rand_G.vlist[m].push_back(v);
	}

	for(int i=0; i<rand_G.N; ++i){
		std::sort(rand_G.elist[i].begin(),rand_G.elist[i].end());
	}
	for(int i=0; i<rand_G.M; ++i){
		std::sort(rand_G.vlist[i].begin(),rand_G.vlist[i].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 0).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_zero_d_e_one(HyperGraph G){

	int N = G.N;
	int M = G.M;

	std::vector<int> stublist;
	for(int m=0; m<M; ++m){
		int s = int(G.vlist[m].size());
		for(int i=0; i<s; ++i){
			stublist.push_back(m);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(stublist.begin(), stublist.end(), mt);

	std::uniform_int_distribution<int> randN(0, N-1);

	HyperGraph rand_G;
	rand_G.N = N;
	rand_G.M = M;
	rand_G.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	rand_G.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(stublist.size()) > 0){
		v = randN(mt);
		m = stublist.back();
		stublist.pop_back();
		rand_G.elist[v].push_back(m);
		rand_G.vlist[m].push_back(v);
	}

	for(int i=0; i<rand_G.N; ++i){
		std::sort(rand_G.elist[i].begin(),rand_G.elist[i].end());
	}
	for(int i=0; i<rand_G.M; ++i){
		std::sort(rand_G.vlist[i].begin(),rand_G.vlist[i].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 1).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_one_d_e_one(HyperGraph G){
	
	int N = G.N;
	int M = G.M;

	std::vector<int> node_stublist;
	for(int v=0; v<N; ++v){
		int d = int(G.elist[v].size());
		for(int i=0; i<d; ++i){
			node_stublist.push_back(v);
		}
	}

	std::vector<int> edge_stublist;
	for(int m=0; m<M; ++m){
		int s = int(G.vlist[m].size());
		for(int i=0; i<s; ++i){
			edge_stublist.push_back(m);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(node_stublist.begin(), node_stublist.end(), mt);
	std::shuffle(edge_stublist.begin(), edge_stublist.end(), mt);

	HyperGraph rand_G;
	rand_G.N = N;
	rand_G.M = M;
	rand_G.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	rand_G.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(node_stublist.size()) > 0 && int(edge_stublist.size()) > 0){
		v = node_stublist.back();
		node_stublist.pop_back();
		m = edge_stublist.back();
		edge_stublist.pop_back();
		rand_G.elist[v].push_back(m);
		rand_G.vlist[m].push_back(v);
	}

	for(int i=0; i<rand_G.N; ++i){
		std::sort(rand_G.elist[i].begin(),rand_G.elist[i].end());
	}
	for(int i=0; i<rand_G.M; ++i){
		std::sort(rand_G.vlist[i].begin(),rand_G.vlist[i].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 1).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_two_d_e_zero(HyperGraph G){

	HyperGraph rand_G;
	
	rand_G = randomizing_d_v_one_d_e_zero(G);

	rand_G = targeting_rewiring_d_v_two(G, rand_G);

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 0).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_two_d_e_one(HyperGraph G){

	HyperGraph rand_G;

	rand_G = randomizing_d_v_one_d_e_one(G);

	rand_G = targeting_rewiring_d_v_two(G, rand_G);
	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 1).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_two_five_d_e_zero(HyperGraph G){

	HyperGraph rand_G;

	rand_G = randomizing_d_v_two_d_e_zero(G);

	rand_G = targeting_rewiring_d_v_two_five(G, rand_G);
	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 0).\n");

	return rand_G;
}

HyperGraph randomizing_d_v_two_five_d_e_one(HyperGraph G){

	HyperGraph rand_G;

	rand_G = randomizing_d_v_two_d_e_one(G);
	
	rand_G = targeting_rewiring_d_v_two_five(G, rand_G);
	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 1).\n");

	return rand_G;
}

int main(int argc,char *argv[]){
	if(argc != 5){
		printf("Please input following:\n");
		printf("./hyper_dk_series (name of hypergraph) (value of d_v) (value of d_e) (number of generation)\n");
		exit(0);
	}
	const char *graphname = argv[1];
	const std::string d_v = argv[2];
	const std::string d_e = argv[3];
	const int num_gen = std::stoi(argv[4]);

	HyperGraph G;
	G.read_hypergraph(graphname);

	if(d_v == "0" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_zero_d_e_zero(G);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "1" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_one_d_e_zero(G);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_two_d_e_zero(G);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2.5" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_two_five_d_e_zero(G);
			write_hypergraph(graphname, "2_5", d_e, k, rand_G);
		}
	}
	else if(d_v == "0" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_zero_d_e_one(G);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "1" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_one_d_e_one(G);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_two_d_e_one(G);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2.5" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("%d generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n", k, d_v.c_str(), d_e.c_str());
			HyperGraph rand_G = randomizing_d_v_two_five_d_e_one(G);
			write_hypergraph(graphname, "2_5", d_e, k, rand_G);
		}
	}
	else{
		printf("Error: Given pair (d_v, d_e) is not defined.\n");
		printf("The pair (d_v, d_e) should be (0, 0), (1, 0), (2, 0), (2.5, 0), (0, 1), (1, 1), (2, 1), or (2.5, 1).\n");
		exit(0);
	}

	return 0;
}