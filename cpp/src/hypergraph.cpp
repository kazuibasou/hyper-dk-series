#include <iostream>
#include <vector>
#include <random>
#include <deque>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "hypergraph.h"

HyperGraph::HyperGraph(){

	V = std::vector<int>();
	E = std::vector<int>();
	elist = std::unordered_map<int, std::vector<int>>();
	vlist = std::unordered_map<int, std::vector<int>>();
}

HyperGraph::~HyperGraph(){

	std::vector<int>().swap(V);
	std::vector<int>().swap(E);
	std::unordered_map<int, std::vector<int>>().swap(elist);
	std::unordered_map<int, std::vector<int>>().swap(vlist);
}

int HyperGraph::read_hypergraph(const char *hypergraph){

	const char *dir = "../../data/";

	FILE *f1;
	std::string f1path = std::string(dir) + hypergraph + "_nverts.txt";
	f1 = fopen(f1path.c_str(), "r");
	if(f1 == NULL) {
		printf("Error: Could not open file named %s_nverts.txt.\n", hypergraph);
		exit(0);
	}

	FILE *f2;
	std::string f2path = std::string(dir) + hypergraph + "_hyperedges.txt";
	f2 = fopen(f2path.c_str(), "r");
	if(f2 == NULL) {
		printf("Error: Could not open file named %s_hyperedges.txt.\n", hypergraph);
		exit(0);
	}

	int nv, v;
	int m = 0;
	V = std::vector<int>();
	E = std::vector<int>();
	elist = std::unordered_map<int, std::vector<int>>();
	vlist = std::unordered_map<int, std::vector<int>>();
	while(fscanf(f1, "%d", &nv) != EOF) {
		std::vector<int> e;
		for(int i=0; i<nv; ++i){
			auto fs = fscanf(f2, "%d", &v);
			if(fs != EOF){
				e.push_back(v);
			}
			else{
				std::string s = std::string(hypergraph) + "_hyperedges.txt";
				printf("Error: Failed to read a list of hyperedges named %s.\n", s.c_str());
				exit(0);
			}
		}

		vlist[m] = e;
		E.push_back(m);
		for(int v:e){
			if(elist.find(v) == elist.end()){
				elist[v] = std::vector<int>();
			}
			elist[v].push_back(m);

			if(std::find(V.begin(), V.end(), v) == V.end()){
				V.push_back(v);
			}
		}

		m += 1;
	}
	
	fclose(f1);
	fclose(f2);

	int N = int(V.size());
	int M = int(E.size());

	for(int i=0; i<N; ++i){
		std::sort(elist[i].begin(),elist[i].end());
	}
	for(int i=0; i<M; ++i){
		std::sort(vlist[i].begin(),vlist[i].end());
	}

	printf("The given hypergraph named %s was successfully read.\n", hypergraph);
	printf("Number of nodes: %d\n", N);
	printf("Number of hyperedges: %d\n\n", M);

	return 0;
}

int add_node_to_hyperedge(HyperGraph &G, const int &v, const int &m){
	if(G.elist.find(v) == G.elist.end()){
		printf("Error: Given node is not found.\n");
		exit(0);
	}
	if(G.vlist.find(m) == G.vlist.end()){
		printf("Error: Given hyperedge is not found.\n");
		exit(0);
	}

	G.vlist[m].push_back(v);
	std::sort(G.vlist[m].begin(),G.vlist[m].end());
	G.elist[v].push_back(m);
	std::sort(G.elist[v].begin(),G.elist[v].end());
	
	return 0;
}

