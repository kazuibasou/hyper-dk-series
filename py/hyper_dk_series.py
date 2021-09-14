import hypergraph
import rewiring
import random

def randomizing_d_v_zero_d_e_zero(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (0, 0).

	B_M = 0
	for v in G.V:
		B_M += len(G.elist[v])

	randG = hypergraph.HyperGraph()
	randG.V = list(G.V)
	randG.E = [[] for e_i in range(0, len(G.E))]
	for v in randG.V:
		randG.elist[v] = []

	for i in range(0, B_M):
		v = random.choice(randG.V)
		e_i = random.randrange(0, len(randG.E))
		randG.add_node_to_hyperedge(v, e_i)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 0).\n")

	return randG

def randomizing_d_v_one_d_e_zero(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (1, 0).

	stublist = []
	for v in G.V:
		k = len(G.elist[v])
		for i in range(0, k):
			stublist.append(v)

	random.shuffle(stublist)

	randG = hypergraph.HyperGraph()
	randG.V = list(G.V)
	randG.E = [[] for e_i in range(0, len(G.E))]
	for v in randG.V:
		randG.elist[v] = []

	while len(stublist) > 0:
		v = stublist.pop()
		e_i = random.randrange(0, len(randG.E))
		randG.add_node_to_hyperedge(v, e_i)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 0).\n")

	return randG

def randomizing_d_v_two_d_e_zero(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2, 0).

	randG = hypergraph.HyperGraph()
	
	randG = randomizing_d_v_one_d_e_zero(G)

	randG = rewiring.targeting_rewiring_d_v_two(G, randG)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 0).\n")

	return randG

def randomizing_d_v_two_five_d_e_zero(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2.5, 0).

	randG = hypergraph.HyperGraph()

	randG = randomizing_d_v_two_d_e_zero(G)

	randG = rewiring.targeting_rewiring_d_v_two_five(G, randG)
	
	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 0).\n")

	return randG

def randomizing_d_v_zero_d_e_one(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (0, 1).

	stublist = []
	for e_i in range(0, len(G.E)):
		s = len(G.E[e_i])
		for i in range(0, s):
			stublist.append(e_i)

	random.shuffle(stublist)

	randG = hypergraph.HyperGraph()
	randG.V = list(G.V)
	randG.E = [[] for e_i in range(0, len(G.E))]
	for v in randG.V:
		randG.elist[v] = []

	while len(stublist) > 0:
		v = random.choice(randG.V)
		e_i = stublist.pop()
		randG.add_node_to_hyperedge(v, e_i)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (0, 1).\n")

	return randG

def randomizing_d_v_one_d_e_one(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (1, 1).

	node_stublist = []
	for v in G.V:
		k = len(G.elist[v])
		for i in range(0, k):
			node_stublist.append(v)

	random.shuffle(node_stublist)

	hyperedge_stublist = []
	for e_i in range(0, len(G.E)):
		s = len(G.E[e_i])
		for i in range(0, s):
			hyperedge_stublist.append(e_i)

	random.shuffle(hyperedge_stublist)

	randG = hypergraph.HyperGraph()
	randG.V = list(G.V)
	randG.E = [[] for e_i in range(0, len(G.E))]
	for v in randG.V:
		randG.elist[v] = []

	while len(node_stublist) > 0 and len(hyperedge_stublist) > 0:
		v = node_stublist.pop()
		e_i = hyperedge_stublist.pop()
		randG.add_node_to_hyperedge(v, e_i)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (1, 1).\n")

	return randG

def randomizing_d_v_two_d_e_one(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2, 1).

	randG = hypergraph.HyperGraph()

	randG = randomizing_d_v_one_d_e_one(G)

	randG = rewiring.targeting_rewiring_d_v_two(G, randG)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2, 1).\n")

	return randG

def randomizing_d_v_two_five_d_e_one(G):
	# Given a hypergraph, return a randomized hypergraph with (d_v, d_e) = (2.5, 1).

	randG = hypergraph.HyperGraph()

	randG = randomizing_d_v_two_d_e_one(G)
	
	randG = rewiring.targeting_rewiring_d_v_two_five(G, randG)

	print("Successfully generated a randomized hypergraph with (d_v, d_e) = (2.5, 1).\n")

	return randG