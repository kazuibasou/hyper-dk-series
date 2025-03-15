#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <math.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <set>
#include "hypergraph.h"

HyperGraph null_model(HyperGraph G, const bool test){

	int N = G.N;
	int M = G.M;

	std::vector<int> node_stublist;
	for(int v=0; v<N; ++v){
		int k = int(G.elist[v].size());
		for(int i=0; i<k; ++i){
			node_stublist.push_back(v);
		}
	}

	std::vector<int> edge_stublist;
	for(int m=0; m<M; ++m){
		int s = int(G.E[m].size());
		for(int i=0; i<s; ++i){
			edge_stublist.push_back(m);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(node_stublist.begin(), node_stublist.end(), mt);
	std::shuffle(edge_stublist.begin(), edge_stublist.end(), mt);

	HyperGraph randG;
	randG.N = N;
	randG.M = M;
	randG.elist = std::vector<std::vector<std::pair<int, double>>>(N, std::vector<std::pair<int, double>>());
	randG.E = std::vector<std::vector<std::pair<int, double>>>(M, std::vector<std::pair<int, double>>());

	int v, m;
	while(int(node_stublist.size()) > 0 && int(edge_stublist.size()) > 0){
		v = node_stublist.back();
		node_stublist.pop_back();
		m = edge_stublist.back();
		edge_stublist.pop_back();
		randG.elist[v].push_back({m, 1});
		randG.E[m].push_back({v, 1});
	}

	if(test){
		if(G.N != randG.N){
			printf("Error: Number of nodes has not been preserved.\n");
			exit(0);
		}
		
		if(G.M != randG.M){
			printf("Error: Number of hyperedges has not been preserved.\n");
			exit(0);
		}

		for(int v=0; v<G.N; ++v){
			if(int(G.elist[v].size()) != int(randG.elist[v].size())){
				printf("Error: Node's richness has not been preserved.\n");
				exit(0);
			}
		}

		for(int m=0; m<G.M; ++m){
			if(int(G.E[m].size()) != int(randG.E[m].size())){
				printf("Error: Hyperedge's size has not been preserved.\n");
				exit(0);
			}
		}
	}

	return randG;
}

HyperGraph extract_size_s_sub_hypergraph(HyperGraph G, const int s){

	HyperGraph G_s;
	std::unordered_map<int, int> node_index;

	int m, v;
	double w;
	int N = 0;
	int M = 0;
	for(m=0; m<G.M; ++m){
		if(int(G.E[m].size()) != s){continue;}

		std::vector<std::pair<int, double>> e;
		for(std::pair<int, double> p:G.E[m]){
			v = p.first;
			w = p.second;
			if(node_index.find(v) == node_index.end()){
				node_index[v] = N;
				N += 1;
				G_s.elist.resize(N);
			}
			e.push_back({node_index[v], w});
			G_s.elist[node_index[v]].push_back({M, w});
		}

		G_s.E.push_back(e);
		M += 1;
	}

	G_s.N = N;
	G_s.M = M;

	return G_s;
}

int calc_size_dependent_normalized_topological_rich_club_coefficient(HyperGraph G, 
	const int num_null, const double alpha, const bool test, 
	std::map<int, std::map<int, double>> &normalized_node_rcc, 
	std::map<int, std::map<int, int>> &rich_club_test){

	std::set<int> hyperedge_size_set;
	for(int m=0; m<G.M; ++m){
		hyperedge_size_set.insert(int(G.E[m].size()));
	}

	normalized_node_rcc.clear();
	rich_club_test.clear();

	for(int s:hyperedge_size_set){
		HyperGraph G_s = extract_size_s_sub_hypergraph(G, s);

		std::set<int> node_richness_set;
		for(int v=0; v<G_s.N; ++v){
			node_richness_set.insert(int(G_s.elist[v].size()));
		}

		std::unordered_map<int, double> G_s_node_rcc;
		G_s.calc_topological_rich_club_coefficient(G_s_node_rcc);

		std::unordered_map<int, std::vector<double>> null_node_rccs;
		for(int r:node_richness_set){
			null_node_rccs[r] = std::vector<double>();
		}

		for(int i=0; i<num_null; ++i){
			HyperGraph randG_s = null_model(G_s, test);
			std::unordered_map<int, double> null_node_rcc;
			randG_s.calc_topological_rich_club_coefficient(null_node_rcc);
			
			for(int r:node_richness_set){
				null_node_rccs[r].push_back(double(null_node_rcc[r]));
			}
		}

		normalized_node_rcc[s] = std::map<int, double>();
		rich_club_test[s] = std::map<int, int>();

		std::unordered_map<int, double> p_val;
		for(int r:node_richness_set){
			p_val[r] = 0.0;
		}

		int m = 0;
		for(int r:node_richness_set){
			double ave_null_node_rcc;
			double std_null_node_rcc;
			calc_sample_mean(null_node_rccs[r], ave_null_node_rcc);
			calc_unbiased_standard_deviation(null_node_rccs[r], std_null_node_rcc);

			if(G_s_node_rcc[r] >= 5){
				normalized_node_rcc[s][r] = double(G_s_node_rcc[r])/ave_null_node_rcc;
				//rcc_var[r] = double(std_null_node_rcc)/ave_null_node_rcc;
				permutation_testing(G_s_node_rcc[r], null_node_rccs[r], p_val[r]);
				m += 1;
			}
			else{
				normalized_node_rcc[s][r] = -1;
				p_val[r] = -1;
			}
		}

		double corrected_alpha = double(alpha)/m;
		for(int r:node_richness_set){
			if(p_val[r] >= 0){
				if(p_val[r] < corrected_alpha){
					rich_club_test[s][r] = 1;
				}
				else{
					rich_club_test[s][r] = 0;
				}
			}
			else{
				rich_club_test[s][r] = -1;
			}
		}
	}

	return 0;
}

int write_rich_club_coefficient(const char *hypergraph_name,
	const int num_null, const double alpha, 
	const std::map<int, std::map<int, double>> normalized_node_rcc,
	const std::map<int, std::map<int, int>> rich_club_test){

	const char *dir_result = "../data/result/";
	const std::string expfilepath = std::string(dir_result) + std::string(hypergraph_name) + "_size_dependent_topological_rich_club_coefficient.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n", expfilepath.c_str());
		return -1;
	}

	fprintf(ef,"Target graph: %s\n\n", hypergraph_name);

	fprintf(ef,"Number of null model generations: %d\n\n", num_null);
	fprintf(ef,"Alpha for statistical testing: %lf\n\n", alpha);

	fprintf(ef,"Hyperedge size-dependent rich club coefficient.\n\n");

	for(auto elm1:normalized_node_rcc){
		int s = elm1.first;
		fprintf(ef,"Hyperedge's size: %d\n", s);

		fprintf(ef,"Normalized rich-club coefficient:\n");
		for(auto elm2:normalized_node_rcc.at(s)){
			int r = elm2.first;
			if(normalized_node_rcc.at(s).at(r) >= 0){
				fprintf(ef,"%d %lf, ", r, normalized_node_rcc.at(s).at(r));
			}
		}
		fprintf(ef, "\n");
		fprintf(ef,"Topological rich-club test:\n");
		for(auto elm2:rich_club_test.at(s)){
			int r = elm2.first;
			fprintf(ef,"%d %d, ", r, rich_club_test.at(s).at(r));
		}
		fprintf(ef, "\n\n");
	}

	fclose(ef);

	return 0;
}

int main(int argc,char *argv[]){
	if(argc != 2){
		printf("Please input following: ./calc_hyper_rich_club_coefficient (name of graph) (type of node's richness)\n");
		exit(0);
	}
	const char *hypergraph_name = argv[1];

	srand((unsigned) time(NULL));
	
	HyperGraph G;
	G.read_hypergraph(hypergraph_name);
	
	int num_null = 10000;
	double alpha = 0.005;
	bool test = false;
	std::map<int, std::map<int, double>> normalized_node_rcc;
	std::map<int, std::map<int, int>> rich_club_test;

	calc_size_dependent_normalized_topological_rich_club_coefficient(G, num_null, alpha, test, normalized_node_rcc, rich_club_test);

	write_rich_club_coefficient(hypergraph_name, num_null, alpha, normalized_node_rcc, rich_club_test);

	return 0;
}