#include <vector>
#include <random>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "hypergraph.h"
#include "rewiring.h"

HyperGraph targeting_rewiring_d_v_two(HyperGraph G, HyperGraph randG){

	printf("Targeting-rewiring process with d_v = 2.\n");

	const int G_N = int(G.V.size());
	const int G_M = int(G.E.size());

	Matrix<int> target_num_jnt_node_deg;
	calc_num_jnt_node_deg(G, target_num_jnt_node_deg);

	int target_sum_num_jnt_node_deg = 0;
	for(int m=0; m<G_M; ++m){
		int s = int(G.vlist[m].size());
		target_sum_num_jnt_node_deg += s*(s-1);
	}

	const int randG_N = int(randG.V.size());
	const int randG_M = int(randG.E.size());

	Matrix<int> num_jnt_node_deg;
	calc_num_jnt_node_deg(randG, num_jnt_node_deg);

	int sum_num_jnt_node_deg = 0;
	for(int m=0; m<randG_M; ++m){
		int s = int(randG.vlist[m].size());
		sum_num_jnt_node_deg += s*(s-1);
	}

	std::vector<int> G_node_degree, randG_node_degree, randG_hyperedge_size;
	calc_node_degree(G, G_node_degree);
	calc_node_degree(randG, randG_node_degree);
	calc_hyperedge_size(randG, randG_hyperedge_size);

	std::set<int> degrees;
	for(int v=0; v<G_N; ++v){
		degrees.insert(G_node_degree[v]);
	}
	for(int v=0; v<randG_N; ++v){
		degrees.insert(randG_node_degree[v]);
	}

	double dist = 0;
	double norm = 0;
	for(int k:degrees){
		for(int l:degrees){
			dist += std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg - double(num_jnt_node_deg.get(k, l))/sum_num_jnt_node_deg);
			norm += std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg);
		}
	}

	printf("Initial L1 distance between the target and present P_v(k, k'): %lf\n", double(dist)/norm);

	std::vector<std::pair<int,int>> bipartite_edges;
	for(int v=0; v<randG_N; ++v){
		for(int m:randG.elist[v]){
			bipartite_edges.push_back({v, m});
		}
	}

	const int B_M = int(bipartite_edges.size());

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randB_M(0, B_M-1);

	const int R = 500*B_M;

	int r, e1, e2, u, v, m1, m2, k1, k2, k, l, id1, id2;
	double rewired_dist, delta_dist, x, y;

	for(r=0; r<R; ++r){
		e1 = randB_M(mt);
		e2 = randB_M(mt);
		u = bipartite_edges[e1].first;
		m1 = bipartite_edges[e1].second;
		v = bipartite_edges[e2].first;
		m2 = bipartite_edges[e2].second;
		while(u == v || m1 == m2){
			e1 = randB_M(mt);
			e2 = randB_M(mt);
			u = bipartite_edges[e1].first;
			m1 = bipartite_edges[e1].second;
			v = bipartite_edges[e2].first;
			m2 = bipartite_edges[e2].second;
		}

		k1 = randG_node_degree[u];
		k2 = randG_node_degree[v];

		Matrix<int> num_jnt_node_deg_to_add;

		for(int w:randG.vlist[m1]){
			l = randG_node_degree[w];
			num_jnt_node_deg_to_add.subtract(k1, l, 1);
			num_jnt_node_deg_to_add.subtract(l, k1, 1);
		}
		num_jnt_node_deg_to_add.add(k1, k1, 2);

		for(int w:randG.vlist[m2]){
			l = randG_node_degree[w];
			num_jnt_node_deg_to_add.add(k1, l, 1);
			num_jnt_node_deg_to_add.add(l, k1, 1);
		}

		for(int w:randG.vlist[m2]){
			l = randG_node_degree[w];
			num_jnt_node_deg_to_add.subtract(k2, l, 1);
			num_jnt_node_deg_to_add.subtract(l, k2, 1);
		}
		num_jnt_node_deg_to_add.add(k2, k2, 2);
		num_jnt_node_deg_to_add.subtract(k1, k2, 1);
		num_jnt_node_deg_to_add.subtract(k2, k1, 1);

		for(int w:randG.vlist[m1]){
			l = randG_node_degree[w];
			num_jnt_node_deg_to_add.add(k2, l, 1);
			num_jnt_node_deg_to_add.add(l, k2, 1);
		}
		num_jnt_node_deg_to_add.subtract(k1, k2, 1);
		num_jnt_node_deg_to_add.subtract(k2, k1, 1);

		rewired_dist = dist;
		for(id1=0; id1<int(num_jnt_node_deg_to_add.id_to_keys.size()); ++id1){
			k = num_jnt_node_deg_to_add.id_to_keys[id1];
			for(id2=0; id2<int(num_jnt_node_deg_to_add.id_to_keys.size()); ++id2){
				l = num_jnt_node_deg_to_add.id_to_keys[id2];
				x = std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg - double(num_jnt_node_deg.get(k, l)+num_jnt_node_deg_to_add.get(k, l))/sum_num_jnt_node_deg);
				y = std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg - double(num_jnt_node_deg.get(k, l))/sum_num_jnt_node_deg);
				rewired_dist += x - y;
			}
		}

		delta_dist = rewired_dist - dist;

		if(delta_dist < 0){
			bipartite_edges[e1] = {u, m2};
			bipartite_edges[e2] = {v, m1};

			remove_node_from_hyperedge(randG, u, m1);
			add_node_to_hyperedge(randG, u, m2);
			remove_node_from_hyperedge(randG, v, m2);
			add_node_to_hyperedge(randG, v, m1);

			for(id1=0; id1<int(num_jnt_node_deg_to_add.id_to_keys.size()); ++id1){
				k = num_jnt_node_deg_to_add.id_to_keys[id1];
				for(id2=0; id2<int(num_jnt_node_deg_to_add.id_to_keys.size()); ++id2){
					l = num_jnt_node_deg_to_add.id_to_keys[id2];
					num_jnt_node_deg.add(k, l, num_jnt_node_deg_to_add.get(k, l));
				}
			}

			dist = rewired_dist;
		}
	}

	printf("Final L1 distance between target and current P_v(k, k'): %lf\n", double(dist)/norm);

	return randG;
}

