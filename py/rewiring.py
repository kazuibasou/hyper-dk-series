import hypergraph
import math
import random
from collections import defaultdict

def targeting_rewiring_d_v_two(G, randG):
	print("Started targeting-rewiring process with d_v = 2.")

	target_num_jnt_node_deg = G.num_jnt_node_deg()

	target_sum_num_jnt_node_deg = 0
	for e in G.E:
		s = len(e)
		target_sum_num_jnt_node_deg += s*(s-1)

	num_jnt_node_deg = randG.num_jnt_node_deg()

	sum_num_jnt_node_deg = 0
	for e in randG.E:
		s = len(e)
		sum_num_jnt_node_deg += s*(s-1)

	G_node_degree = G.node_degree()
	randG_node_degree = randG.node_degree()
	randG_hyperedge_size = randG.hyperedge_size()

	node_degree_set = set()
	for v in G.V:
		k = len(G.elist[v])
		node_degree_set.add(k)

	dist = 0
	norm = 0
	for k1 in node_degree_set:
		for k2 in node_degree_set:
			dist += math.fabs(float(target_num_jnt_node_deg[k1][k2])/target_sum_num_jnt_node_deg - float(num_jnt_node_deg[k1][k2])/sum_num_jnt_node_deg)
			norm += float(target_num_jnt_node_deg[k1][k2])/target_sum_num_jnt_node_deg

	print("Initial L1 distance between the target and present P_v(k, k'): ", float(dist)/norm)

	bipartite_edges = []
	for v in randG.V:
		for e_i in randG.elist[v]:
			bipartite_edges.append([v, e_i])

	B_M = len(bipartite_edges)
	R = 500*B_M

	for r in range(0, R):

		e1 = random.randrange(0, B_M)
		e2 = random.randrange(0, B_M)
		[v1, e_i] = bipartite_edges[e1]
		[v2, e_j] = bipartite_edges[e2]
		while(v1 == v2 or e_i == e_j):
			e1 = random.randrange(0, B_M)
			e2 = random.randrange(0, B_M)
			[v1, e_i] = bipartite_edges[e1]
			[v2, e_j] = bipartite_edges[e2]

		k1 = randG_node_degree[v1]
		k2 = randG_node_degree[v2]

		num_jnt_node_deg_to_add = defaultdict(lambda: defaultdict(int))

		for w in randG.E[e_i]:
			l = randG_node_degree[w]
			num_jnt_node_deg_to_add[k1][l] -= 1
			num_jnt_node_deg_to_add[l][k1] -= 1
		num_jnt_node_deg_to_add[k1][k1] += 2

		for w in randG.E[e_j]:
			l = randG_node_degree[w]
			num_jnt_node_deg_to_add[k1][l] += 1
			num_jnt_node_deg_to_add[l][k1] += 1

		for w in randG.E[e_j]:
			l = randG_node_degree[w]
			num_jnt_node_deg_to_add[k2][l] -= 1
			num_jnt_node_deg_to_add[l][k2] -= 1
		num_jnt_node_deg_to_add[k2][k2] += 2
		num_jnt_node_deg_to_add[k1][k2] -= 1
		num_jnt_node_deg_to_add[k2][k1] -= 1

		for w in randG.E[e_i]:
			l = randG_node_degree[w]
			num_jnt_node_deg_to_add[k2][l] += 1
			num_jnt_node_deg_to_add[l][k2] += 1
		num_jnt_node_deg_to_add[k1][k2] -= 1
		num_jnt_node_deg_to_add[k2][k1] -= 1

		rewired_dist = dist
		for k in num_jnt_node_deg_to_add:
			for l in num_jnt_node_deg_to_add:
				x = math.fabs(float(target_num_jnt_node_deg[k][l])/target_sum_num_jnt_node_deg - float(num_jnt_node_deg[k][l] + num_jnt_node_deg_to_add[k][l])/sum_num_jnt_node_deg)
				y = math.fabs(float(target_num_jnt_node_deg[k][l])/target_sum_num_jnt_node_deg - float(num_jnt_node_deg[k][l])/sum_num_jnt_node_deg)
				rewired_dist += x - y

		delta_dist = rewired_dist - dist

		if delta_dist >= 0:
			continue

		bipartite_edges[e1] = [v1, e_j]
		bipartite_edges[e2] = [v2, e_i]

		randG.remove_node_from_hyperedge(v1, e_i)
		randG.add_node_to_hyperedge(v1, e_j)
		randG.remove_node_from_hyperedge(v2, e_j)
		randG.add_node_to_hyperedge(v2, e_i)

		for k in num_jnt_node_deg_to_add:
			for l in num_jnt_node_deg_to_add:
				num_jnt_node_deg[k][l] += num_jnt_node_deg_to_add[k][l]

		dist = rewired_dist

	print("Final L1 distance between target and current P_v(k, k'): ", float(dist)/norm, "\n")

	return randG

def update_degree_node_redundancy_coefficient_by_edge_addition(randG, v, e_i, k, randG_node_degree):
	degree_node_num_redun_to_add = defaultdict(int)

	checked = defaultdict(int)
	for e_j in randG.elist[v]:
		checked[e_j] = 0

	s2 = set([v])

	for e_j in randG.elist[v]:
		if len(randG.E[e_j]) < 2 or e_j == e_i:
			continue

		s1 = set(randG.E[e_i]) & set(randG.E[e_j])
		s3 = s1 - s2

		if len(s3) == 0:
			continue

		degree_node_num_redun_to_add[k] += 1

		if checked[e_j] == 1:
			continue

		for w in s3:
			s4 = set([w])
			s5 = s1 - s4

			if s5 == s2 and randG.E[e_i].count(v) == 1:
				l = randG_node_degree[w]
				x = randG.elist[w].count(e_i)
				y = randG.elist[w].count(e_j)
				degree_node_num_redun_to_add[l] += x*y

		checked[e_j] = 1

	return degree_node_num_redun_to_add

