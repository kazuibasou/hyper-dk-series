#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <math.h>
#include <set>
#include "basic_function.h"
#include "bipartite_graph.h"
#include "bipartite_rewiring.h"

BipartiteGraph Rewiring::targeting_node_twoK_preserving_node_oneK_and_edge_oneK(BipartiteGraph B, BipartiteGraph genB){

	B.calc_num_jnt_node_deg();
	B.calc_sum_num_jnt_node_deg();
	const TwoDimProp<int> target_num_jnt_node_deg = TwoDimProp<int>(B.num_jnt_node_deg);
	const int target_sum_num_jnt_node_deg = B.sum_num_jnt_node_deg;

	genB.calc_num_jnt_node_deg();
	genB.calc_sum_num_jnt_node_deg();
	TwoDimProp<int> num_jnt_node_deg = TwoDimProp<int>(genB.num_jnt_node_deg);
	const int sum_num_jnt_node_deg = genB.sum_num_jnt_node_deg;

	B.calc_node_degree();
	genB.calc_node_degree();
	genB.calc_edge_size();

	std::set<int> degrees;
	for(int v=0; v<B.N; ++v){
		degrees.insert(B.node_degree.get(v));
	}
	for(int v=0; v<genB.N; ++v){
		degrees.insert(genB.node_degree.get(v));
	}

	double dist = 0;
	double norm = 0;
	for(int k:degrees){
		for(int l:degrees){
			dist += std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg - double(num_jnt_node_deg.get(k, l))/sum_num_jnt_node_deg);
			norm += std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg);
		}
	}

	//printf("Initial normalized distance between the target and current joint degree distributions of the node: %lf\n", double(dist)/norm);

	std::vector<std::pair<int,int>> bipartite_edges;
	for(int v=0; v<genB.N; ++v){
		for(int m:genB.elist[v]){
			bipartite_edges.push_back({v, m});
		}
	}

	int B_M = int(bipartite_edges.size());

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randB_M(0, B_M-1);

	const int R = R_factor_targeting*B_M;

	jdd_distance_track.clear();
	jdd_distance_track.push_back(double(dist)/norm);

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

		k1 = genB.node_degree.get(u);
		k2 = genB.node_degree.get(v);

		TwoDimProp<int> num_jnt_node_deg_to_add;

		for(int w:genB.vlist[m1]){
			l = genB.node_degree.get(w);
			num_jnt_node_deg_to_add.subtract(k1, l, 1);
			num_jnt_node_deg_to_add.subtract(l, k1, 1);
		}
		num_jnt_node_deg_to_add.add(k1, k1, 2);

		for(int w:genB.vlist[m2]){
			l = genB.node_degree.get(w);
			num_jnt_node_deg_to_add.add(k1, l, 1);
			num_jnt_node_deg_to_add.add(l, k1, 1);
		}

		for(int w:genB.vlist[m2]){
			l = genB.node_degree.get(w);
			num_jnt_node_deg_to_add.subtract(k2, l, 1);
			num_jnt_node_deg_to_add.subtract(l, k2, 1);
		}
		num_jnt_node_deg_to_add.add(k2, k2, 2);
		num_jnt_node_deg_to_add.subtract(k1, k2, 1);
		num_jnt_node_deg_to_add.subtract(k2, k1, 1);

		for(int w:genB.vlist[m1]){
			l = genB.node_degree.get(w);
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

			genB.remove_edge(u, m1);
			genB.add_edge(u, m2);
			genB.remove_edge(v, m2);
			genB.add_edge(v, m1);

			for(id1=0; id1<int(num_jnt_node_deg_to_add.id_to_keys.size()); ++id1){
				k = num_jnt_node_deg_to_add.id_to_keys[id1];
				for(id2=0; id2<int(num_jnt_node_deg_to_add.id_to_keys.size()); ++id2){
					l = num_jnt_node_deg_to_add.id_to_keys[id2];
					num_jnt_node_deg.add(k, l, num_jnt_node_deg_to_add.get(k, l));
				}
			}

			dist = rewired_dist;
		}

		jdd_distance_track.push_back(double(dist)/norm);
	}

	//printf("Final normalized distance between target and current joint degree distributions of the node: %lf\n", double(dist)/norm);

	return genB;
}

