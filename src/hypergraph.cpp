#include <vector>
#include <random>
#include <deque>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "basic_function.h"
#include "hypergraph.h"

HyperGraph::HyperGraph(){

	N = 0;
	M = 0;
	elist = std::vector<std::vector<int>>();
	vlist = std::vector<std::vector<int>>();
	max_node_deg = 0;
	
	node_degree = std::vector<int>();
	num_jnt_node_deg.clear();
	node_redun_coeff = std::vector<double>();
	degree_node_redun_coeff = std::vector<double>();
	edge_size = std::vector<int>();
}

HyperGraph::~HyperGraph(){

	std::vector<std::vector<int>>().swap(elist);
	std::vector<std::vector<int>>().swap(vlist);
	std::vector<int>().swap(node_degree);
	std::vector<double>().swap(degree_node_redun_coeff);
	std::vector<int>().swap(edge_size);
}

int HyperGraph::read_hypergraph(const char *graphname){

	const char *dir = "../data/";

	FILE *f1;
	std::string f1path = std::string(dir) + graphname + "_nverts.txt";
	f1 = fopen(f1path.c_str(), "r");
	if(f1 == NULL) {
		printf("Error: Could not open file named %s_nverts.txt.\n", graphname);
		exit(0);
	}

	FILE *f2;
	std::string f2path = std::string(dir) + graphname + "_hyperedges.txt";
	f2 = fopen(f2path.c_str(), "r");
	if(f2 == NULL) {
		printf("Error: Could not open file named %s_hyperedges.txt.\n", graphname);
		exit(0);
	}

	int n = 0;
	int m = 0;
	int nv, v;
	std::vector<std::set<int>> edges;
	std::unordered_map<int, int> node_index;
	elist = std::vector<std::vector<int>>();
	vlist = std::vector<std::vector<int>>();
	while(fscanf(f1, "%d", &nv) != EOF) {
		std::vector<int> e;
		for(int i=0; i<nv; ++i){
			auto fs = fscanf(f2, "%d", &v);
			if(fs != EOF){
				e.push_back(v);
			}
			else{
				std::string s = std::string(graphname) + "_hyperedges.txt";
				printf("Error: Failed to read a list of hyperedges.\n");
				exit(0);
			}
		}

		std::set<int> e_set;
		for(int u:e){
			e_set.insert(u);
		}
		if(int(e.size()) != int(e_set.size())){
			printf("Error: There are hyperedges containing duplicate nodes.\n");
			exit(0);
		}

		if(std::find(edges.begin(), edges.end(), e_set) != edges.end()){
			printf("Error: The given hypergraph contains multiple hyperedges.\n");
			exit(0);
		}

		vlist.resize(m+1, std::vector<int>());
		edges.push_back(e_set);
		for(int v:e_set){
			if(node_index.find(v) == node_index.end()){
				node_index[v] = n;
				elist.resize(n+1, std::vector<int>());
				n += 1;
			}
			vlist[m].push_back(node_index[v]);
			elist[node_index[v]].push_back(m);
		}

		m += 1;
	}

	N = n;
	M = m;

	if(N != int(elist.size())){
		printf("Error: Number of nodes is wrong.\n");
		exit(0);
	}
	if(M != int(vlist.size())){
		printf("Error: Number of hyperedges is wrong.\n");
		exit(0);
	}
	
	fclose(f1);
	fclose(f2);

	max_node_deg = 0;
	for(int i=0; i<N; ++i){
		std::sort(elist[i].begin(),elist[i].end());
		if(int(elist[i].size()) > max_node_deg){
			max_node_deg = int(elist[i].size());
		}
	}
	for(int i=0; i<M; ++i){
		std::sort(vlist[i].begin(),vlist[i].end());
	}

	printf("The given hypergraph named %s was successfully read.\n", graphname);
	printf("Number of nodes: %d\n", N);
	printf("Number of hyperedges: %d\n", M);

	return 0;
}