int remove_node_from_hyperedge(HyperGraph &G, const int &v, const int &m){
	if(G.elist.find(v) == G.elist.end()){
		printf("Error: Given node is not found.\n");
		exit(0);
	}
	if(G.vlist.find(m) == G.vlist.end()){
		printf("Error: Given hyperedge is not found.\n");
		exit(0);
	}

	auto itr1 = std::find(G.elist[v].begin(), G.elist[v].end(), m);
	if(itr1 == G.elist[v].end()){
		printf("Error: Node %d is not included in hyperedge %d\n", v, m);
	}
	G.elist[v].erase(itr1);

	auto itr2 = std::find(G.vlist[m].begin(), G.vlist[m].end(), v);
	if(itr2 == G.vlist[m].end()){
		printf("Error: Node %d does not belong to hyperedge %d\n", v, m);
	}
	G.vlist[m].erase(itr2);

	return 0;
}

int calc_node_degree(HyperGraph &G, std::unordered_map<int, int> &node_degree){

	node_degree.clear();
	for(int v:G.V){
		node_degree[v] = int(G.elist[v].size());
	}

	return 0;
}

int calc_maximum_node_degree(HyperGraph &G, int &max_node_deg){
	int k;
	max_node_deg = 0;
	for(int v:G.V){
		k = int(G.elist[v].size());
		if(k > max_node_deg){
			max_node_deg = k;
		}
	}

	return 0;
}

int calc_num_jnt_node_deg(HyperGraph &G, Matrix<int> &num_jnt_node_deg){

	num_jnt_node_deg.clear();
	int s, i, j, u, v, k, l;
	for(int m:G.E){
		s = int(G.vlist[m].size());
		for(i=0; i<s-1; ++i){
			u = G.vlist[m][i];
			k = int(G.elist[u].size());
			for(j=i+1; j<s; ++j){
				v = G.vlist[m][j];
				l = int(G.elist[v].size());
				num_jnt_node_deg.add(k, l, 1);
				num_jnt_node_deg.add(l, k, 1);
			}
		}
	}

	return 0;
}

int calc_node_redundancy_coefficient(HyperGraph &G, std::unordered_map<int, double> &node_redun_coeff){
	node_redun_coeff.clear();
	int d, i, j, m1, m2;

	for(int v:G.V){
		d = int(G.elist[v].size());
		if(d < 2){continue;}

		double rc = 0.0;
		for(i=0; i<d-1; ++i){
			m1 = G.elist[v][i];
			for(j=i+1; j<d; ++j){
				m2 = G.elist[v][j];
				if(m1 == m2){continue;}
				std::set<int> s1, s3;
				std::set<int> s2 = {v};
				std::set_intersection(G.vlist[m1].begin(), G.vlist[m1].end(), G.vlist[m2].begin(), G.vlist[m2].end(), std::inserter(s1, s1.end()));
				std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));

				if(int(s3.size()) > 0){
					rc += 2.0;
				}
			}
		}

		node_redun_coeff[v] = double(rc)/(d*(d-1));
	}

	return 0;
}

int calc_degree_dependent_node_redundancy_coefficient(HyperGraph &G, std::vector<double> &degree_node_redun_coeff){
	
	std::unordered_map<int, double> node_redun_coeff;
	calc_node_redundancy_coefficient(G, node_redun_coeff);

	int max_node_deg;
	calc_maximum_node_degree(G, max_node_deg);

	degree_node_redun_coeff = std::vector<double>(max_node_deg+1, 0.0);
	std::vector<int> N_k(max_node_deg+1, 0);

	for(int v:G.V){
		int k = int(G.elist[v].size());
		N_k[k] += 1;
		degree_node_redun_coeff[k] += node_redun_coeff[v];
	}

	for(int k=1; k<int(N_k.size()); ++k){
		if(N_k[k] > 0){
			degree_node_redun_coeff[k] = double(degree_node_redun_coeff[k])/N_k[k];
		}
	}

	return 0;
}

int calc_hyperedge_size(HyperGraph &G, std::unordered_map<int, int> &hyperedge_size){

	hyperedge_size.clear();
	for(int m:G.E){
		hyperedge_size[m] = int(G.vlist[m].size());
	}

	return 0;
}