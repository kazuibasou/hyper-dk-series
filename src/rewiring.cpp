#include <vector>
#include <random>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "basic_function.h"
#include "hypergraph.h"
#include "rewiring.h"

HyperGraph targeting_rewiring_d_v_two(HyperGraph G, HyperGraph rand_G){

	G.calc_num_jnt_node_deg();
	G.calc_sum_num_jnt_node_deg();
	const Matrix<int> target_num_jnt_node_deg = Matrix<int>(G.num_jnt_node_deg);
	const int target_sum_num_jnt_node_deg = G.sum_num_jnt_node_deg;

	rand_G.calc_num_jnt_node_deg();
	rand_G.calc_sum_num_jnt_node_deg();
	Matrix<int> num_jnt_node_deg = Matrix<int>(rand_G.num_jnt_node_deg);
	const int sum_num_jnt_node_deg = rand_G.sum_num_jnt_node_deg;

	G.calc_node_degree();
	rand_G.calc_node_degree();
	rand_G.calc_edge_size();

	std::set<int> degrees;
	for(int v=0; v<G.N; ++v){
		degrees.insert(G.node_degree.get(v));
	}
	for(int v=0; v<rand_G.N; ++v){
		degrees.insert(rand_G.node_degree.get(v));
	}

	double dist = 0;
	double norm = 0;
	for(int k:degrees){
		for(int l:degrees){
			dist += std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg - double(num_jnt_node_deg.get(k, l))/sum_num_jnt_node_deg);
			norm += std::fabs(double(target_num_jnt_node_deg.get(k, l))/target_sum_num_jnt_node_deg);
		}
	}

	printf("Initial L1 distance between the target and current pairwise joint degree distributions of the node: %lf\n", double(dist)/norm);

	std::vector<std::pair<int,int>> bipartite_edges;
	for(int v=0; v<rand_G.N; ++v){
		for(int m:rand_G.elist[v]){
			bipartite_edges.push_back({v, m});
		}
	}

	int B_M = int(bipartite_edges.size());

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

		k1 = rand_G.node_degree.get(u);
		k2 = rand_G.node_degree.get(v);

		Matrix<int> num_jnt_node_deg_to_add;

		for(int w:rand_G.vlist[m1]){
			l = rand_G.node_degree.get(w);
			num_jnt_node_deg_to_add.subtract(k1, l, 1);
			num_jnt_node_deg_to_add.subtract(l, k1, 1);
		}
		num_jnt_node_deg_to_add.add(k1, k1, 2);

		for(int w:rand_G.vlist[m2]){
			l = rand_G.node_degree.get(w);
			num_jnt_node_deg_to_add.add(k1, l, 1);
			num_jnt_node_deg_to_add.add(l, k1, 1);
		}

		for(int w:rand_G.vlist[m2]){
			l = rand_G.node_degree.get(w);
			num_jnt_node_deg_to_add.subtract(k2, l, 1);
			num_jnt_node_deg_to_add.subtract(l, k2, 1);
		}
		num_jnt_node_deg_to_add.add(k2, k2, 2);
		num_jnt_node_deg_to_add.subtract(k1, k2, 1);
		num_jnt_node_deg_to_add.subtract(k2, k1, 1);

		for(int w:rand_G.vlist[m1]){
			l = rand_G.node_degree.get(w);
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

			rand_G.remove_node_from_hyperedge(u, m1);
			rand_G.add_node_to_hyperedge(u, m2);
			rand_G.remove_node_from_hyperedge(v, m2);
			rand_G.add_node_to_hyperedge(v, m1);

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

	printf("Final L1 distance between target and current pairwise joint degree distributions of the node: %lf\n", double(dist)/norm);

	return rand_G;
}

int update_degree_node_redundancy_coefficient_by_edge_addition(HyperGraph &rand_G, 
	const int &v, const int &m, const int &k, std::vector<int> &degree_node_num_redun_to_add){

	std::unordered_map<int, int> checked;
	for(int m2:rand_G.elist[v]){
		checked[m2] = 0;
	}

	std::set<int> s1, s3, s4, s5;
	std::set<int> s2 = {v};
	int x, y, l;

	for(int m2: rand_G.elist[v]){
		if(int(rand_G.vlist[m2].size()) < 2 || m2 == m){continue;}

		s1.clear();
		s3.clear();
		std::set_intersection(rand_G.vlist[m].begin(), rand_G.vlist[m].end(), rand_G.vlist[m2].begin(), rand_G.vlist[m2].end(), std::inserter(s1, s1.end()));
		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));
		
		if(int(s3.size()) == 0){continue;}
		
		degree_node_num_redun_to_add[k] += 1;

		if(checked[m2] == 1){continue;}

		for(int w:s3){
			s4 = {w};
			s5.clear();
			std::set_difference(s1.begin(), s1.end(), s4.begin(), s4.end(), std::inserter(s5, s5.end()));
			if(s5 == s2 && std::count(rand_G.vlist[m].begin(), rand_G.vlist[m].end(), v) == 1){
				l = rand_G.node_degree.get(w);
				x = std::count(rand_G.elist[w].begin(), rand_G.elist[w].end(), m);
				y = std::count(rand_G.elist[w].begin(), rand_G.elist[w].end(), m2);
				degree_node_num_redun_to_add[l] += x*y;
			}
		}
		checked[m2] = 1;
	}

	return 0;
}

