#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

class HyperGraph{
	public:
		std::vector<int> V;
		std::vector<int> E;
		std::unordered_map<int, std::vector<int>> elist;
		std::unordered_map<int, std::vector<int>> vlist;

		HyperGraph();
		~HyperGraph();

		int read_hypergraph(const char *hypergraph);
};

//two-dimensional property (e.g., pairwise joint degree distribution)
template <typename TYPE> class Matrix{
	public:
		std::unordered_map<int, int> key_to_ids;
		std::vector<int> id_to_keys;
		std::vector<std::vector<TYPE>> entries;

		Matrix();
		~Matrix();

		int clear();
		int entry(const int k);
		TYPE get(const int k, const int l) const;
		int set(const int k, const int l, const TYPE value);
		int add(const int k, const int l, const TYPE value);
		int subtract(const int k, const int l, const TYPE value);
};

template <typename TYPE>
Matrix<TYPE>::Matrix(){
	key_to_ids = std::unordered_map<int, int>();
	id_to_keys = std::vector<int>();
    entries = std::vector<std::vector<TYPE>>();
}

template <typename TYPE>
Matrix<TYPE>::~Matrix(){
	std::unordered_map<int, int>().swap(key_to_ids);
	std::vector<int>().swap(id_to_keys);
    std::vector<std::vector<TYPE>>().swap(entries);
}

template <typename TYPE>
int Matrix<TYPE>::clear(){
	key_to_ids.clear();
	id_to_keys.clear();
	entries.clear();

	return 0;
}

template <typename TYPE>
int Matrix<TYPE>::entry(const int k){
	int size = int(id_to_keys.size());

	id_to_keys.push_back(k);
	key_to_ids[k] = size;

	for(int i=0; i<size; ++i){
		entries[i].push_back(TYPE(0));
	}

	entries.push_back(std::vector<TYPE>(size+1, TYPE(0)));

	return 0;
}

template <typename TYPE>
TYPE Matrix<TYPE>::get(const int k, const int l) const{
	if(key_to_ids.find(k) == key_to_ids.end()){
		return 0;
	}
	if(key_to_ids.find(l) == key_to_ids.end()){
		return 0;
	}

	int i_k = key_to_ids.at(k);
	int i_l = key_to_ids.at(l);

	return entries[i_k][i_l];
}

template <typename TYPE>
int Matrix<TYPE>::set(const int k, const int l, const TYPE value){
	if(key_to_ids.find(k) == key_to_ids.end()){
		entry(k);
	}

	if(key_to_ids.find(l) == key_to_ids.end()){
		entry(l);
	}

	int i_k = key_to_ids[k];
	int i_l = key_to_ids[l];

	entries[i_k][i_l] = value;

    return 0;
}

template <typename TYPE>
int Matrix<TYPE>::add(const int k, const int l, const TYPE value){
    if(key_to_ids.find(k) == key_to_ids.end()){
		entry(k);
	}

	if(key_to_ids.find(l) == key_to_ids.end()){
		entry(l);
	}

	int i_k = key_to_ids[k];
	int i_l = key_to_ids[l];

	entries[i_k][i_l] += value;

    return 0;
}

template <typename TYPE>
int Matrix<TYPE>::subtract(const int k, const int l, const TYPE value){
	if(key_to_ids.find(k) == key_to_ids.end()){
		entry(k);
	}

	if(key_to_ids.find(l) == key_to_ids.end()){
		entry(l);
	}

	int i_k = key_to_ids[k];
	int i_l = key_to_ids[l];

	entries[i_k][i_l] -= value;

    return 0;
}

int add_node_to_hyperedge(HyperGraph &G, const int &v, const int &m);
int remove_node_from_hyperedge(HyperGraph &G, const int &v, const int &m);
int calc_node_degree(HyperGraph &G, std::vector<int> &node_degree);
int calc_maximum_node_degree(HyperGraph &G, int &max_node_deg);
int calc_num_jnt_node_deg(HyperGraph &G, Matrix<int> &num_jnt_node_deg);
int calc_node_redundancy_coefficient(HyperGraph &G, std::vector<double> &node_redun_coeff);
int calc_degree_dependent_node_redundancy_coefficient(HyperGraph &G, std::vector<double> &degree_node_redun_coeff);
int calc_hyperedge_size(HyperGraph &G, std::vector<int> &hyperedge_size);

#endif