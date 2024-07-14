import hypergraph
import hyper_dk_series
import math
import sys
import os

os.makedirs("../rand_hypergraph", exist_ok=True)

def write_hypergraph(hypergraph_name, d_v, d_e, k, randG):
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

if __name__ == "__main__":
	args = sys.argv
	hypergraph_name = str(args[1])
	d_v = str(args[2])
	d_e = str(args[3])
	num_gen = int(args[4])

	G = hypergraph.HyperGraph()
	G.read_hypergraph(hypergraph_name)
	print()

	hyper_dk_series.run(G, d_v, d_e)