int update_degree_node_redundancy_coefficient_by_edge_addition(BipartiteGraph &genB, const int &v, const int &m, const int &k,
	std::vector<int> &degree_node_num_redun_to_add){

	std::unordered_map<int, int> checked;
	for(int m2:genB.elist[v]){
		checked[m2] = 0;
	}

	std::set<int> s1, s3, s4, s5;
	std::set<int> s2 = {v};
	int x, y, l;

	for(int m2: genB.elist[v]){
		if(int(genB.vlist[m2].size()) < 2 || m2 == m){continue;}

		s1.clear();
		s3.clear();
		std::set_intersection(genB.vlist[m].begin(), genB.vlist[m].end(), genB.vlist[m2].begin(), genB.vlist[m2].end(), std::inserter(s1, s1.end()));
		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));
		
		if(int(s3.size()) == 0){continue;}
		
		degree_node_num_redun_to_add[k] += 1;

		if(checked[m2] == 1){continue;}

		for(int w:s3){
			s4 = {w};
			s5.clear();
			std::set_difference(s1.begin(), s1.end(), s4.begin(), s4.end(), std::inserter(s5, s5.end()));
			if(s5 == s2 && std::count(genB.vlist[m].begin(), genB.vlist[m].end(), v) == 1){
				l = genB.node_degree.get(w);
				x = std::count(genB.elist[w].begin(), genB.elist[w].end(), m);
				y = std::count(genB.elist[w].begin(), genB.elist[w].end(), m2);
				degree_node_num_redun_to_add[l] += x*y;
			}
		}
		checked[m2] = 1;
	}

	return 0;
}

int update_degree_node_redundancy_coefficient_by_edge_deletion(BipartiteGraph &genB, const int &v, const int &m, const int &k,
	std::vector<int> &degree_node_num_redun_to_add){

	std::unordered_map<int, int> checked;
	for(int m2:genB.elist[v]){
		checked[m2] = 0;
	}

	std::set<int> s1, s3, s4, s5;
	std::set<int> s2 = {v};
	int x, y, l;

	for(int m2: genB.elist[v]){
		if(int(genB.vlist[m2].size()) < 2 || m2 == m){continue;}

		s1.clear();
		s3.clear();
		std::set_intersection(genB.vlist[m].begin(), genB.vlist[m].end(), genB.vlist[m2].begin(), genB.vlist[m2].end(), std::inserter(s1, s1.end()));
		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));
		
		if(int(s3.size()) == 0){continue;}

		degree_node_num_redun_to_add[k] -= 1;

		if(checked[m2] == 1){continue;}

		for(int w:s3){
			s4 = {w};
			s5.clear();
			std::set_difference(s1.begin(), s1.end(), s4.begin(), s4.end(), std::inserter(s5, s5.end()));
			if(int(s5.size()) == 0){
				l = genB.node_degree.get(w);
				x = std::count(genB.elist[w].begin(), genB.elist[w].end(), m);
				y = std::count(genB.elist[w].begin(), genB.elist[w].end(), m2);
				degree_node_num_redun_to_add[l] -= x*y;
			}
		}
		checked[m2] = 1;
	}

	return 0;
}

