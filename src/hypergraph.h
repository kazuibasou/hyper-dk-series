#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

class HyperGraph{
	public:
		int N;
		int M;
		std::vector<std::vector<int>> elist;
		std::vector<std::vector<int>> vlist;
		int max_node_deg;

		//Properties
		std::vector<int> node_degree;
		Matrix<int> num_jnt_node_deg;
		std::vector<double> node_redun_coeff;
		std::vector<double> degree_node_redun_coeff;
		std::vector<int> edge_size;

		HyperGraph();
		~HyperGraph();
		int read_hypergraph(const char *graphname);

		int add_node_to_hyperedge(const int &v, const int &m);
		int remove_node_from_hyperedge(const int &v, const int &m);

		HyperGraph return_largest_connected_component() const;
		HyperGraph remove_hyperedges_less_than_two_size() const;

		int calc_node_degree();
		int calc_num_jnt_node_deg();
		int calc_node_redundancy_coefficient();
		int calc_degree_dependent_node_redundancy_coefficient();
		int calc_edge_size();
};

#endif