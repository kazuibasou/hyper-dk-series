#ifndef BASIC_FUNCTION_H
#define BASIC_FUNCTION_H

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

int generate_rand(const int N);

int random_choice(const std::vector<int> &v);

#endif
