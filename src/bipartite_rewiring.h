#ifndef BIPARTITE_REWIRING_H
#define BIPARTITE_REWIRING_H

int update_degree_node_redundancy_coefficient_by_edge_addition(BipartiteGraph &genB, const int &v, const int &m, const int &k, std::vector<int> &node_num_redun_to_add);

int update_degree_node_redundancy_coefficient_by_edge_deletion(BipartiteGraph &genB, const int &v, const int &m, const int &k, std::vector<int> &node_num_redun_to_add);

class Rewiring{
	public:
		//parameters
	  	const int R_factor_targeting = 500;
	  	const int R_factor_randomization = 100;

		std::vector<double> jdd_distance_track;
		std::vector<double> dnrc_distance_track;
		std::vector<double> jaccard_index_track;

		Rewiring(){
			jdd_distance_track.clear();
			dnrc_distance_track.clear();
			jaccard_index_track.clear();
		}

		~Rewiring(){
			std::vector<double>().swap(jdd_distance_track);
			std::vector<double>().swap(dnrc_distance_track);
			std::vector<double>().swap(jaccard_index_track);
		}

		BipartiteGraph targeting_node_twoK_preserving_node_oneK_and_edge_oneK(BipartiteGraph B, BipartiteGraph genB);
		BipartiteGraph targeting_node_two_fiveK_preserving_node_twoK_and_edge_oneK(BipartiteGraph B, BipartiteGraph genB);
		BipartiteGraph randomization_preserving_node_twoK_and_node_oneK_and_edge_oneK(BipartiteGraph B);
};

#endif