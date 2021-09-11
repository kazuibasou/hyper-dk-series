import hypergraph
import hyper_dk_series
import math
import sys
import os

os.makedirs("../rand_hypergraph", exist_ok=True)

def write_hypergraph(hypergraph_name, d_v, d_e, k, randG):
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

	if d_v == "0" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_zero_d_e_zero(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "1" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_one_d_e_zero(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_two_d_e_zero(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2.5" and d_e == "0":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_two_five_d_e_zero(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "0" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_zero_d_e_one(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "1" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_one_d_e_one(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_two_d_e_one(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	elif d_v == "2.5" and d_e == "1":
		for k in range(1, num_gen+1):
			print("Started "+ str(k) + "-th randomization with (d_v, d_e) = (" + d_v + ", " + d_e + ").\n")
			randG = hyper_dk_series.randomizing_d_v_two_five_d_e_one(G)
			write_hypergraph(hypergraph_name, d_v, d_e, k, randG)
	else:
		print("Error: Given pair (d_v, d_e) is not defined.\n")
		print("The pair (d_v, d_e) should be (0, 0), (1, 0), (2, 0), (2.5, 0), (0, 1), (1, 1), (2, 1), or (2.5, 1).\n")
		exit()

