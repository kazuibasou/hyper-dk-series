#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

class BipartiteGraph{
	public:
		int N;
		int M;
		std::vector<std::vector<int>> elist;
		std::vector<std::vector<int>> vlist;

		//Properties
		int LCC_N;
		int LCC_M;
		int M_less_than_two_size;

		OneDimProp<int> node_degree;
		int sum_node_degree;
		double ave_node_degree;
		OneDimProp<double> node_degree_dist;
		std::vector<double> cumulative_node_degree_dist;
		OneDimProp<double> ave_neighbor_node_degree;
		TwoDimProp<int> num_jnt_node_deg;
		TwoDimProp<double> node_joint_degree_dist;
		int sum_num_jnt_node_deg;
		OneDimProp<double> node_redun_coeff;
		double ave_node_redun_coeff;
		OneDimProp<double> degree_node_redun_coeff;
		OneDimProp<double> node_lcc;
		double ave_node_lcc;
		OneDimProp<double> degree_node_lcc;
		double ave_node_shortest_length;
		OneDimProp<double> node_shortest_length_dist;
		OneDimProp<int> edge_size;
		int sum_edge_size;
		double ave_edge_size;
		OneDimProp<double> edge_size_dist;
		std::vector<double> cumulative_edge_size_dist;
		std::vector<double> cumulative_projected_degree_dist;
		int num_multiple_edges;

		BipartiteGraph();
		~BipartiteGraph();
		int read_hypergraph(const char *graphname);

		int add_edge(const int &v, const int &m);
		int remove_edge(const int &v, const int &m);

		BipartiteGraph return_largest_connected_component();
		BipartiteGraph remove_hyperedges_less_than_two_size() const;

		int calc_number_of_nodes();
		int calc_number_of_edges();
		int calc_node_degree();
		int calc_sum_node_degree();
		int calc_average_node_degree();
		int calc_node_degree_distribution();
		int calc_cumulative_node_degree_distribution();
		int calc_average_neighbor_node_degree();
		int calc_num_jnt_node_deg();
		int calc_node_joint_degree_distribution();
		int calc_sum_num_jnt_node_deg();
		int calc_node_redundancy_coefficient();
		int calc_average_node_redundancy_coefficient();
		int calc_degree_dependent_node_redundancy_coefficient();
		int calc_average_node_shortest_path_length();
		int calc_node_shortest_path_length_distribution();
		int calc_edge_size();
		int calc_sum_edge_size();
		int calc_average_edge_size();
		int calc_edge_size_distribution();
		int calc_cumulative_edge_size_distribution();
		int calc_cumulative_projected_degree_distribution();
		int calc_number_of_multiple_edges();
		int calc_number_of_hyperedges_less_than_two_size();

		int calc_properties();
};

#endif