def update_degree_node_redundancy_coefficient_by_edge_deletion(randG, v, e_i, k, randG_node_degree):
	degree_node_num_redun_to_add = defaultdict(int)

	checked = defaultdict(int)
	for e_j in randG.elist[v]:
		checked[e_j] = 0

	s2 = set([v])

	for e_j in randG.elist[v]:
		if len(randG.E[e_j]) < 2 or e_j == e_i:
			continue

		s1 = set(randG.E[e_i]) & set(randG.E[e_j])
		s3 = s1 - s2

		if len(s3) == 0:
			continue

		degree_node_num_redun_to_add[k] -= 1

		if checked[e_j] == 1:
			continue

		for w in s3:
			s4 = set([w])
			s5 = s1 - s4

			if s5 == s2 and randG.E[e_i].count(v) == 1:
				l = randG_node_degree[w]
				x = randG.elist[w].count(e_i)
				y = randG.elist[w].count(e_j)
				degree_node_num_redun_to_add[l] -= x*y

		checked[e_j] = 1

	return degree_node_num_redun_to_add

def targeting_rewiring_d_v_two_five(G, randG):

	print("Started targeting-rewiring process with d_v = 2.5.")

	target_degree_node_redun_coeff = G.degree_dependent_node_redundancy_coefficient()
	degree_node_redun_coeff = randG.degree_dependent_node_redundancy_coefficient()

	max_node_deg = randG.maximum_node_degree()
	d_size = max_node_deg + 1

	dist = 0
	norm = 0
	for k in range(2, d_size):
		dist += math.fabs(target_degree_node_redun_coeff[k] - degree_node_redun_coeff[k])
		norm += math.fabs(target_degree_node_redun_coeff[k])

	print("Initial L1 distance between target and present r(k): " + str(float(dist)/norm))

	randG_node_degree = randG.node_degree()
	randG_hyperedge_size = randG.hyperedge_size()

	N_k = [0]*d_size
	for v in randG.V:
		k = randG_node_degree[v]
		N_k[k] += 1

	const_coeff = [0]*d_size
	for k in range(2, d_size):
		if N_k[k] > 0:
			const_coeff[k] = float(2)/(k*(k-1))
			const_coeff[k] = float(const_coeff[k])/N_k[k]

	bipartite_edges = []
	for v in randG.V:
		for e_i in randG.elist[v]:
			bipartite_edges.append([v, e_i])

	B_M = len(bipartite_edges)

	rewired_degree_node_redun_coeff = [0]*d_size
	degree_node_num_redun_to_add = [0]*d_size

	R = 500*B_M

	for r in range(0, R):
		e1 = random.randrange(0, B_M)
		[u, e_i] = bipartite_edges[e1]
		e2 = random.randrange(0, B_M)
		[v, e_j] = bipartite_edges[e2]
		k1 = randG_node_degree[u]
		k2 = randG_node_degree[v]

		while u == v or e_i == e_j or k1 != k2:
			e1 = random.randrange(0, B_M)
			[u, e_i] = bipartite_edges[e1]
			e2 = random.randrange(0, B_M)
			[v, e_j] = bipartite_edges[e2]
			k1 = randG_node_degree[u]
			k2 = randG_node_degree[v]

		degree_node_num_redun_to_add = [0]*d_size

		randG.remove_node_from_hyperedge(u, e_i)
		degree_node_num_redun_to_add = update_degree_node_redundancy_coefficient_by_edge_deletion(randG, u, e_i, k1, randG_node_degree)
		randG.add_node_to_hyperedge(u, e_j)
		degree_node_num_redun_to_add = update_degree_node_redundancy_coefficient_by_edge_addition(randG, u, e_j, k1, randG_node_degree)
		randG.remove_node_from_hyperedge(v, e_j)
		degree_node_num_redun_to_add = update_degree_node_redundancy_coefficient_by_edge_deletion(randG, v, e_j, k2, randG_node_degree)
		randG.add_node_to_hyperedge(v, e_i)
		degree_node_num_redun_to_add = update_degree_node_redundancy_coefficient_by_edge_addition(randG, v, e_i, k2, randG_node_degree)

		delta_dist = 0

		for k in range(2, d_size):
			if degree_node_num_redun_to_add[k] != 0:
				new_k = math.fabs(target_degree_node_redun_coeff[k] - float(degree_node_redun_coeff[k] + degree_node_num_redun_to_add[k]*const_coeff[k]))
				old_k = math.fabs(target_degree_node_redun_coeff[k] - degree_node_redun_coeff[k])
				delta_dist += new_k - old_k

		if delta_dist < 0:
			bipartite_edges[e1] = [u, e_j]
			bipartite_edges[e2] = [v, e_i]
			
			dist += delta_dist
			
			for k in range(2, d_size):
				if degree_node_num_redun_to_add[k] != 0:
					degree_node_redun_coeff[k] += float(degree_node_num_redun_to_add[k]*const_coeff[k])
		else:
			randG.remove_node_from_hyperedge(u, e_j)
			randG.add_node_to_hyperedge(u, e_i)
			randG.remove_node_from_hyperedge(v, e_i)
			randG.add_node_to_hyperedge(v, e_j)

	print("Final L1 distance between target and present r(k): " + str(float(dist)/norm) + "\n")

	return randG