int update_degree_node_redundancy_coefficient_by_edge_addition(HyperGraph &randG, const int &v, const int &m, 
	const int &k, const std::vector<int> &randG_node_degree, std::vector<int> &degree_node_num_redun_to_add){

	std::unordered_map<int, int> checked;
	for(int m2:randG.elist[v]){
		checked[m2] = 0;
	}

	std::set<int> s1, s3, s4, s5;
	std::set<int> s2 = {v};
	int x, y, l;

	for(int m2: randG.elist[v]){
		if(int(randG.vlist[m2].size()) < 2 || m2 == m){continue;}

		s1.clear();
		s3.clear();
		std::set_intersection(randG.vlist[m].begin(), randG.vlist[m].end(), randG.vlist[m2].begin(), randG.vlist[m2].end(), std::inserter(s1, s1.end()));
		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));
		
		if(int(s3.size()) == 0){continue;}
		
		degree_node_num_redun_to_add[k] += 1;

		if(checked[m2] == 1){continue;}

		for(int w:s3){
			s4 = {w};
			s5.clear();
			std::set_difference(s1.begin(), s1.end(), s4.begin(), s4.end(), std::inserter(s5, s5.end()));
			if(s5 == s2 && std::count(randG.vlist[m].begin(), randG.vlist[m].end(), v) == 1){
				l = randG_node_degree[w];
				x = std::count(randG.elist[w].begin(), randG.elist[w].end(), m);
				y = std::count(randG.elist[w].begin(), randG.elist[w].end(), m2);
				degree_node_num_redun_to_add[l] += x*y;
			}
		}
		checked[m2] = 1;
	}

	return 0;
}

int update_degree_node_redundancy_coefficient_by_edge_deletion(HyperGraph &randG, const int &v, const int &m, 
	const int &k, const std::vector<int> &randG_node_degree, std::vector<int> &degree_node_num_redun_to_add){

	std::unordered_map<int, int> checked;
	for(int m2:randG.elist[v]){
		checked[m2] = 0;
	}

	std::set<int> s1, s3, s4, s5;
	std::set<int> s2 = {v};
	int x, y, l;

	for(int m2: randG.elist[v]){
		if(int(randG.vlist[m2].size()) < 2 || m2 == m){continue;}

		s1.clear();
		s3.clear();
		std::set_intersection(randG.vlist[m].begin(), randG.vlist[m].end(), randG.vlist[m2].begin(), randG.vlist[m2].end(), std::inserter(s1, s1.end()));
		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));
		
		if(int(s3.size()) == 0){continue;}

		degree_node_num_redun_to_add[k] -= 1;

		if(checked[m2] == 1){continue;}

		for(int w:s3){
			s4 = {w};
			s5.clear();
			std::set_difference(s1.begin(), s1.end(), s4.begin(), s4.end(), std::inserter(s5, s5.end()));
			if(int(s5.size()) == 0){
				l = randG_node_degree[w];
				x = std::count(randG.elist[w].begin(), randG.elist[w].end(), m);
				y = std::count(randG.elist[w].begin(), randG.elist[w].end(), m2);
				degree_node_num_redun_to_add[l] -= x*y;
			}
		}
		checked[m2] = 1;
	}

	return 0;
}

