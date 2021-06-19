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
	
	node_degree.clear();
	sum_node_degree = 0;
	num_jnt_node_deg.clear();
	degree_node_redun_coeff.clear();
	edge_size.clear();
	sum_edge_size = 0;
}

HyperGraph::~HyperGraph(){

	std::vector<std::vector<int>>().swap(elist);
	std::vector<std::vector<int>>().swap(vlist);
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

	for(int i=0; i<N; ++i){
		std::sort(elist[i].begin(),elist[i].end());
	}
	for(int i=0; i<M; ++i){
		std::sort(vlist[i].begin(),vlist[i].end());
	}

	printf("The given hypergraph named %s was successfully read.\n", graphname);
	printf("Number of nodes: %d\n", N);
	printf("Number of hyperedges: %d\n\n", M);

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

HyperGraph HyperGraph::return_largest_connected_component(){

	std::vector<int> searched(N, 0);
	std::vector<std::vector<int>> LCC_elist(N, std::vector<int>());
	std::vector<std::vector<int>> LCC_vlist(M, std::vector<int>());
	int n = 0;
	int v, s;

	for(int u=0; u<N; ++u){
		if(searched[u] == 1){continue;}

		std::deque<int> Q;
		std::vector<int> visited(N, 0);

		std::vector<int> CC_nodes;
		std::vector<std::vector<int>> CC_elist(N, std::vector<int>());
		std::vector<std::vector<int>> CC_vlist(M, std::vector<int>());

		visited[u] = 1;
		Q.push_back(u);

		while(Q.empty() == false){
			v = Q.front();
			Q.pop_front();
			searched[v] = 1;
			CC_nodes.push_back(v);
			for(int m:elist[v]){
				CC_elist[v].push_back(m);
				CC_vlist[m].push_back(v);
				for(int w:vlist[m]){
					if(visited[w] == 0){
						visited[w] = 1;
						Q.push_back(w);
					}
				}
			}
		}

		s = int(CC_nodes.size());
		if(s > n){
			n = s;
			LCC_elist = CC_elist;
			LCC_vlist = CC_vlist;
		}

		if(std::accumulate(searched.begin(), searched.end(), 0) >= N-n){break;}
	}
	
	HyperGraph LCC;
	std::unordered_map<int,int> node_index;
	std::unordered_map<int,int> edge_index;
	int v_i = 0;
	int e_i = 0;
	for(int v=0; v<N; ++v){
		if(int(LCC_elist[v].size()) == 0){continue;}

		node_index[v] = v_i;
		v_i += 1;
		LCC.elist.resize(v_i);

		for(int m:LCC_elist[v]){
			if(edge_index.find(m) == edge_index.end()){
				edge_index[m] = e_i;
				e_i += 1;
				LCC.vlist.resize(e_i);
			}
			LCC.elist[node_index[v]].push_back(edge_index[m]);
			LCC.vlist[edge_index[m]].push_back(node_index[v]);
		}
	}

	LCC.N = v_i;
	LCC.M = e_i;
	for(int i=0; i<LCC.N; ++i){
		std::sort(LCC.elist[i].begin(),LCC.elist[i].end());
	}
	for(int i=0; i<LCC.M; ++i){
		std::sort(LCC.vlist[i].begin(),LCC.vlist[i].end());
	}

	return LCC;
}

HyperGraph HyperGraph::remove_hyperedges_less_than_two_size() const{

	HyperGraph G;
	G.N = N;
	G.M = 0;
	G.elist = std::vector<std::vector<int>>(G.N, std::vector<int>());

	std::vector<int> edge_index(M, -1);

	for(int m=0; m<M; ++m){
		if(int(vlist[m].size()) < 2){continue;}

		if(edge_index[m] == -1){
			edge_index[m] = G.M;
			G.M += 1;
			G.vlist.resize(G.M);
		}

		for(int v:vlist[m]){
			G.vlist[edge_index[m]].push_back(v);
			G.elist[v].push_back(edge_index[m]);
		}
	}

	return G;
}

int HyperGraph::calc_node_degree(){

	node_degree.clear();
	for(int v=0; v<N; ++v){
		node_degree.set(v, int(elist[v].size()));
	}

	return 0;
}

int HyperGraph::calc_sum_node_degree(){

	sum_node_degree = 0;
	for(int v=0; v<N; ++v){
		sum_node_degree += int(elist[v].size());
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

int HyperGraph::calc_sum_num_jnt_node_deg(){

	sum_num_jnt_node_deg = 0;
	int m, s;
	for(m=0; m<M; ++m){
		s = int(vlist[m].size());
		sum_num_jnt_node_deg += s*(s-1);
	}

	return 0;
}

int HyperGraph::calc_node_redundancy_coefficient(){
	node_redun_coeff.clear();
	int d, i, j, m1, m2;

	for(int v=0; v<N; ++v){
		d = int(elist[v].size());
		if(d < 2){continue;}

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
					node_redun_coeff.add(v, 2);
				}
			}
		}

		node_redun_coeff.set(v, double(node_redun_coeff.get(v))/(d*(d-1)));
	}

	return 0;
}

int HyperGraph::calc_degree_dependent_node_redundancy_coefficient(){
	
	calc_node_redundancy_coefficient();
	degree_node_redun_coeff.clear();
	Vector<int> N_k;

	for(int v=0; v<N; ++v){
		int k = int(elist[v].size());
		N_k.add(k, 1);
		degree_node_redun_coeff.add(k, node_redun_coeff.get(v));
	}

	for(int id=0; id<int(N_k.id_to_keys.size()); ++id){
		int k = N_k.id_to_keys[id];
		if(k > 1 && N_k.get(k) > 0){
			degree_node_redun_coeff.set(k, double(degree_node_redun_coeff.get(k))/N_k.get(k));
		}
	}

	return 0;
}

int HyperGraph::calc_edge_size(){

	edge_size.clear();
	for(int m=0; m<M; ++m){
		edge_size.set(m, int(vlist[m].size()));
	}

	return 0;
}

int HyperGraph::calc_sum_edge_size(){

	sum_edge_size = 0;
	for(int m=0; m<M; ++m){
		sum_edge_size += int(vlist[m].size());
	}

	return 0;
}