int update_degree_node_redundancy_coefficient_by_edge_deletion(HyperGraph &rand_G, 
	const int &v, const int &m, const int &k, std::vector<int> &degree_node_num_redun_to_add){

	std::unordered_map<int, int> checked;
	for(int m2:rand_G.elist[v]){
		checked[m2] = 0;
	}

	std::set<int> s1, s3, s4, s5;
	std::set<int> s2 = {v};
	int x, y, l;

	for(int m2: rand_G.elist[v]){
		if(int(rand_G.vlist[m2].size()) < 2 || m2 == m){continue;}

		s1.clear();
		s3.clear();
		std::set_intersection(rand_G.vlist[m].begin(), rand_G.vlist[m].end(), rand_G.vlist[m2].begin(), rand_G.vlist[m2].end(), std::inserter(s1, s1.end()));
		std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(s3, s3.end()));
		
		if(int(s3.size()) == 0){continue;}

		degree_node_num_redun_to_add[k] -= 1;

		if(checked[m2] == 1){continue;}

		for(int w:s3){
			s4 = {w};
			s5.clear();
			std::set_difference(s1.begin(), s1.end(), s4.begin(), s4.end(), std::inserter(s5, s5.end()));
			if(int(s5.size()) == 0){
				l = rand_G.node_degree.get(w);
				x = std::count(rand_G.elist[w].begin(), rand_G.elist[w].end(), m);
				y = std::count(rand_G.elist[w].begin(), rand_G.elist[w].end(), m2);
				degree_node_num_redun_to_add[l] -= x*y;
			}
		}
		checked[m2] = 1;
	}

	return 0;
}

HyperGraph targeting_rewiring_d_v_two_five(HyperGraph G, HyperGraph rand_G){

	G.calc_degree_dependent_node_redundancy_coefficient();
	rand_G.calc_degree_dependent_node_redundancy_coefficient();

	int max_degree = 0;
	int k;
	for(int id=0; id<int(rand_G.degree_node_redun_coeff.id_to_keys.size()); ++id){
		k = rand_G.degree_node_redun_coeff.id_to_keys[id];
		max_degree = std::max(max_degree, k);
	}
	const int d_size = max_degree + 1;

	std::vector<double> target_degree_node_redun_coeff(d_size, 0.0);
	std::vector<double> degree_node_redun_coeff(d_size, 0.0);
	double dist = 0;
	double norm = 0;
	for(int k=2; k<d_size; ++k){
		target_degree_node_redun_coeff[k] = double(G.degree_node_redun_coeff.get(k));
		degree_node_redun_coeff[k] = double(rand_G.degree_node_redun_coeff.get(k));
		dist += std::fabs(target_degree_node_redun_coeff[k] - degree_node_redun_coeff[k]);
		norm += std::fabs(target_degree_node_redun_coeff[k]);
	}

	printf("Initial L1 distance between target and current degree-dependent redundancy coefficient of the node: %lf\n", double(dist)/norm);

	rand_G.calc_node_degree();
	rand_G.calc_edge_size();

	std::vector<int> N_k(d_size, 0);
	for(int v=0; v<rand_G.N; ++v){
		k = rand_G.node_degree.get(v);
		N_k[k] += 1;
	}

	std::vector<double> const_coeff(d_size, 0.0);
	for(k=2; k<d_size; ++k){
		if(N_k[k] == 0){continue;}
		const_coeff[k] = double(2)/(k*(k-1));
		const_coeff[k] = double(const_coeff[k])/N_k[k];
	}

	std::vector<std::pair<int,int>> bipartite_edges;
	for(int v=0; v<rand_G.N; ++v){
		for(int m:rand_G.elist[v]){
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
		k1 = rand_G.node_degree.get(u);
		k2 = rand_G.node_degree.get(v);
		while(u == v || m1 == m2 || k1 != k2){
			e1 = randB_M(mt);
			e2 = randB_M(mt);
			u = bipartite_edges[e1].first;
			m1 = bipartite_edges[e1].second;
			v = bipartite_edges[e2].first;
			m2 = bipartite_edges[e2].second;
			k1 = rand_G.node_degree.get(u);
			k2 = rand_G.node_degree.get(v);
		}

		degree_node_num_redun_to_add = std::vector<int>(d_size, 0);

		rand_G.remove_node_from_hyperedge(u, m1);
		update_degree_node_redundancy_coefficient_by_edge_deletion(rand_G, u, m1, k1, degree_node_num_redun_to_add);
		rand_G.add_node_to_hyperedge(u, m2);
		update_degree_node_redundancy_coefficient_by_edge_addition(rand_G, u, m2, k1, degree_node_num_redun_to_add);
		rand_G.remove_node_from_hyperedge(v, m2);
		update_degree_node_redundancy_coefficient_by_edge_deletion(rand_G, v, m2, k2, degree_node_num_redun_to_add);
		rand_G.add_node_to_hyperedge(v, m1);
		update_degree_node_redundancy_coefficient_by_edge_addition(rand_G, v, m1, k2, degree_node_num_redun_to_add);

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
			rand_G.remove_node_from_hyperedge(u, m2);
			rand_G.add_node_to_hyperedge(u, m1);
			rand_G.remove_node_from_hyperedge(v, m1);
			rand_G.add_node_to_hyperedge(v, m2);
		}
	}

	printf("Final L1 distance between target and current degree-dependent redundancy coefficient of the node: %lf\n", double(dist)/norm);

	return rand_G;
}