import numpy as np
import hypergraph
import random
import math
from collections import Counter
from numba import jit, types
from collections import defaultdict
import os

def _randomizing_d_v_zero_d_e_zero(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (0, 0).

	B_M = 0
	for v in H.V:
		B_M += len(H.elist[v])

	randH = hypergraph.HyperGraph()
	randH.add_nodes_from(set(H.V))
	randH.add_hyperedges_from([[] for e_i in range(0, len(H.E))])

	for i in range(0, B_M):
		v = random.choice(list(randH.V))
		m = random.randrange(0, len(randH.E))
		randH.add_node_to_hyperedge(v, m)

	#print("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 0).")

	return randH

def _randomizing_d_v_one_d_e_zero(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (1, 0).

	stublist = []
	for v in H.V:
		k = len(H.elist[v])
		for i in range(0, k):
			stublist.append(v)

	random.shuffle(stublist)

	randH = hypergraph.HyperGraph()
	randH.add_nodes_from(set(H.V))
	randH.add_hyperedges_from([[] for e_i in range(0, len(H.E))])

	while len(stublist) > 0:
		v = stublist.pop()
		e_i = random.randrange(0, len(randH.E))
		randH.add_node_to_hyperedge(v, e_i)

	#print("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 0).")

	return randH

def _randomizing_d_v_zero_d_e_one(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (0, 1).

	stublist = []
	for e_i in range(0, len(H.E)):
		s = len(H.E[e_i])
		for i in range(0, s):
			stublist.append(e_i)

	random.shuffle(stublist)

	randH = hypergraph.HyperGraph()
	randH.add_nodes_from(set(H.V))
	randH.add_hyperedges_from([[] for e_i in range(0, len(H.E))])

	while len(stublist) > 0:
		v = random.choice(list(randH.V))
		e_i = stublist.pop()
		randH.add_node_to_hyperedge(v, e_i)

	#print("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 1).")

	return randH

def _randomizing_d_v_one_d_e_one(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (1, 1).

	node_stublist = []
	for v in H.V:
		k = len(H.elist[v])
		for i in range(0, k):
			node_stublist.append(v)

	random.shuffle(node_stublist)

	hyperedge_stublist = []
	for e_i in range(0, len(H.E)):
		s = len(H.E[e_i])
		for i in range(0, s):
			hyperedge_stublist.append(e_i)

	random.shuffle(hyperedge_stublist)

	randH = hypergraph.HyperGraph()
	randH.add_nodes_from(set(H.V))
	randH.add_hyperedges_from([[] for e_i in range(0, len(H.E))])

	while len(node_stublist) > 0 and len(hyperedge_stublist) > 0:
		v = node_stublist.pop()
		e_i = hyperedge_stublist.pop()
		randH.add_node_to_hyperedge(v, e_i)

	#print("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 1).")

	return randH

def _setup_for_d_v_two(H: hypergraph.HyperGraph, randH: hypergraph.HyperGraph):
	B_M = sum(list(H.hyperedge_size().values()))
	H_E = np.zeros(B_M, dtype=int)
	H_E_index = np.zeros(randH.M, dtype=int)
	H_E_size = np.zeros(H.M, dtype=int)
	for m in range(0, H.M):
		H_E_index[m] = np.sum(H_E_size)
		H_E_size[m] = len(H.E[m])
		for k in range(0, len(H.E[m])):
			H_E[H_E_index[m] + k] = H.E[m][k]
	H_node_deg = H.node_degree(ndarray=True)
	H_jnt_node_deg = H.joint_node_degree_matrix(ndarray=True)

	randB_M = sum(list(randH.hyperedge_size().values()))
	randH_E = np.zeros(randB_M, dtype=int)
	randH_E_index = np.zeros(randH.M, dtype=int)
	randH_E_size = np.zeros(randH.M, dtype=int)
	for m in range(0, randH.M):
		randH_E_index[m] = np.sum(randH_E_size)
		randH_E_size[m] = len(randH.E[m])
		for k in range(0, len(randH.E[m])):
			randH_E[randH_E_index[m] + k] = randH.E[m][k]
	randH_node_deg = randH.node_degree(ndarray=True)
	randH_jnt_node_deg = randH.joint_node_degree_matrix(ndarray=True)
	
	if H.N != randH.N:
		print("ERROR: numbers of nodes are not the same between the given hypergraph and randomized one.")
		return
	
	if H.M != randH.M:
		print("ERROR: numbers of hyperedges are not the same between the given hypergraph and randomized one.")
		return

	for i in range(0, H.N):
		if H_node_deg[i] != randH_node_deg[i]:
			print("ERROR: node degree mismatch.")
			return

	if H_E.ndim != 1:
		print("ERROR: hyperedges of the hypergraph mush be one-dimensional array.")
		return

	if randH_E.ndim != 1:
		print("ERROR: hyperedges of the randomized hypergraph mush be one-dimensional array.")
		return

	if H_E_size.shape[0] != randH_E_size.shape[0]:
		print("ERROR: the numbers of hyperedges are not the same between the given hypergraph and randomized one.")
		return

	if H_E.shape[0] != randH_E.shape[0]:
		print("ERROR: the sum of hyperedge sizes are not the same between the given hypergraph and randomized one.")
		return

	if H_jnt_node_deg.shape != randH_jnt_node_deg.shape:
		print("ERROR: joint node degree matrices do not form the same shape between the hypergraph and randomized one.")
		return

	return (H_E_size, H_jnt_node_deg, randH_E, randH_E_index, randH_E_size, randH_node_deg, randH_jnt_node_deg)

@jit(nopython=True)
def _targeting_rewiring_d_v_two(H_E_size: np.ndarray, H_jnt_node_deg: np.ndarray,
								randH_E: np.ndarray, randH_E_index: np.ndarray, randH_E_size: np.ndarray,
								randH_node_deg: np.ndarray, randH_jnt_node_deg: np.ndarray,
								R_coeff=500):

	#print("Started targeting-rewiring process with d_v = 2.")

	M = int(randH_E_size.shape[0])

	H_sum_jnt_node_deg = 0
	for m in range(0, M):
		s = H_E_size[m]
		H_sum_jnt_node_deg += s * (s - 1)

	randH_sum_jnt_node_deg = 0
	for m in range(0, M):
		s = randH_E_size[m]
		randH_sum_jnt_node_deg += s * (s - 1)

	K = H_jnt_node_deg.shape[0]
	dist = np.sum(np.fabs((H_jnt_node_deg/H_sum_jnt_node_deg) - (randH_jnt_node_deg/randH_sum_jnt_node_deg)))
	norm = np.sum(H_jnt_node_deg / H_sum_jnt_node_deg)

	#print(dist_, norm_)

	#print("Initial L1 distance between the target and present P_v(k, k'): ", float(dist) / norm)

	randH_E_index_inverse = np.zeros(randH_E.shape[0], dtype=types.int64)
	for m in range(0, M):
		for k in range(0, randH_E_size[m]):
			randH_E_index_inverse[randH_E_index[m] + k] = m

	B_M = int(randH_E.shape[0])
	R = R_coeff * B_M
	jnt_node_deg_to_add = np.zeros((K, K), dtype=types.int8)

	for r in range(0, R):

		#t_s = time.time()
		e1 = np.random.randint(0, B_M)
		e_i = randH_E_index_inverse[e1]
		v1 = randH_E[e1]
		e2 = np.random.randint(0, B_M)
		e_j = randH_E_index_inverse[e2]
		v2 = randH_E[e2]
		while (v1 == v2 or e_i == e_j):
			e1 = np.random.randint(0, B_M)
			e_i = randH_E_index_inverse[e1]
			v1 = randH_E[e1]
			e2 = np.random.randint(0, B_M)
			e_j = randH_E_index_inverse[e2]
			v2 = randH_E[e2]

		k1 = randH_node_deg[v1]
		k2 = randH_node_deg[v2]

		#t_e = time.time()
		#print("1", t_e - t_s)

		#t_s = time.time()
		jnt_node_deg_to_add.fill(0.0)
		k_l_lst = []

		#t_s = time.time()

		for z in range(0, randH_E_size[e_i]):
			w = randH_E[randH_E_index[e_i] + z]
			l = randH_node_deg[w]
			jnt_node_deg_to_add[k1][l] -= 1
			jnt_node_deg_to_add[l][k1] -= 1
			if [k1, l] not in k_l_lst:
				k_l_lst.append([k1, l])
			if [l, k1] not in k_l_lst:
				k_l_lst.append([l, k1])
		jnt_node_deg_to_add[k1][k1] += 2
		if [k1, k1] not in k_l_lst:
			k_l_lst.append([k1, k1])

		for z in range(0, randH_E_size[e_j]):
			w = randH_E[randH_E_index[e_j] + z]
			l = randH_node_deg[w]
			jnt_node_deg_to_add[k1][l] += 1
			jnt_node_deg_to_add[l][k1] += 1
			if [k1, l] not in k_l_lst:
				k_l_lst.append([k1, l])
			if [l, k1] not in k_l_lst:
				k_l_lst.append([l, k1])

		for z in range(0, randH_E_size[e_j]):
			w = randH_E[randH_E_index[e_j] + z]
			l = randH_node_deg[w]
			jnt_node_deg_to_add[k2][l] -= 1
			jnt_node_deg_to_add[l][k2] -= 1
			if [k2, l] not in k_l_lst:
				k_l_lst.append([k2, l])
			if [l, k2] not in k_l_lst:
				k_l_lst.append([l, k2])
		jnt_node_deg_to_add[k2][k2] += 2
		jnt_node_deg_to_add[k1][k2] -= 1
		jnt_node_deg_to_add[k2][k1] -= 1
		if [k2, k2] not in k_l_lst:
			k_l_lst.append([k2, k2])
		if [k1, k2] not in k_l_lst:
			k_l_lst.append([k1, k2])
		if [k2, k1] not in k_l_lst:
			k_l_lst.append([k2, k1])

		for z in range(0, randH_E_size[e_i]):
			w = randH_E[randH_E_index[e_i] + z]
			l = randH_node_deg[w]
			jnt_node_deg_to_add[k2][l] += 1
			jnt_node_deg_to_add[l][k2] += 1
			if [k2, l] not in k_l_lst:
				k_l_lst.append([k2, l])
			if [l, k2] not in k_l_lst:
				k_l_lst.append([l, k2])
		jnt_node_deg_to_add[k1][k2] -= 1
		jnt_node_deg_to_add[k2][k1] -= 1

		#t_e = time.time()
		#print("2", t_e - t_s)

		#t_s = time.time()
		rewired_dist = dist

		for [k, l] in k_l_lst:
			x = math.fabs(float(H_jnt_node_deg[k][l]) / H_sum_jnt_node_deg
						  - float(randH_jnt_node_deg[k][l] + jnt_node_deg_to_add[k][l]) / randH_sum_jnt_node_deg)
			y = math.fabs(float(H_jnt_node_deg[k][l]) / H_sum_jnt_node_deg
						  - float(randH_jnt_node_deg[k][l]) / randH_sum_jnt_node_deg)
			rewired_dist += x - y

		delta_dist = rewired_dist - dist

		#t_e = time.time()
		#print("3", t_e - t_s)

		if delta_dist >= 0:
			continue

		#t_s = time.time()
		randH_E[e1] = v2
		randH_E[e2] = v1

		randH_jnt_node_deg += jnt_node_deg_to_add

		dist = rewired_dist

		#t_e = time.time()
		#print("4", t_e - t_s)

	#print("Final L1 distance between target and current P_v(k, k'): ", float(dist) / norm, "")

	return randH_E

def _setup_for_d_v_two_five(H: hypergraph.HyperGraph, randH: hypergraph.HyperGraph):
	B_M = sum(list(H.hyperedge_size().values()))
	H_E = np.zeros(B_M, dtype=int)
	H_E_index = np.zeros(randH.M, dtype=int)
	H_E_size = np.zeros(H.M, dtype=int)
	for m in range(0, H.M):
		H_E_index[m] = np.sum(H_E_size)
		H_E_size[m] = len(H.E[m])
		for k in range(0, len(H.E[m])):
			H_E[H_E_index[m] + k] = H.E[m][k]
	H_node_deg = H.node_degree(ndarray=True)
	H_degree_node_redun_coeff = H.degree_dependent_node_redundancy_coefficient(ndarray=True)

	randB_M = sum(list(randH.hyperedge_size().values()))
	randH_E = np.zeros(randB_M, dtype=int)
	randH_E_index = np.zeros(randH.M, dtype=int)
	randH_E_size = np.zeros(randH.M, dtype=int)
	for m in range(0, randH.M):
		randH_E_index[m] = np.sum(randH_E_size)
		randH_E_size[m] = len(randH.E[m])
		for k in range(0, len(randH.E[m])):
			randH_E[randH_E_index[m] + k] = randH.E[m][k]
	randH_elist = np.zeros(randB_M, dtype=int)
	randH_elist_index = np.zeros(randH.N, dtype=int)
	randH_node_deg = np.zeros(randH.N, dtype=int)
	for v in range(0, randH.N):
		randH_elist_index[v] = np.sum(randH_node_deg)
		randH_node_deg[v] = len(randH.elist[v])
		for k in range(0, len(randH.elist[v])):
			randH_elist[randH_elist_index[v] + k] = randH.elist[v][k]
	randH_degree_node_redun_coeff = randH.degree_dependent_node_redundancy_coefficient(ndarray=True)

	if H.N != randH.N:
		print("ERROR: numbers of nodes are not the same between the given hypergraph and randomized one.")
		return

	if H.M != randH.M:
		print("ERROR: numbers of hyperedges are not the same between the given hypergraph and randomized one.")
		return

	if B_M != randB_M:
		print("ERROR: numbers of pairs between nodes and hyperedges are not the same between the given hypergraph and randomized one.")
		return

	for i in range(0, H.N):
		if H_node_deg[i] != randH_node_deg[i]:
			print("ERROR: node degree mismatch.")
			return

	if H_E.ndim != 1:
		print("ERROR: hyperedges of the hypergraph mush be one-dimensional array.")
		return

	if randH_E.ndim != 1:
		print("ERROR: hyperedges of the randomized hypergraph mush be one-dimensional array.")
		return

	if H_E_size.shape[0] != randH_E_size.shape[0]:
		print("ERROR: the numbers of hyperedges are not the same between the given hypergraph and randomized one.")
		return

	if H_E.shape[0] != randH_E.shape[0]:
		print("ERROR: the sum of hyperedge sizes are not the same between the given hypergraph and randomized one.")
		return

	return (H_degree_node_redun_coeff, randH_E, randH_E_index, randH_E_size, randH_elist, randH_elist_index, randH_node_deg, randH_degree_node_redun_coeff)

@jit(nopython=True)
def _targeting_rewiring_d_v_two_five(H_degree_node_redun_coeff: np.ndarray,
									 randH_E: np.ndarray, randH_E_index: np.ndarray, randH_E_size: np.ndarray,
									 randH_elist: np.ndarray, randH_elist_index: np.ndarray,
									 randH_node_deg: np.ndarray,
									 randH_degree_node_redun_coeff: np.ndarray,
									 R_coeff=500):

	#print("Started targeting-rewiring process with d_v = 2.5.")

	if H_degree_node_redun_coeff.ndim != 1:
		print("ERROR: hyperedges of the hypergraph mush be one-dimensional array.")
		return

	if randH_degree_node_redun_coeff.ndim != 1:
		print("ERROR: hyperedges of the randomized hypergraph mush be one-dimensional array.")
		return

	if H_degree_node_redun_coeff.shape[0] != randH_degree_node_redun_coeff.shape[0]:
		print("ERROR: maximum degrees are not the same between the hypergraph and randomized one.")
		return

	K = int(H_degree_node_redun_coeff.shape[0])

	dist = np.sum(np.fabs(H_degree_node_redun_coeff - randH_degree_node_redun_coeff))
	#norm = np.sum(np.fabs(H_degree_node_redun_coeff))

	#print("Initial L1 distance between target and present r(k):", float(dist) / norm)

	N = int(randH_node_deg.shape[0])
	M = int(randH_E_size.shape[0])
	N_k = np.zeros(K, dtype=float)
	for v in range(0, N):
		k = randH_node_deg[v]
		if k > 1:
			N_k[k] += 1

	const_coeff = np.zeros(K, dtype=float)
	for k in range(2, K):
		if N_k[k] > 0:
			const_coeff[k] = float(2) / (k * (k - 1))
			const_coeff[k] = float(const_coeff[k]) / N_k[k]

	randH_E_index_inverse = np.zeros(randH_E.shape[0], dtype=types.int64)
	for m in range(0, M):
		for k in range(0, randH_E_size[m]):
			randH_E_index_inverse[randH_E_index[m] + k] = m

	B_M = int(randH_E.shape[0])
	R = R_coeff * B_M
	deg_node_redun_to_add = np.zeros(K, dtype=float)
	checked = np.zeros(M, dtype=types.int8)
	e_included = np.zeros(N, dtype=types.int8)
	s3_included = np.zeros(N, dtype=types.int8)

	for r in range(0, R):
		m1 = np.random.randint(0, B_M)
		e_i = randH_E_index_inverse[m1]
		v_i = randH_E[m1]
		m2 = np.random.randint(0, B_M)
		e_j = randH_E_index_inverse[m2]
		v_j = randH_E[m2]
		k_i = randH_node_deg[v_i]
		k_j = randH_node_deg[v_j]
		while (v_i == v_j or e_i == e_j or k_i != k_j):
			m1 = np.random.randint(0, B_M)
			e_i = randH_E_index_inverse[m1]
			v_i = randH_E[m1]
			m2 = np.random.randint(0, B_M)
			e_j = randH_E_index_inverse[m2]
			v_j = randH_E[m2]
			k_i = randH_node_deg[v_i]
			k_j = randH_node_deg[v_j]

		ii = np.where(randH_elist[randH_elist_index[v_i]: randH_elist_index[v_i] + randH_node_deg[v_i]] == e_i)[0]
		jj = np.where(randH_elist[randH_elist_index[v_j]: randH_elist_index[v_j] + randH_node_deg[v_j]] == e_j)[0]

		deg_node_redun_to_add.fill(0.0)

		randH_E[m1] = -1
		randH_elist[randH_elist_index[v_i] + ii] = -1
		#deg_node_redun_to_add = _update_degree_node_redun_by_edge_deletion(randH_E, randH_E_index,
		#																   randH_E_size, randH_elist,
		#																   randH_elist_index, randH_node_deg,
		#																   v_i, e_i, k_i, N, M, deg_node_redun_to_add)
		
		###
		checked.fill(0)
		#v, e, k = v_i, e_i, k_i
		for i_ in range(0, randH_node_deg[v_i]):
			e_ = randH_elist[randH_elist_index[v_i] + i_]

			if randH_E_size[e_] < 2 or e_ == -1 or e_ == e_i:
				continue

			e_included.fill(0)
			for j_ in range(0, randH_E_size[e_]):
				w = randH_E[randH_E_index[e_] + j_]
				if w != -1:
					e_included[w] = 1

			s3 = []
			s3_included.fill(0)
			for j_ in range(0, randH_E_size[e_i]):
				w = randH_E[randH_E_index[e_i] + j_]
				if e_included[w] == 1 and s3_included[w] == 0 and w != v_i and w != -1:
					s3.append(w)
					s3_included[w] = 1

			if len(s3) == 0:
				continue

			deg_node_redun_to_add[k_i] -= 1.0

			if checked[e_] == 1:
				continue

			if len(s3) == 1:
				w = s3[0]
				l = randH_node_deg[w]
				lst = randH_elist[randH_elist_index[w]: randH_elist_index[w] + l]
				x = np.count_nonzero(lst == e_i)
				y = np.count_nonzero(lst == e_)
				deg_node_redun_to_add[l] -= float(x * y)

			checked[e_] = 1
		###

		randH_E[m2] = -1
		randH_elist[randH_elist_index[v_j] + jj] = -1
		#deg_node_redun_to_add = _update_degree_node_redun_by_edge_deletion(randH_E, randH_E_index,
		#																   randH_E_size, randH_elist,
		#																   randH_elist_index, randH_node_deg,
		#																   v_j, e_j, k_j, N, M, deg_node_redun_to_add)

		###
		checked.fill(0)
		# v, e, k = v_i, e_i, k_i
		for i_ in range(0, randH_node_deg[v_j]):
			e_ = randH_elist[randH_elist_index[v_j] + i_]

			if randH_E_size[e_] < 2 or e_ == -1 or e_ == e_j:
				continue

			e_included.fill(0)
			for j_ in range(0, randH_E_size[e_]):
				w = randH_E[randH_E_index[e_] + j_]
				if w != -1:
					e_included[w] = 1

			s3 = []
			s3_included.fill(0)
			for j_ in range(0, randH_E_size[e_j]):
				w = randH_E[randH_E_index[e_j] + j_]
				if e_included[w] == 1 and s3_included[w] == 0 and w != v_j and w != -1:
					s3.append(w)
					s3_included[w] = 1

			if len(s3) == 0:
				continue

			deg_node_redun_to_add[k_j] -= 1.0

			if checked[e_] == 1:
				continue

			if len(s3) == 1:
				w = s3[0]
				l = randH_node_deg[w]
				lst = randH_elist[randH_elist_index[w]: randH_elist_index[w] + l]
				x = np.count_nonzero(lst == e_j)
				y = np.count_nonzero(lst == e_)
				deg_node_redun_to_add[l] -= float(x * y)

			checked[e_] = 1
		###

		randH_E[m2] = v_i
		randH_elist[randH_elist_index[v_i] + ii] = e_j
		#deg_node_redun_to_add = _update_degree_node_redun_by_edge_addition(randH_E, randH_E_index,
		#																   randH_E_size, randH_elist,
		#																   randH_elist_index, randH_node_deg,
		#																   v_i, e_j, k_i, N, M, deg_node_redun_to_add)

		###
		checked.fill(0)
		# v, e, k = v_i, e_j, k_i
		for i_ in range(0, randH_node_deg[v_i]):
			e_ = randH_elist[randH_elist_index[v_i] + i_]

			if randH_E_size[e_] < 2 or e_ == -1 or e_ == e_j:
				continue

			e_included.fill(0)
			for j_ in range(0, randH_E_size[e_]):
				w = randH_E[randH_E_index[e_] + j_]
				if w != -1:
					e_included[w] = 1

			s3 = []
			s3_included.fill(0)
			for j_ in range(0, randH_E_size[e_j]):
				w = randH_E[randH_E_index[e_j] + j_]
				if e_included[w] == 1 and s3_included[w] == 0 and w != v_i and w != -1:
					s3.append(w)
					s3_included[w] = 1

			if len(s3) == 0:
				continue

			deg_node_redun_to_add[k_i] += 1

			if checked[e_] == 1:
				continue

			l = randH_E_size[e_j]
			lst = randH_E[randH_E_index[e_j]: randH_E_index[e_j] + l]

			if len(s3) == 1 and np.count_nonzero(lst == v_i) == 1:
				w = s3[0]
				l = randH_node_deg[w]
				lst = randH_elist[randH_elist_index[w]: randH_elist_index[w] + l]
				x = np.count_nonzero(lst == e_j)
				y = np.count_nonzero(lst == e_)
				deg_node_redun_to_add[l] += float(x * y)

			checked[e_] = 1
		###

		randH_E[m1] = v_j
		randH_elist[randH_elist_index[v_j] + jj] = e_i
		#deg_node_redun_to_add = _update_degree_node_redun_by_edge_addition(randH_E, randH_E_index,
		#																   randH_E_size, randH_elist,
		#																   randH_elist_index, randH_node_deg,
		#																   v_j, e_i, k_j, N, M, deg_node_redun_to_add)

		###
		checked.fill(0)
		# v, e, k = v_j, e_i, k_j
		for i_ in range(0, randH_node_deg[v_j]):
			e_ = randH_elist[randH_elist_index[v_j] + i_]

			if randH_E_size[e_] < 2 or e_ == -1 or e_ == e_i:
				continue

			e_included.fill(0)
			for j_ in range(0, randH_E_size[e_]):
				w = randH_E[randH_E_index[e_] + j_]
				if w != -1:
					e_included[w] = 1

			s3 = []
			s3_included.fill(0)
			for j_ in range(0, randH_E_size[e_i]):
				w = randH_E[randH_E_index[e_i] + j_]
				if e_included[w] == 1 and s3_included[w] == 0 and w != v_j and w != -1:
					s3.append(w)
					s3_included[w] = 1

			if len(s3) == 0:
				continue

			deg_node_redun_to_add[k_j] += 1

			if checked[e_] == 1:
				continue

			l = randH_E_size[e_i]
			lst = randH_E[randH_E_index[e_i]: randH_E_index[e_i] + l]

			if len(s3) == 1 and np.count_nonzero(lst == v_j) == 1:
				w = s3[0]
				l = randH_node_deg[w]
				lst = randH_elist[randH_elist_index[w]: randH_elist_index[w] + l]
				x = np.count_nonzero(lst == e_i)
				y = np.count_nonzero(lst == e_)
				deg_node_redun_to_add[l] += float(x * y)

			checked[e_] = 1
		###

		to_add = np.multiply(deg_node_redun_to_add, const_coeff)
		rewired_dist = np.sum(np.fabs(H_degree_node_redun_coeff - (randH_degree_node_redun_coeff + to_add)))
		delta_dist = rewired_dist - dist

		if delta_dist >= 0:
			randH_E[m1] = v_i
			randH_elist[randH_elist_index[v_i] + ii] = e_i
			randH_E[m2] = v_j
			randH_elist[randH_elist_index[v_j] + jj] = e_j
		else:
			randH_degree_node_redun_coeff += to_add
			dist = rewired_dist

	#print("Final L1 distance between target and present r(k):", float(dist) / norm)

	return randH_E

def _randomizing_d_v_two_d_e_zero(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2, 0).

	randH = _randomizing_d_v_one_d_e_zero(H)
	#randH = _targeting_rewiring_d_v_two_(H, randH)

	(H_E_size, H_jnt_node_deg, randH_E, randH_E_index, randH_E_size, randH_node_deg, randH_jnt_node_deg) = _setup_for_d_v_two(H, randH)
	randH_E_ = _targeting_rewiring_d_v_two(H_E_size, H_jnt_node_deg, randH_E, randH_E_index, randH_E_size, randH_node_deg, randH_jnt_node_deg)
	randH_E = []
	for m in range(0, randH.M):
		if m < randH.M - 1:
			e = list(randH_E_[randH_E_index[m]: randH_E_index[m+1]])
		else:
			e = list(randH_E_[randH_E_index[m]: len(randH_E_)])
		randH_E.append(e)
	randH = hypergraph.HyperGraph()
	randH.add_hyperedges_from(randH_E)

	#print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 0).")

	return randH

def _randomizing_d_v_two_five_d_e_zero(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2.5, 0).

	randH = _randomizing_d_v_two_d_e_zero(H)

	(H_degree_node_redun_coeff, randH_E, randH_E_index, randH_E_size, randH_elist, randH_elist_index,
	 randH_node_deg, randH_degree_node_redun_coeff) = _setup_for_d_v_two_five(H, randH)
	randH_E_ = _targeting_rewiring_d_v_two_five(H_degree_node_redun_coeff, randH_E, randH_E_index, randH_E_size,
												randH_elist, randH_elist_index, randH_node_deg,
												randH_degree_node_redun_coeff)
	randH_E = []
	for m in range(0, randH.M):
		e = randH_E_[randH_E_index[m]: randH_E_index[m] + randH_E_size[m]].tolist()
		randH_E.append(e)
	randH = hypergraph.HyperGraph()
	randH.add_hyperedges_from(randH_E)

	#print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 0).")

	return randH

def _randomizing_d_v_two_d_e_one(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2, 1).

	randH = _randomizing_d_v_one_d_e_one(H)

	(H_E_size, H_jnt_node_deg, randH_E, randH_E_index, randH_E_size, randH_node_deg,
	 randH_jnt_node_deg) = _setup_for_d_v_two(H, randH)
	randH_E_ = _targeting_rewiring_d_v_two(H_E_size, H_jnt_node_deg, randH_E, randH_E_index, randH_E_size,
										   randH_node_deg, randH_jnt_node_deg)
	randH_E = []
	for m in range(0, randH.M):
		if m < randH.M - 1:
			e = list(randH_E_[randH_E_index[m]: randH_E_index[m + 1]])
		else:
			e = list(randH_E_[randH_E_index[m]: len(randH_E_)])
		randH_E.append(e)
	randH = hypergraph.HyperGraph()
	randH.add_hyperedges_from(randH_E)

	# print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 0).")

	return randH

def _randomizing_d_v_two_five_d_e_one(H):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2.5, 1).

	randH = _randomizing_d_v_two_d_e_one(H)

	(H_degree_node_redun_coeff, randH_E, randH_E_index, randH_E_size, randH_elist, randH_elist_index,
	 randH_node_deg, randH_degree_node_redun_coeff) = _setup_for_d_v_two_five(H, randH)
	randH_E_ = _targeting_rewiring_d_v_two_five(H_degree_node_redun_coeff, randH_E, randH_E_index, randH_E_size,
												randH_elist, randH_elist_index, randH_node_deg,
												randH_degree_node_redun_coeff)
	randH_E = []
	for m in range(0, randH.M):
		e = randH_E_[randH_E_index[m]: randH_E_index[m] + randH_E_size[m]].tolist()
		randH_E.append(e)
	randH = hypergraph.HyperGraph()
	randH.add_hyperedges_from(randH_E)

	# print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 0).")

	return randH

def hyper_dk(H: hypergraph.HyperGraph, d_v: str, d_e: str):
	if d_v == "0" and d_e == "0":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_zero_d_e_zero(H)
	elif d_v == "1" and d_e == "0":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_one_d_e_zero(H)
	elif d_v == "2" and d_e == "0":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_two_d_e_zero(H)
	elif d_v == "2.5" and d_e == "0":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_two_five_d_e_zero(H)
	elif d_v == "0" and d_e == "1":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_zero_d_e_one(H)
	elif d_v == "1" and d_e == "1":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_one_d_e_one(H)
	elif d_v == "2" and d_e == "1":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_two_d_e_one(H)
	elif d_v == "2.5" and d_e == "1":
		#print("Started " + str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
		randH = _randomizing_d_v_two_five_d_e_one(H)
	else:
		print("Error: Given pair (d_v, d_e) is not defined.\n")
		print(
			"The pair (d_v, d_e) must be (0, 0), (1, 0), (2, 0), (2.5, 0), (0, 1), (1, 1), (2, 1), or (2.5, 1).\n")
		exit()

	return randH

def B2K(H: hypergraph.HyperGraph):

	stublist = []
	for m in range(0, H.M):
		e = H.E[m]
		s = len(e)
		for v in e:
			d = H.node_degree(v)
			stublist.append((d, s))

	random.shuffle(stublist)

	degree_stublist = defaultdict(list)
	for v in H.nodes():
		d = H.node_degree(v)
		for i in range(0, d):
			degree_stublist[d].append(v)

	for d in degree_stublist:
		random.shuffle(degree_stublist[d])

	size_stublist = defaultdict(list)
	for m in range(0, H.M):
		s = len(H.E[m])
		for i in range(0, s):
			size_stublist[s].append(m)

	for s in size_stublist:
		random.shuffle(size_stublist[s])

	randH = hypergraph.HyperGraph()
	randH.add_nodes_from(set(H.V))
	randH.add_hyperedges_from([[] for e_i in range(0, len(H.E))])

	while len(stublist) > 0:
		(d, s) = stublist.pop()
		v = degree_stublist[d].pop()
		m = size_stublist[s].pop()
		randH.add_node_to_hyperedge(v, m)

	# print("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 1).")

	return randH

def _L1_dist(G_prop: dict or float or int, randG_prop: dict or float):
	if (type(G_prop) == float and type(randG_prop) == float) or (type(G_prop) == int and type(randG_prop) == int):
		return float(math.fabs(G_prop - randG_prop)) / G_prop
	elif type(G_prop) == dict and type(randG_prop) == dict:
		union_keys = set(G_prop.keys()) | set(randG_prop.keys())
		x = 0
		for key in union_keys:
			x += math.fabs(G_prop.get(key, 0) - randG_prop.get(key, 0))
		y = sum([G_prop[key] for key in G_prop])
		return float(x) / y

def get_L1_distance(H: hypergraph.HyperGraph, randH: hypergraph.HyperGraph):

	node_degree_dist = dict(Counter(list(H.node_degree().values())))
	rand_node_degree_dist = dict(Counter(list(randH.node_degree().values())))

	hyperedge_size_dist = dict(Counter(list(H.hyperedge_size().values())))
	rand_hyperedge_size_dist = dict(Counter(list(randH.hyperedge_size().values())))

	joint_node_degree_dist = dict(H.joint_node_degree_distribution())
	rand_joint_node_degree_dist = dict(randH.joint_node_degree_distribution())

	degree_node_redun_dist = dict(H.degree_dependent_node_redundancy_coefficient())
	rand_degree_node_redun_dist = dict(randH.degree_dependent_node_redundancy_coefficient())

	dist = {
		'Number of nodes': _L1_dist(H.N, randH.N),
		'Number of hyperedges': _L1_dist(H.M, randH.M),
		'Node degree distribution': _L1_dist(node_degree_dist, rand_node_degree_dist),
		'Node degree correlation': _L1_dist(joint_node_degree_dist, rand_joint_node_degree_dist),
		'Node redundancy': _L1_dist(degree_node_redun_dist, rand_degree_node_redun_dist),
		'Hyperedge size distribution': _L1_dist(hyperedge_size_dist, rand_hyperedge_size_dist),
	}

	return dist

def write_randomized_hypergraph(randH, hypergraph_name, d_v, d_e):
	# Write files for a randomized hypergraph named randH.

	if not os.path.exists("./outputs/"):
		os.makedirs("./outputs/")

	f1_path = "outputs/" + str(hypergraph_name) + "_" + str(d_v) + "_" + str(d_e) + "_nverts.txt"
	f1 = open(f1_path, 'w')

	for e in randH.E:
		f1.write(str(int(len(e))) + "\n")

	f1.close()

	f2_path = "outputs/" + str(hypergraph_name) + "_" + str(d_v) + "_" + str(d_e) + "_hyperedges.txt"
	f2 = open(f2_path, 'w')

	for e in randH.E:
		for v in e:
			f2.write(str(int(v)) + "\n")

	f2.close()

	#print("Wrote the " + str(k) + "-th hypergraph randomized with (" + d_v + ", " + d_e + ").\n")

	return