int HyperGraph::add_node_to_hyperedge(const int &v, const int &m){
	if(v >= N){
		printf("Error: Given node is not found.\n");
		exit(0);
	}
	if(m >= M){
		printf("Error: Given hyperedge is not found.\n");
		exit(0);
	}

	vlist[m].push_back(v);
	std::sort(vlist[m].begin(),vlist[m].end());
	elist[v].push_back(m);
	std::sort(elist[v].begin(),elist[v].end());
	
	return 0;
}

int HyperGraph::remove_node_from_hyperedge(const int &v, const int &m){
	if(v >= N){
		printf("Error: Given node is not found.\n");
		exit(0);
	}
	if(m >= M){
		printf("Error: Given hyperedge is not found.\n");
		exit(0);
	}

	auto itr1 = std::find(elist[v].begin(), elist[v].end(), m);
	if(itr1 == elist[v].end()){
		printf("Error: Node %d is not included in hyperedge %d\n", v, m);
	}
	elist[v].erase(itr1);

	auto itr2 = std::find(vlist[m].begin(), vlist[m].end(), v);
	if(itr2 == vlist[m].end()){
		printf("Error: Node %d does not belong to hyperedge %d\n", v, m);
	}
	vlist[m].erase(itr2);

	return 0;
}

int HyperGraph::calc_node_degree(){

	node_degree = std::vector<int>(N, 0);
	max_node_deg = 0;
	for(int v=0; v<N; ++v){
		node_degree[v] = int(elist[v].size());
		if(int(elist[v].size()) > max_node_deg){
			max_node_deg = int(elist[v].size());
		}
	}

	return 0;
}

int HyperGraph::calc_num_jnt_node_deg(){

	num_jnt_node_deg.clear();
	int m, s, i, j, u, v, k, l;
	for(m=0; m<M; ++m){
		s = int(vlist[m].size());
		for(i=0; i<s-1; ++i){
			u = vlist[m][i];
			k = int(elist[u].size());
			for(j=i+1; j<s; ++j){
				v = vlist[m][j];
				l = int(elist[v].size());
				num_jnt_node_deg.add(k, l, 1);
				num_jnt_node_deg.add(l, k, 1);
			}
		}
	}

	return 0;
}

int HyperGraph::calc_node_redundancy_coefficient(){
	node_redun_coeff = std::vector<double>(N, 0.0);
	int d, i, j, m1, m2;

	for(int v=0; v<N; ++v){
		d = int(elist[v].size());
		if(d < 2){continue;}

		double rc = 0.0;
		for(i=0; i<d-1; ++i){
			m1 = elist[v][i];
			for(j=i+1; j<d; ++j){
				m2 = elist[v][j];
				if(m1 == m2){continue;}
				std::set<int> s1, s3;
				std::set<int> s2 = {v};
				std::set_intersection(vlist[m1].begin(), vlist[m1].end(), vlist[m2].begin(), vlist[m2].end(), std::inserter(s1, s1.end()));
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

int HyperGraph::calc_degree_dependent_node_redundancy_coefficient(){
	
	calc_node_redundancy_coefficient();
	degree_node_redun_coeff = std::vector<double>(max_node_deg+1, 0.0);
	std::vector<int> N_k(max_node_deg+1, 0);

	for(int v=0; v<N; ++v){
		int k = int(elist[v].size());
		N_k[k] += 1;
		degree_node_redun_coeff[k] += node_redun_coeff[v];
	}

	for(int k=2; k<int(N_k.size()); ++k){
		if(N_k[k] > 0){
			degree_node_redun_coeff[k] = double(degree_node_redun_coeff[k])/N_k[k];
		}
	}

	return 0;
}

int HyperGraph::calc_edge_size(){

	edge_size = std::vector<int>(M, 0);
	for(int m=0; m<M; ++m){
		edge_size[m] = int(vlist[m].size());
	}

	return 0;
}