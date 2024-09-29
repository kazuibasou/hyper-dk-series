#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <set>
#include "hypergraph.h"
#include "rewiring.h"

int write_hypergraph(const char *graphname, const std::string d_v, const std::string d_e, 
	const int k, HyperGraph rand_G){

	const char *dir = "../../rand_hypergraph/";

	FILE *f1;
	std::string f1path = std::string(dir) + graphname + "_" + d_v + "_" + d_e + "_" + std::to_string(k) + "_nverts.txt";
	f1 = fopen(f1path.c_str(), "w");
	if(f1 == NULL) {
		printf("Error: Could not open file named %s_nverts.txt.\n", graphname);
		exit(0);
	}

	for(int m:rand_G.E){
		fprintf(f1, "%d\n", int(rand_G.vlist[m].size()));
	}

	fclose(f1);

	FILE *f2;
	std::string f2path = std::string(dir) + graphname + "_" + d_v + "_" + d_e + "_" + std::to_string(k) + "_hyperedges.txt";
	f2 = fopen(f2path.c_str(), "w");
	if(f2 == NULL) {
		printf("Error: Could not open file named %s_hyperedges.txt.\n", graphname);
		exit(0);
	}

	for(int m:rand_G.E){
		for(int v:rand_G.vlist[m]){
			fprintf(f2, "%d\n", v);
		}
	}

	fclose(f2);

	printf("Wrote the %d-th hypergraph randomized with (d_v, d_e) = (%s, %s) at ./hyper_dk_series/rand_hypergraph/.\n\n", k, d_v.c_str(), d_e.c_str());

	return 0;
}

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

	int iv, v, im, m;
	for(int i=0; i<B_M; ++i){
		iv = randN(mt);
		v = randG.V[iv];
		im = randM(mt);
		m = randG.E[im];
		randG.elist[v].push_back(m);
		randG.vlist[m].push_back(v);
	}

	for(int v:randG.V){
		std::sort(randG.elist[v].begin(),randG.elist[v].end());
	}
	for(int m:randG.E){
		std::sort(randG.vlist[m].begin(),randG.vlist[m].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 0).\n\n");

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

	int v, im, m;
	while(int(stublist.size()) > 0){
		v = stublist.back();
		stublist.pop_back();
		im = randM(mt);
		m = randG.E[im];
		randG.elist[v].push_back(m);
		randG.vlist[m].push_back(v);
	}

	for(int v:randG.V){
		std::sort(randG.elist[v].begin(),randG.elist[v].end());
	}
	for(int m:randG.E){
		std::sort(randG.vlist[m].begin(),randG.vlist[m].end());
	}

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 0).\n\n");

	return randG;
}

HyperGraph randomizing_d_v_two_d_e_zero(HyperGraph G){

	HyperGraph randG;
	
	randG = randomizing_d_v_one_d_e_zero(G);

	randG = targeting_rewiring_d_v_two(G, randG);

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 0).\n\n");

	return randG;
}

HyperGraph randomizing_d_v_two_five_d_e_zero(HyperGraph G){

	HyperGraph randG;

	randG = randomizing_d_v_two_d_e_zero(G);

	randG = targeting_rewiring_d_v_two_five(G, randG);
	
	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 0).\n\n");

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

	int iv, v, m;
	while(int(stublist.size()) > 0){
		iv = randN(mt);
		v = randG.V[iv];
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

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 1).\n\n");

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

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 1).\n\n");

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

	printf("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 1).\n\n");

	return randG;
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
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_zero_d_e_zero(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "1" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_one_d_e_zero(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_two_d_e_zero(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2.5" && d_e == "0"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_two_five_d_e_zero(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "0" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_zero_d_e_one(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "1" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_one_d_e_one(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_two_d_e_one(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else if(d_v == "2.5" && d_e == "1"){
		for(int k=1; k<=num_gen; ++k){
			printf("Started %d-th generation of a randomized hypergraph with (d_v, d_e) = (%s, %s).\n\n", k, d_v.c_str(), d_e.c_str());
			std::chrono::system_clock::time_point  start, end;
			start = std::chrono::system_clock::now();
			HyperGraph rand_G = randomizing_d_v_two_five_d_e_one(G);
			end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
			printf("Generation time: %lf [milliseconds].\n", elapsed);
			write_hypergraph(graphname, d_v, d_e, k, rand_G);
		}
	}
	else{
		printf("Error: Given pair (d_v, d_e) is not defined.\n");
		printf("The pair (d_v, d_e) should be (0, 0), (1, 0), (2, 0), (2.5, 0), (0, 1), (1, 1), (2, 1), or (2.5, 1).\n");
		exit(0);
	}

	return 0;
}