BipartiteGraph Rewiring::targeting_node_two_fiveK_preserving_node_twoK_and_edge_oneK(BipartiteGraph B, BipartiteGraph genB){

	B.calc_degree_dependent_node_redundancy_coefficient();
	genB.calc_degree_dependent_node_redundancy_coefficient();

	int max_degree = 0;
	int k;
	for(int id=0; id<int(genB.degree_node_redun_coeff.id_to_keys.size()); ++id){
		k = genB.degree_node_redun_coeff.id_to_keys[id];
		max_degree = std::max(max_degree, k);
	}
	const int d_size = max_degree + 1;

	std::vector<double> target_degree_node_redun_coeff(d_size, 0.0);
	std::vector<double> degree_node_redun_coeff(d_size, 0.0);
	double dist = 0;
	double norm = 0;
	for(int k=2; k<d_size; ++k){
		target_degree_node_redun_coeff[k] = double(B.degree_node_redun_coeff.get(k));
		degree_node_redun_coeff[k] = double(genB.degree_node_redun_coeff.get(k));
		dist += std::fabs(target_degree_node_redun_coeff[k] - degree_node_redun_coeff[k]);
		norm += std::fabs(target_degree_node_redun_coeff[k]);
	}

	//printf("Initial normalized distance between target and current degree-dependent redundancy coefficients of the node: %lf\n", double(dist)/norm);

	genB.calc_node_degree();
	genB.calc_edge_size();

	std::vector<int> N_k(d_size, 0);
	for(int v=0; v<genB.N; ++v){
		k = genB.node_degree.get(v);
		N_k[k] += 1;
	}

	std::vector<double> const_coeff(d_size, 0.0);
	for(k=2; k<d_size; ++k){
		if(N_k[k] == 0){continue;}
		const_coeff[k] = double(2)/(k*(k-1));
		const_coeff[k] = double(const_coeff[k])/N_k[k];
	}

	std::vector<std::pair<int,int>> bipartite_edges;
	for(int v=0; v<genB.N; ++v){
		for(int m:genB.elist[v]){
			bipartite_edges.push_back({v, m});
		}
	}
	const int B_M = int(bipartite_edges.size());

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randB_M(0, B_M-1);

	std::vector<double> rewired_degree_node_redun_coeff(d_size, 0.0);
	std::vector<int> degree_node_num_redun_to_add(d_size, 0);

	const int R = R_factor_targeting*B_M;

	dnrc_distance_track.clear();
	dnrc_distance_track.push_back(double(dist)/norm);

	int r, e1, e2, u, v, m1, m2, k1, k2;
	double delta_dist;

	for(r=0; r<R; ++r){
		e1 = randB_M(mt);
		e2 = randB_M(mt);
		u = bipartite_edges[e1].first;
		m1 = bipartite_edges[e1].second;
		v = bipartite_edges[e2].first;
		m2 = bipartite_edges[e2].second;
		k1 = genB.node_degree.get(u);
		k2 = genB.node_degree.get(v);
		while(u == v || m1 == m2 || k1 != k2){
			e1 = randB_M(mt);
			e2 = randB_M(mt);
			u = bipartite_edges[e1].first;
			m1 = bipartite_edges[e1].second;
			v = bipartite_edges[e2].first;
			m2 = bipartite_edges[e2].second;
			k1 = genB.node_degree.get(u);
			k2 = genB.node_degree.get(v);
		}

		degree_node_num_redun_to_add = std::vector<int>(d_size, 0);

		genB.remove_edge(u, m1);
		update_degree_node_redundancy_coefficient_by_edge_deletion(genB, u, m1, k1, degree_node_num_redun_to_add);
		genB.add_edge(u, m2);
		update_degree_node_redundancy_coefficient_by_edge_addition(genB, u, m2, k1, degree_node_num_redun_to_add);
		genB.remove_edge(v, m2);
		update_degree_node_redundancy_coefficient_by_edge_deletion(genB, v, m2, k2, degree_node_num_redun_to_add);
		genB.add_edge(v, m1);
		update_degree_node_redundancy_coefficient_by_edge_addition(genB, v, m1, k2, degree_node_num_redun_to_add);

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
			genB.remove_edge(u, m2);
			genB.add_edge(u, m1);
			genB.remove_edge(v, m1);
			genB.add_edge(v, m2);
		}

		dnrc_distance_track.push_back(double(dist)/norm);
	}

	//printf("Final normalized distance between target and current degree-dependent redundancy coefficients of the node: %lf\n", double(dist)/norm);

	return genB;
}

