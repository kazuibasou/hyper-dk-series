from lib import hyper_dk_series

if __name__ == '__main__':
    G = hyper_dk_series.HyperGraph()
    hypergraph_name = "example-hypergraph"
    G.read_hypergraph(hypergraph_name)
    print(G.V)

	#rand_G = hyper_dk_series.randomizing_hypergraph(G, 2.5, 1)
	#print(rand_G.N, rand_G.M)
	#print(rand_G.elist)