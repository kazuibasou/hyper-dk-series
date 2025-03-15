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

	int N = int(G.V.size());
	int M = int(G.E.size());

	std::vector<int> node_stublist;
	for(int v: G.V){
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

int calc_normalized_topological_rich_club_coefficient(HyperGraph G, const int num_null, 
	const double alpha, const bool test, std::map<int, double> &normalized_node_rcc, 
	std::map<int, double> &rcc_var, std::map<int, int> &rich_club_test){

	std::vector<int> node_richness;
	G.calc_node_degree(node_richness);

	std::set<int> node_richness_set;
	for(int v=0; v<G.N; ++v){
		node_richness_set.insert(node_richness[v]);
	}

	std::unordered_map<int, double> G_node_rcc;
	G.calc_topological_rich_club_coefficient(G_node_rcc);

	std::unordered_map<int, std::vector<double>> null_node_rccs;
	for(int r:node_richness_set){
		null_node_rccs[r] = std::vector<double>();
	}
	
	for(int i=0; i<num_null; ++i){
		HyperGraph randG = null_model(G, test);
		std::unordered_map<int, double> null_node_rcc;
		randG.calc_topological_rich_club_coefficient(null_node_rcc);

		for(int r:node_richness_set){
			null_node_rccs[r].push_back(double(null_node_rcc[r]));
		}
	}

	normalized_node_rcc.clear();
	rcc_var.clear();
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

		if(G_node_rcc[r] >= 5){
			normalized_node_rcc[r] = double(G_node_rcc[r])/ave_null_node_rcc;
			rcc_var[r] = double(std_null_node_rcc)/ave_null_node_rcc;
			permutation_testing(G_node_rcc[r], null_node_rccs[r], p_val[r]);
			m += 1;
		}
		else{
			normalized_node_rcc[r] = -1;
			p_val[r] = -1;
		}
	}

	double corrected_alpha = double(alpha)/m;
	for(int r:node_richness_set){
		if(p_val[r] >= 0){
			if(p_val[r] < corrected_alpha){
				rich_club_test[r] = 1;
			}
			else{
				rich_club_test[r] = 0;
			}
		}
		else{
			rich_club_test[r] = -1;
		}
	}

	return 0;
}

int write_rich_club_coefficient(const char *hypergraph_name, const int num_null, 
	const double alpha, const std::map<int, double> &normalized_node_rcc, 
	const std::map<int, double> &rcc_var, const std::map<int, int> &rich_club_test){

	const char *dir_result = "../data/result/";
	const std::string expfilepath = std::string(dir_result) + std::string(hypergraph_name) + "_topological_rich_club_coefficient.txt";
	FILE *ef = fopen(expfilepath.c_str(),"w");
	if(ef == NULL) {
		printf("Could not open file named %s.\n", expfilepath.c_str());
		return -1;
	}

	fprintf(ef,"Target graph: %s\n\n", hypergraph_name);

	fprintf(ef,"Number of null generations: %d\n\n", num_null);
	fprintf(ef,"Alpha for statistical testing: %lf\n\n", alpha);

	fprintf(ef,"Normalized topological rich-club coefficient:\n");
	for(auto elm:normalized_node_rcc){
		if(elm.second >= 0){
			fprintf(ef,"%d %lf, ", elm.first, elm.second);
		}
	}
	fprintf(ef, "\n\n");

	fprintf(ef,"Standard deviation of normalized topological rich-club coefficient:\n");
	for(auto elm:rcc_var){
		if(elm.second >= 0){
			fprintf(ef,"%d %lf, ", elm.first, elm.second);
		}
	}
	fprintf(ef, "\n\n");

	fprintf(ef,"Topological rich-club test:\n");
	for(auto elm:rich_club_test){
		fprintf(ef,"%d %d, ", elm.first, elm.second);
	}
	fprintf(ef, "\n");

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
	std::map<int, double> normalized_node_rcc;
	std::map<int, double> rcc_var;
	std::map<int, int> rich_club_test;

	calc_normalized_topological_rich_club_coefficient(G, num_null, alpha, test, normalized_node_rcc, rcc_var, rich_club_test);

	write_rich_club_coefficient(hypergraph_name, num_null, alpha, normalized_node_rcc, rcc_var, rich_club_test);

	return 0;
}