#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <deque>
#include <cmath>
#include <limits>
#include <numeric>
#include <set>
#include "basic_function.h"
#include "bipartite_graph.h"
#include "bipartite_generator.h"
#include "bipartite_rewiring.h"

BipartiteGraph BipartitedKseries::node_zeroK_edge_zeroK_generator(BipartiteGraph B){

	B.calc_sum_node_degree();
	int N = B.N;
	int M = B.M;
	int B_M = B.sum_node_degree;

	int v, m;
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randN(0, N-1);
    std::uniform_int_distribution<int> randM(0, M-1);

    BipartiteGraph node_zeroK_edge_zeroK;
	node_zeroK_edge_zeroK.N = N;
	node_zeroK_edge_zeroK.M = M;
	node_zeroK_edge_zeroK.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	node_zeroK_edge_zeroK.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	for(int i=0; i<B_M; ++i){
		v = randN(mt);
		m = randM(mt);
		node_zeroK_edge_zeroK.elist[v].push_back(m);
		node_zeroK_edge_zeroK.vlist[m].push_back(v);
	}

	for(int i=0; i<node_zeroK_edge_zeroK.N; ++i){
		std::sort(node_zeroK_edge_zeroK.elist[i].begin(),node_zeroK_edge_zeroK.elist[i].end());
	}
	for(int i=0; i<node_zeroK_edge_zeroK.M; ++i){
		std::sort(node_zeroK_edge_zeroK.vlist[i].begin(),node_zeroK_edge_zeroK.vlist[i].end());
	}

	return node_zeroK_edge_zeroK;
}