HyperGraph targeting_rewiring_d_v_two_five(HyperGraph G, HyperGraph randG){

	printf("Targeting-rewiring process with d_v = 2.5.\n");

	std::vector<double> target_degree_node_redun_coeff;
	calc_degree_dependent_node_redundancy_coefficient(G, target_degree_node_redun_coeff);
	
	std::vector<double> degree_node_redun_coeff;
	calc_degree_dependent_node_redundancy_coefficient(randG, degree_node_redun_coeff);

	const int randG_N = int(randG.V.size());

	int max_node_deg;
	calc_maximum_node_degree(randG, max_node_deg);
	int d_size = max_node_deg+1;

	double dist = 0;
	double norm = 0;
	int k;
	for(k=2; k<d_size; ++k){
		dist += std::fabs(target_degree_node_redun_coeff[k] - degree_node_redun_coeff[k]);
		norm += std::fabs(target_degree_node_redun_coeff[k]);
	}

	printf("Initial L1 distance between target and present r(k): %lf\n", double(dist)/norm);

	std::vector<int> randG_node_degree, randG_hyperedge_size;
	calc_node_degree(randG, randG_node_degree);
	calc_hyperedge_size(randG, randG_hyperedge_size);

	std::vector<int> N_k(d_size, 0);
	for(int v=0; v<randG_N; ++v){
		k = randG_node_degree[v];
		N_k[k] += 1;
	}

	std::vector<double> const_coeff(d_size, 0.0);
	for(k=2; k<d_size; ++k){
		if(N_k[k] == 0){continue;}
		const_coeff[k] = double(2)/(k*(k-1));
		const_coeff[k] = double(const_coeff[k])/N_k[k];
	}

	std::vector<std::pair<int,int>> bipartite_edges;
	for(int v=0; v<randG_N; ++v){
		for(int m:randG.elist[v]){
			bipartite_edges.push_back({v, m});
		}
	}
	const int B_M = int(bipartite_edges.size());

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randB_M(0, B_M-1);

	std::vector<double> rewired_degree_node_redun_coeff(d_size, 0.0);
	std::vector<int> degree_node_num_redun_to_add(d_size, 0);

	const int R = 500*B_M;

	int r, e1, e2, u, v, m1, m2, k1, k2;
	double delta_dist;

	for(r=0; r<R; ++r){
		e1 = randB_M(mt);
		e2 = randB_M(mt);
		u = bipartite_edges[e1].first;
		m1 = bipartite_edges[e1].second;
		v = bipartite_edges[e2].first;
		m2 = bipartite_edges[e2].second;
		k1 = randG_node_degree[u];
		k2 = randG_node_degree[v];
		while(u == v || m1 == m2 || k1 != k2){
			e1 = randB_M(mt);
			e2 = randB_M(mt);
			u = bipartite_edges[e1].first;
			m1 = bipartite_edges[e1].second;
			v = bipartite_edges[e2].first;
			m2 = bipartite_edges[e2].second;
			k1 = randG_node_degree[u];
			k2 = randG_node_degree[v];
		}

		degree_node_num_redun_to_add = std::vector<int>(d_size, 0);

		remove_node_from_hyperedge(randG, u, m1);
		update_degree_node_redundancy_coefficient_by_edge_deletion(randG, u, m1, k1, randG_node_degree, degree_node_num_redun_to_add);
		add_node_to_hyperedge(randG, u, m2);
		update_degree_node_redundancy_coefficient_by_edge_addition(randG, u, m2, k1, randG_node_degree, degree_node_num_redun_to_add);
		remove_node_from_hyperedge(randG, v, m2);
		update_degree_node_redundancy_coefficient_by_edge_deletion(randG, v, m2, k2, randG_node_degree, degree_node_num_redun_to_add);
		add_node_to_hyperedge(randG, v, m1);
		update_degree_node_redundancy_coefficient_by_edge_addition(randG, v, m1, k2, randG_node_degree, degree_node_num_redun_to_add);

		rewired_degree_node_redun_coeff = std::vector<double>(degree_node_redun_coeff);
		delta_dist = 0;
		
		for(k=2; k<d_size; ++k){
			if(degree_node_num_redun_to_add[k] == 0){continue;}
			rewired_degree_node_redun_coeff[k] += double(degree_node_num_redun_to_add[k]*const_coeff[k]);
			delta_dist += std::fabs(target_degree_node_redun_coeff[k] - rewired_degree_node_redun_coeff[k]) - std::fabs(target_degree_node_redun_coeff[k] - degree_node_redun_coeff[k]);
		}

		if(delta_dist < 0){
			bipartite_edges[e1] = {u, m2};
			bipartite_edges[e2] = {v, m1};
			degree_node_redun_coeff = std::vector<double>(rewired_degree_node_redun_coeff);
			dist += delta_dist;
		}
		else{
			remove_node_from_hyperedge(randG, u, m2);
			add_node_to_hyperedge(randG, u, m1);
			remove_node_from_hyperedge(randG, v, m1);
			add_node_to_hyperedge(randG, v, m2);
		}
	}

	printf("Final L1 distance between target and present r(k): %lf\n", double(dist)/norm);

	return randG;
}