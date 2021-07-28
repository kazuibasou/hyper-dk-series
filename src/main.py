import hyper_dk_series

if __name__ == '__main__':
	G = hyper_dk_series.HyperGraph()
	hyper_dk_series.read_hypergraph(G, "example-hypergraph")
	print(G.N, G.M)
	print(G.elist)

	rand_G = hyper_dk_series.randomizing_d_v_one_d_e_one(G)
	print(rand_G.N, rand_G.M)
	print(rand_G.elist)