BipartiteGraph BipartitedKseries::node_oneK_edge_zeroK_generator(BipartiteGraph B){

	int N = B.N;
	int M = B.M;

	std::vector<int> stublist;
	for(int v=0; v<N; ++v){
		int d = int(B.elist[v].size());
		for(int i=0; i<d; ++i){
			stublist.push_back(v);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(stublist.begin(), stublist.end(), mt);

	std::uniform_int_distribution<int> randM(0, M-1);

	BipartiteGraph node_oneK_edge_zeroK;
	node_oneK_edge_zeroK.N = N;
	node_oneK_edge_zeroK.M = M;
	node_oneK_edge_zeroK.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	node_oneK_edge_zeroK.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(stublist.size()) > 0){
		v = stublist.back();
		stublist.pop_back();
		m = randM(mt);
		node_oneK_edge_zeroK.elist[v].push_back(m);
		node_oneK_edge_zeroK.vlist[m].push_back(v);
	}

	for(int i=0; i<node_oneK_edge_zeroK.N; ++i){
		std::sort(node_oneK_edge_zeroK.elist[i].begin(),node_oneK_edge_zeroK.elist[i].end());
	}
	for(int i=0; i<node_oneK_edge_zeroK.M; ++i){
		std::sort(node_oneK_edge_zeroK.vlist[i].begin(),node_oneK_edge_zeroK.vlist[i].end());
	}

	return node_oneK_edge_zeroK;
}

BipartiteGraph BipartitedKseries::node_zeroK_edge_oneK_generator(BipartiteGraph B){
	
	int N = B.N;
	int M = B.M;

	std::vector<int> stublist;
	for(int m=0; m<M; ++m){
		int s = int(B.vlist[m].size());
		for(int i=0; i<s; ++i){
			stublist.push_back(m);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(stublist.begin(), stublist.end(), mt);

	std::uniform_int_distribution<int> randN(0, N-1);

	BipartiteGraph node_zeroK_edge_oneK;
	node_zeroK_edge_oneK.N = N;
	node_zeroK_edge_oneK.M = M;
	node_zeroK_edge_oneK.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	node_zeroK_edge_oneK.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(stublist.size()) > 0){
		v = randN(mt);
		m = stublist.back();
		stublist.pop_back();
		node_zeroK_edge_oneK.elist[v].push_back(m);
		node_zeroK_edge_oneK.vlist[m].push_back(v);
	}

	for(int i=0; i<node_zeroK_edge_oneK.N; ++i){
		std::sort(node_zeroK_edge_oneK.elist[i].begin(),node_zeroK_edge_oneK.elist[i].end());
	}
	for(int i=0; i<node_zeroK_edge_oneK.M; ++i){
		std::sort(node_zeroK_edge_oneK.vlist[i].begin(),node_zeroK_edge_oneK.vlist[i].end());
	}

	return node_zeroK_edge_oneK;
}

BipartiteGraph BipartitedKseries::node_oneK_edge_oneK_generator(BipartiteGraph B){
	
	int N = B.N;
	int M = B.M;

	std::vector<int> node_stublist;
	for(int v=0; v<N; ++v){
		int d = int(B.elist[v].size());
		for(int i=0; i<d; ++i){
			node_stublist.push_back(v);
		}
	}

	std::vector<int> edge_stublist;
	for(int m=0; m<M; ++m){
		int s = int(B.vlist[m].size());
		for(int i=0; i<s; ++i){
			edge_stublist.push_back(m);
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(node_stublist.begin(), node_stublist.end(), mt);
	std::shuffle(edge_stublist.begin(), edge_stublist.end(), mt);

	BipartiteGraph node_oneK_edge_oneK;
	node_oneK_edge_oneK.N = N;
	node_oneK_edge_oneK.M = M;
	node_oneK_edge_oneK.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	node_oneK_edge_oneK.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(node_stublist.size()) > 0 && int(edge_stublist.size()) > 0){
		v = node_stublist.back();
		node_stublist.pop_back();
		m = edge_stublist.back();
		edge_stublist.pop_back();
		node_oneK_edge_oneK.elist[v].push_back(m);
		node_oneK_edge_oneK.vlist[m].push_back(v);
	}

	for(int i=0; i<node_oneK_edge_oneK.N; ++i){
		std::sort(node_oneK_edge_oneK.elist[i].begin(),node_oneK_edge_oneK.elist[i].end());
	}
	for(int i=0; i<node_oneK_edge_oneK.M; ++i){
		std::sort(node_oneK_edge_oneK.vlist[i].begin(),node_oneK_edge_oneK.vlist[i].end());
	}

	return node_oneK_edge_oneK;
}

BipartiteGraph BipartitedKseries::bipartite_degree_correlation_generator(BipartiteGraph B){

	int N = B.N;
	int M = B.M;

	std::vector<std::pair<int, int>> stubpairlist;
	for(int m=0; m<M; ++m){
		int s = int(B.vlist[m].size());
		for(int v:B.vlist[m]){
			int d = int(B.elist[v].size());
			stubpairlist.push_back({d, s});
		}
	}

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::shuffle(stubpairlist.begin(), stubpairlist.end(), mt);

	std::unordered_map<int, std::vector<int>> node_deg_stublist;
	for(int v=0; v<N; ++v){
		int d = int(B.elist[v].size());
		if(node_deg_stublist.find(d) == node_deg_stublist.end()){
			node_deg_stublist[d] = std::vector<int>();
		}
		for(int i=0; i<d; ++i){
			node_deg_stublist[d].push_back(v);
		}
	}

	for(auto itr=node_deg_stublist.begin(); itr!=node_deg_stublist.end(); ++itr){
		int d = itr->first;
		std::shuffle(node_deg_stublist[d].begin(), node_deg_stublist[d].end(), mt);
	}

	std::unordered_map<int, std::vector<int>> edge_deg_stublist;
	for(int m=0; m<M; ++m){
		int s = int(B.vlist[m].size());
		if(edge_deg_stublist.find(s) == edge_deg_stublist.end()){
			edge_deg_stublist[s] = std::vector<int>();
		}
		for(int i=0; i<s; ++i){
			edge_deg_stublist[s].push_back(m);
		}
	}

	for(auto itr=edge_deg_stublist.begin(); itr!=edge_deg_stublist.end(); ++itr){
		int s = itr->first;
		std::shuffle(edge_deg_stublist[s].begin(), edge_deg_stublist[s].end(), mt);
	}

	BipartiteGraph BDC;
	BDC.N = N;
	BDC.M = M;
	BDC.elist = std::vector<std::vector<int>>(N, std::vector<int>());
	BDC.vlist = std::vector<std::vector<int>>(M, std::vector<int>());

	int v, m;
	while(int(stubpairlist.size()) > 0){
		int d = stubpairlist.back().first;
		int s = stubpairlist.back().second;
		stubpairlist.pop_back();
		v = node_deg_stublist[d].back();
		node_deg_stublist[d].pop_back();
		m = edge_deg_stublist[s].back();
		edge_deg_stublist[s].pop_back();
		BDC.elist[v].push_back(m);
		BDC.vlist[m].push_back(v);
	}

	for(int i=0; i<BDC.N; ++i){
		std::sort(BDC.elist[i].begin(),BDC.elist[i].end());
	}
	for(int i=0; i<BDC.M; ++i){
		std::sort(BDC.vlist[i].begin(),BDC.vlist[i].end());
	}

	return BDC;
}