#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <random>
#include <deque>
#include <stack>
#include <queue>
#include <numeric>
#include <set>
#include <unordered_map>
#include "basic_function.h"
#include "bipartite_graph.h"

BipartiteGraph::BipartiteGraph(){

	N = 0;
	M = 0;
	elist = std::vector<std::vector<int>>();
	vlist = std::vector<std::vector<int>>();
	
	node_degree.clear();
	sum_node_degree = 0;
	ave_node_degree = 0;
	node_degree_dist.clear();
	ave_neighbor_node_degree.clear();
	num_jnt_node_deg.clear();
	node_joint_degree_dist.clear();
	node_redun_coeff.clear();
	ave_node_redun_coeff = 0;
	degree_node_redun_coeff.clear();
	node_lcc.clear();
	ave_node_lcc = 0;
	degree_node_lcc.clear();
	ave_node_shortest_length = 0;
	node_shortest_length_dist.clear();
	edge_size.clear();
	ave_edge_size = 0;
	edge_size_dist.clear();
	cumulative_projected_degree_dist = std::vector<double>();
	num_multiple_edges = 0;
}

BipartiteGraph::~BipartiteGraph(){

	std::vector<std::vector<int>>().swap(elist);
	std::vector<std::vector<int>>().swap(vlist);
	std::vector<double>().swap(cumulative_projected_degree_dist);
}

