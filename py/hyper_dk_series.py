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

def write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG):
	# Write files for a randomized hypergraph named randG. 

	f1_path = "../rand_hypergraph/" + str(hypergraph_name) + "_" + str(d_v) + "_" + str(d_e) + "_" + str(k) + "_nverts.txt"
	f1 = open(f1_path, 'w')

	for e in randG.E:
		f1.write(str(int(len(e))) + "\n")

	f1.close()

	f2_path = "../rand_hypergraph/" + str(hypergraph_name) + "_" + str(d_v) + "_" + str(d_e) + "_" + str(k) + "_hyperedges.txt"
	f2 = open(f2_path, 'w')

	for e in randG.E:
		for v in e:
			f2.write(str(int(v)) + "\n")

	f2.close()

	print("Wrote the " + str(k) + "-th hypergraph randomized with (" + d_v + ", " + d_e + ") at ./hyper_dk_series/rand_hypergraph/.\n")

	return 0

def run(G: hypergraph.HyperGraph, d_v: str, d_e: str, num_gen: int, hypergraph_name: str):
	if d_v == "0" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_zero_d_e_zero(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "1" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_one_d_e_zero(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_two_d_e_zero(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2.5" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_two_five_d_e_zero(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "0" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_zero_d_e_one(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "1" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_one_d_e_one(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_two_d_e_one(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2.5" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = randomizing_d_v_two_five_d_e_one(G)
			write_randomized_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	else:
		print("Error: Given pair (d_v, d_e) is not defined.\n")
		print("The pair (d_v, d_e) should be (0, 0), (1, 0), (2, 0), (2.5, 0), (0, 1), (1, 1), (2, 1), or (2.5, 1).\n")
		exit()

	return