BipartiteGraph Rewiring::randomization_preserving_node_twoK_and_node_oneK_and_edge_oneK(BipartiteGraph B){

	BipartiteGraph genB;
	genB.N = B.N;
	genB.M = B.M;
	genB.elist = std::vector<std::vector<int>>(genB.N, std::vector<int>());
	genB.vlist = std::vector<std::vector<int>>(genB.M, std::vector<int>());
	for(int v=0; v<genB.N; ++v){
		for(int m:B.elist[v]){
			genB.elist[v].push_back(m);
			genB.vlist[m].push_back(v);
		}
	}

	std::vector<std::pair<int, int>> bipartite_edges;
	for(int v=0; v<B.N; ++v){
		for(int m:genB.elist[v]){
			bipartite_edges.push_back({v, m});
		}
	}

	const int B_M = int(bipartite_edges.size());

	std::random_device rd;
  	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randB_M(0, B_M-1);

	genB.calc_node_degree();

	const int R = R_factor_randomization*B_M;

	int intersection_size = B_M;
	int union_size = B_M;

	TwoDimProp<int> B_im, genB_im;
	for(int v=0; v<B.N; ++v){
		for(int m:B.elist[v]){
			B_im.add(v, m, 1);
		}
		for(int m:genB.elist[v]){
			genB_im.add(v, m, 1);
		}
	}

	jaccard_index_track.clear();
	jaccard_index_track.push_back(double(intersection_size)/(union_size));

	int r, e1, e2, u, v, m1, m2, k1, k2;
	for(r=0; r<R; ++r){

		e1 = randB_M(mt);
		e2 = randB_M(mt);
		u = bipartite_edges[e1].first;
		m1 = bipartite_edges[e1].second;
		v = bipartite_edges[e2].first;
		m2 = bipartite_edges[e2].second;
		k1 = genB.node_degree.get(u);
		k2 = genB.node_degree.get(v);
		while(u == v || m1 == m2 || k1 != k2){
			e1 = randB_M(mt);
			e2 = randB_M(mt);
			u = bipartite_edges[e1].first;
			m1 = bipartite_edges[e1].second;
			v = bipartite_edges[e2].first;
			m2 = bipartite_edges[e2].second;
			k1 = genB.node_degree.get(u);
			k2 = genB.node_degree.get(v);
		}

		if(genB_im.get(u, m1) == 1){
			if(B_im.get(u, m1) > 0){
				intersection_size -= 1;
			}
			else{
				union_size -= 1;
			}
		}
		genB_im.subtract(u, m1, 1);

		if(genB_im.get(u, m2) == 0){
			if(B_im.get(u, m2) > 0){
				intersection_size += 1;
			}
			else{
				union_size += 1;
			}
		}
		genB_im.add(u, m2, 1);

		if(genB_im.get(v, m2) == 1){
			if(B_im.get(v, m2) > 0){
				intersection_size -= 1;
			}
			else{
				union_size -= 1;
			}
		}
		genB_im.subtract(v, m2, 1);

		if(genB_im.get(v, m1) == 0){
			if(B_im.get(v, m1) > 0){
				intersection_size += 1;
			}
			else{
				union_size += 1;
			}
		}
		genB_im.add(v, m1, 1);

		bipartite_edges[e1] = {u, m2};
		bipartite_edges[e2] = {v, m1};
		genB.remove_edge(u, m1);
		genB.add_edge(u, m2);
		genB.remove_edge(v, m2);
		genB.add_edge(v, m1);

		jaccard_index_track.push_back(double(intersection_size)/(union_size));
	}

	return genB;
}
