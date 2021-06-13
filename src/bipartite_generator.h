#ifndef BIPARTITE_GENERATOR_H
#define BIPARTITE_GENERATOR_H

class BipartitedKseries{
	public:

		BipartitedKseries(){}
		
		~BipartitedKseries(){}

		BipartiteGraph node_zeroK_edge_zeroK_generator(BipartiteGraph B);
		BipartiteGraph node_oneK_edge_zeroK_generator(BipartiteGraph B);
		BipartiteGraph node_zeroK_edge_oneK_generator(BipartiteGraph B);
		BipartiteGraph node_oneK_edge_oneK_generator(BipartiteGraph B);
		BipartiteGraph bipartite_degree_correlation_generator(BipartiteGraph B);
};

#endif