int BipartiteGraph::read_hypergraph(const char *readgraph){

	char graphname[100];
	int s = int(sizeof(readgraph))/sizeof(char);
	if(s > 100){
		printf("Error: Length of name of readfile must be less than 100.\n");
		exit(0);
	}

	const char *dir = "../data/";

	FILE *gf;
	std::string gfpath = std::string(dir) + "hypergraphdata.txt";
	gf = fopen(gfpath.c_str(), "r");
	if(gf == NULL) {
		printf("Error: Could not open file named hypergraphdata.txt.\n");
		exit(0);
	}

	int i,j;
	int frag = 0;
	std::string readfile = std::string(readgraph);
	while(fscanf(gf, "%s %d %d", graphname,  &i , &j) != EOF) {
		if(strcmp(graphname, readfile.c_str()) == 0){
			N = i;
			M = j;
			frag = 1;
		}
	}
	fclose(gf);
	if(frag == 0){
		printf("Error: Could not find graph file named %s.txt.\n", readgraph);
		exit(0);
	}

	FILE *f1;
	std::string f1path = std::string(dir) + readgraph + "_nverts.txt";
	f1 = fopen(f1path.c_str(), "r");
	if(f1 == NULL) {
		printf("Error: Could not open file named %s_nverts.txt.\n", readgraph);
		exit(0);
	}

	FILE *f2;
	std::string f2path = std::string(dir) + readgraph + "_hyperedges.txt";
	f2 = fopen(f2path.c_str(), "r");
	if(f2 == NULL) {
		printf("Error: Could not open file named %s_hyperedges.txt.\n", readgraph);
		exit(0);
	}

	int n = 0;
	int m = 0;
	int nv, v;
	std::vector<std::set<int>> edges;
	std::unordered_map<int, int> node_index;
	elist = std::vector<std::vector<int>>(N, std::vector<int>());
	vlist = std::vector<std::vector<int>>(M, std::vector<int>());
	while(fscanf(f1, "%d", &nv) != EOF) {
		std::vector<int> e;
		for(int i=0; i<nv; ++i){
			auto fs = fscanf(f2, "%d", &v);
			if(fs != EOF){
				e.push_back(v);
			}
			else{
				std::string s = std::string(readgraph) + "_hyperedges.txt";
				printf("Error: Failed to read hyperedges wirtten in %s.\n", s.c_str());
				exit(0);
			}
		}

		std::set<int> e_set;
		for(int u:e){
			e_set.insert(u);
		}
		if(int(e.size()) != int(e_set.size())){
			printf("Error: some hyperedges contain duplicate nodes.\n");
			exit(0);
		}

		if(std::find(edges.begin(), edges.end(), e_set) != edges.end()){
			printf("Error: given hypergraph contains multiple hyperedges.\n");
			exit(0);
		}

		edges.push_back(e_set);
		for(int v:e_set){
			if(node_index.find(v) == node_index.end()){
				node_index[v] = n;
				n += 1;
			}
			vlist[m].push_back(node_index[v]);
			elist[node_index[v]].push_back(m);
		}

		m += 1;
	}

	if(n != N){
		printf("Error: Number of nodes is wrong.\n");
		exit(0);
	}
	if(m != M){
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

	return 0;
}

int BipartiteGraph::add_edge(const int &v, const int &m){
	if(v >= N){
		printf("Error: Given index of node is out of range.\n");
		exit(0);
	}
	if(m >= M){
		printf("Error: Given index of edge is out of range.\n");
		exit(0);
	}

	vlist[m].push_back(v);
	std::sort(vlist[m].begin(),vlist[m].end());
	elist[v].push_back(m);
	std::sort(elist[v].begin(),elist[v].end());
	
	return 0;
}

int BipartiteGraph::remove_edge(const int &v, const int &m){
	if(v >= N){
		printf("Error: Given index of node is out of range.\n");
		exit(0);
	}
	if(m >= M){
		printf("Error: Given index of edge is out of range.\n");
		exit(0);
	}

	auto itr1 = std::find(elist[v].begin(), elist[v].end(), m);
	if(itr1 == elist[v].end()){
		printf("Error: Node %d is not included in edge %d\n", v, m);
	}
	elist[v].erase(itr1);

	auto itr2 = std::find(vlist[m].begin(), vlist[m].end(), v);
	if(itr2 == vlist[m].end()){
		printf("Error: Node %d does not belong to edge %d\n", v, m);
	}
	vlist[m].erase(itr2);

	return 0;
}

BipartiteGraph BipartiteGraph::return_largest_connected_component(){

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
	
	BipartiteGraph LCC;
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

BipartiteGraph BipartiteGraph::remove_hyperedges_less_than_two_size() const{

	BipartiteGraph B;
	B.N = N;
	B.M = 0;
	B.elist = std::vector<std::vector<int>>(B.N, std::vector<int>());

	std::vector<int> edge_index(M, -1);

	for(int m=0; m<M; ++m){
		if(int(vlist[m].size()) < 2){continue;}

		if(edge_index[m] == -1){
			edge_index[m] = B.M;
			B.M += 1;
			B.vlist.resize(B.M);
		}

		for(int v:vlist[m]){
			B.vlist[edge_index[m]].push_back(v);
			B.elist[v].push_back(edge_index[m]);
		}
	}

	return B;
}

int BipartiteGraph::calc_number_of_nodes(){

	N = int(elist.size());

	return 0;
}

int BipartiteGraph::calc_number_of_edges(){

	M = int(vlist.size());

	return 0;
}

int BipartiteGraph::calc_node_degree(){

	node_degree.clear();
	for(int v=0; v<N; ++v){
		node_degree.set(v, int(elist[v].size()));
	}

	return 0;
}

int BipartiteGraph::calc_sum_node_degree(){

	sum_node_degree = 0;
	for(int v=0; v<N; ++v){
		sum_node_degree += int(elist[v].size());
	}

	return 0;
}

int BipartiteGraph::calc_average_node_degree(){
	
	ave_node_degree = 0;
	for(int v=0; v<N; ++v){
		ave_node_degree += int(elist[v].size());
	}

	ave_node_degree /= N;

	return 0;
}

int BipartiteGraph::calc_node_degree_distribution(){

	node_degree_dist.clear();
	for(int v=0; v<N; ++v){
		int d = int(elist[v].size());
		node_degree_dist.add(d, 1);
	}

	for(int id=0; id<int(node_degree_dist.id_to_keys.size()); ++id){
		int k = node_degree_dist.id_to_keys[id];
		node_degree_dist.set(k, double(node_degree_dist.get(k))/N);
	}

	return 0;
}

int BipartiteGraph::calc_cumulative_node_degree_distribution(){

	int maxd = 0;
	for(int v=0; v<N; ++v){
		int d = int(elist[v].size());
		if(d > maxd){
			maxd = d;
		}
	}

	cumulative_node_degree_dist = std::vector<double>(maxd+1, 0.0);
	for(int v=0; v<N; ++v){
  		int d = int(elist[v].size());
  		for(int i=1; i<=d; ++i){
  			cumulative_node_degree_dist[i] += 1;
  		}
  	}

  	for(int d=1; d<int(cumulative_node_degree_dist.size()); ++d){
  		cumulative_node_degree_dist[d] = double(cumulative_node_degree_dist[d])/N;
  	}

	return 0;
}

int BipartiteGraph::calc_num_jnt_node_deg(){

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

int BipartiteGraph::calc_node_joint_degree_distribution(){

	node_joint_degree_dist.clear();
	int m, s, i, j, u, v, k, l, norm;
	norm = 0;
	for(m=0; m<M; ++m){
		s = int(vlist[m].size());
		for(i=0; i<s-1; ++i){
			u = vlist[m][i];
			k = int(elist[u].size());
			for(j=i+1; j<s; ++j){
				v = vlist[m][j];
				l = int(elist[v].size());
				node_joint_degree_dist.add(k, l, 1);
				node_joint_degree_dist.add(l, k, 1);
				norm += 2;
			}
		}
	}

	for(int id=0; id<int(node_joint_degree_dist.id_to_keys.size()); ++id){
		int k = node_joint_degree_dist.id_to_keys[id];
		for(int id2=0; id2<int(node_joint_degree_dist.id_to_keys.size()); ++id2){
			int l = node_joint_degree_dist.id_to_keys[id2];
			node_joint_degree_dist.set(k, l, double(node_joint_degree_dist.get(k, l))/norm);
		}
	}
	
	return 0;
}

int BipartiteGraph::calc_sum_num_jnt_node_deg(){

	sum_num_jnt_node_deg = 0;
	int m, s;
	for(m=0; m<M; ++m){
		s = int(vlist[m].size());
		sum_num_jnt_node_deg += s*(s-1);
	}

	return 0;
}

int BipartiteGraph::calc_average_neighbor_node_degree(){

	calc_node_joint_degree_distribution();
	ave_neighbor_node_degree.clear();
	for(int id=0; id<int(node_joint_degree_dist.id_to_keys.size()); ++id){
		int k = node_joint_degree_dist.id_to_keys[id];
		double sum_k = node_joint_degree_dist.sum_line(k);
		for(int id2=0; id2<int(node_joint_degree_dist.id_to_keys.size()); ++id2){
			int l = node_joint_degree_dist.id_to_keys[id2];
			ave_neighbor_node_degree.add(k, double(l*node_joint_degree_dist.get(k, l)));
		}
		ave_neighbor_node_degree.set(k, double(ave_neighbor_node_degree.get(k))/sum_k);
	}

	return 0;
}

int BipartiteGraph::calc_node_redundancy_coefficient(){
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

int BipartiteGraph::calc_average_node_redundancy_coefficient(){

	calc_node_redundancy_coefficient();
	ave_node_redun_coeff = 0;

	for(int v=0; v<N; ++v){
		ave_node_redun_coeff += node_redun_coeff.get(v);
	}

	ave_node_redun_coeff /= N;

	return 0;
}

int BipartiteGraph::calc_degree_dependent_node_redundancy_coefficient(){
	
	calc_node_redundancy_coefficient();
	degree_node_redun_coeff.clear();
	OneDimProp<int> N_k;

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

int BipartiteGraph::calc_average_node_shortest_path_length(){

	ave_node_shortest_length = 0;

	for(int u=0; u<N; ++u){
		std::deque<int> Q;
		std::vector<int> d(N, -1);
		int v;

		Q.push_back(u);
		d[u] = 0;
		while(Q.size() != 0){
			v = Q.front();
			Q.pop_front();
			for(int m:elist[v]){
				for(int w:vlist[m]){
					if(d[w] < 0){
						Q.push_back(w);
						d[w] = d[v] + 1;
						ave_node_shortest_length += d[w];
					}
				}
			}
		}
	}

	ave_node_shortest_length /= N;
	ave_node_shortest_length /= (N-1);

	return 0;
}

int BipartiteGraph::calc_node_shortest_path_length_distribution(){

	std::vector<double> dist(N,0);

	for(int u=0; u<N; ++u){
		std::deque<int> Q;
		std::vector<int> d(N, -1);
		std::vector<int> dist_u(N, 0);
		int v, i;

		Q.push_back(u);
		d[u] = 0;
		while(Q.size() != 0){
			v = Q.front();
			Q.pop_front();
			for(int m:elist[v]){
				for(int w:vlist[m]){
					if(d[w] < 0){
						Q.push_back(w);
						d[w] = d[v] + 1;
						dist_u[d[w]] += 1;
						dist_u[0] = std::max(dist_u[0], d[w]);
					}
				}
			}
		}

		for(i=1; i<=dist_u[0]; ++i){
			dist[i] += dist_u[i];
		}
	}

	node_shortest_length_dist.clear();
	for(int l=1; l<int(dist.size()); ++l){
		if(dist[l] > 0){
			dist[l] = double(dist[l])/N;
			dist[l] = double(dist[l])/(N-1);
			node_shortest_length_dist.set(l, dist[l]);
		}
	}

	return 0;
}

int BipartiteGraph::calc_edge_size(){

	edge_size.clear();
	for(int m=0; m<M; ++m){
		edge_size.set(m, int(vlist[m].size()));
	}

	return 0;
}

int BipartiteGraph::calc_sum_edge_size(){

	sum_edge_size = 0;
	for(int m=0; m<M; ++m){
		sum_edge_size += int(vlist[m].size());
	}

	return 0;
}

int BipartiteGraph::calc_average_edge_size(){

	ave_edge_size = 0;
	for(int m=0; m<M; ++m){
		ave_edge_size += int(vlist[m].size());
	}

	ave_edge_size /= M;

	return 0;
}

int BipartiteGraph::calc_edge_size_distribution(){

	edge_size_dist.clear();
	for(int m=0; m<M; ++m){
		int s = int(vlist[m].size());
		edge_size_dist.add(s, 1);
	}

	for(int id=0; id<int(edge_size_dist.id_to_keys.size()); ++id){
		int s = edge_size_dist.id_to_keys[id];
		edge_size_dist.set(s, double(edge_size_dist.get(s))/M);
	}

	return 0;
}

int BipartiteGraph::calc_cumulative_edge_size_distribution(){

	int maxs = 0;
	for(int m=0; m<M; ++m){
		int s = int(vlist[m].size());
		if(s > maxs){
			maxs = s;
		}
	}

	cumulative_edge_size_dist = std::vector<double>(maxs+1, 0.0);
	for(int m=0; m<M; ++m){
  		int s = int(vlist[m].size());
  		for(int i=1; i<=s; ++i){
  			cumulative_edge_size_dist[i] += 1;
  		}
  	}

  	for(int s=1; s<int(cumulative_edge_size_dist.size()); ++s){
  		cumulative_edge_size_dist[s] = double(cumulative_edge_size_dist[s])/M;
  	}

	return 0;
}

int BipartiteGraph::calc_cumulative_projected_degree_distribution(){

	std::vector<std::vector<int>> projected_nlist = std::vector<std::vector<int>>(N, std::vector<int>());

	int i, j, m, s, u, v, d;
	for(m=0; m<M; ++m){
		s = int(vlist[m].size());
		for(i=0; i<s-1; ++i){
			u = vlist[m][i];
			for(j=i+1; j<s; ++j){
				v = vlist[m][j];
				projected_nlist[u].push_back(v);
				projected_nlist[v].push_back(u);
			}
		}
	}

	int maxd = 0;
  	for(int i=0; i<N; ++i){
  		d = int(projected_nlist[i].size());
  		if(d > maxd){
  			maxd = d;
  		}
  	}

  	cumulative_projected_degree_dist = std::vector<double>(maxd+1, 0.0);
  	for(int i=0; i<N; ++i){
  		d = int(projected_nlist[i].size());
  		for(int j=1; j<=d; ++j){
  			cumulative_projected_degree_dist[j] += 1;
  		}
  	}

  	for(int d=1; d<int(cumulative_projected_degree_dist.size()); ++d){
  		cumulative_projected_degree_dist[d] = double(cumulative_projected_degree_dist[d])/N;
  	}

	return 0;
}

int BipartiteGraph::calc_number_of_multiple_edges(){

	int number_of_edges_with_duplication = 0;
	std::set<std::pair<int, int>> set_of_edges;

	for(int v=0; v<N; ++v){
		for(int m:elist[v]){
			number_of_edges_with_duplication += 1;
			set_of_edges.insert({v, m});
		}
	}

	num_multiple_edges = number_of_edges_with_duplication - int(set_of_edges.size());

	return 0;
}

int BipartiteGraph::calc_number_of_hyperedges_less_than_two_size(){

	M_less_than_two_size = 0;
	for(int m=0; m<M; ++m){
		if(int(vlist[m].size()) < 2){
			M_less_than_two_size += 1;
		}
	}

	return 0;
}

int BipartiteGraph::calc_properties(){

	printf("Properties of the given bipartite graph.\n\n");
		
	printf("Number of nodes: %d\n", N);

	printf("Number of hyperedges: %d\n", M);

	calc_number_of_multiple_edges();
	printf("Number of multiple hyperedges: %d\n", num_multiple_edges);

	calc_number_of_hyperedges_less_than_two_size();
	printf("Number of hyperedges with size less than two: %d\n", M_less_than_two_size);

	BipartiteGraph LCC = return_largest_connected_component();
	printf("Number of nodes in the largest connected component: %d\n", int(LCC.N));
	printf("Number of hyperedges in the largest connected component: %d\n", int(LCC.M));

	calc_average_node_degree();
	printf("Average node degree: %lf\n", ave_node_degree);

	calc_average_edge_size();
	printf("Average edge size: %lf\n", ave_edge_size);

	calc_sum_node_degree();
	printf("Number of edges: %d\n", sum_node_degree);

	calc_node_degree_distribution();
	printf("Node degree distribution:\n");
	for(int id=0; id<int(node_degree_dist.id_to_keys.size()); ++id){
		int k = node_degree_dist.id_to_keys[id];
		printf("(%d: %lf), ", k, node_degree_dist.get(k));
	}
	printf("\n");

	calc_cumulative_node_degree_distribution();
	printf("Cumulative node degree distribution:\n");
	for(int d=0; d<int(cumulative_node_degree_dist.size()); ++d){
		printf("(%d: %lf), ", d, cumulative_node_degree_dist[d]);
	}
	printf("\n");

	calc_edge_size_distribution();
	printf("Edge size distribution:\n");
	for(int id=0; id<int(edge_size_dist.id_to_keys.size()); ++id){
		int s = edge_size_dist.id_to_keys[id];
		printf("(%d: %lf), ", s, edge_size_dist.get(s));
	}
	printf("\n");

	calc_cumulative_edge_size_distribution();
	printf("Cumulative edge size distribution:\n");
	for(int s=0; s<int(cumulative_edge_size_dist.size()); ++s){
		printf("(%d: %lf), ", s, cumulative_edge_size_dist[s]);
	}
	printf("\n");

	calc_average_neighbor_node_degree();
	printf("Average neighbor node degree:\n");
	for(int id=0; id<int(ave_neighbor_node_degree.id_to_keys.size()); ++id){
		int k = ave_neighbor_node_degree.id_to_keys[id];
		printf("(%d: %lf), ", k, ave_neighbor_node_degree.get(k));
	}
	printf("\n");

	calc_average_node_redundancy_coefficient();
	printf("Average node redundancy coefficient: %lf\n", ave_node_redun_coeff);

	calc_degree_dependent_node_redundancy_coefficient();
	printf("Degree-dependent node redundancy coefficient:\n");
	for(int id=0; id<int(degree_node_redun_coeff.id_to_keys.size()); ++id){
		int k = degree_node_redun_coeff.id_to_keys[id];
		printf("(%d: %lf), ", k, degree_node_redun_coeff.get(k));
	}
	printf("\n");

	LCC.calc_average_node_shortest_path_length();
	ave_node_shortest_length = LCC.ave_node_shortest_length;
	printf("Average node shortest path length: %lf\n", ave_node_shortest_length);

	LCC.calc_node_shortest_path_length_distribution();
	node_shortest_length_dist = LCC.node_shortest_length_dist;
	printf("Node shortest path length distribution:\n");
	for(int id=0; id<int(node_shortest_length_dist.id_to_keys.size()); ++id){
		int l = node_shortest_length_dist.id_to_keys[id];
		printf("(%d: %lf), ", l, node_shortest_length_dist.get(l));
	}
	printf("\n");

	calc_cumulative_projected_degree_distribution();
	printf("Cumulative projected degree distribution:\n");
	for(int d=1; d<int(cumulative_projected_degree_dist.size()); ++d){
		printf("(%d: %lf), ", d, cumulative_projected_degree_dist[d]);
	}
	printf("\n");
	
	return 0;
}