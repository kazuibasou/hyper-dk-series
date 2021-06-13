#ifndef BASIC_FUNCTION_H
#define BASIC_FUNCTION_H

//one-dimensional property (e.g., degree distribution)
template <typename TYPE> class OneDimProp{
	public:
		std::unordered_map<int, int> key_to_ids;
		std::vector<int> id_to_keys;
		std::vector<TYPE> entries;

		OneDimProp();
		~OneDimProp();

		int clear();
		int entry(const int k);
		TYPE get(const int k) const;
		int set(const int k, const TYPE value);
		int add(const int k, const TYPE value);
		int subtract(const int k, const TYPE value);
		int get_keys(std::vector<int> &keys) const;
};

template <typename TYPE>
OneDimProp<TYPE>::OneDimProp(){
	key_to_ids = std::unordered_map<int, int>();
	id_to_keys = std::vector<int>();
    entries = std::vector<TYPE>();
}

template <typename TYPE>
OneDimProp<TYPE>::~OneDimProp(){
	std::unordered_map<int, int>().swap(key_to_ids);
	std::vector<int>().swap(id_to_keys);
    std::vector<TYPE>().swap(entries);
}

template <typename TYPE>
int OneDimProp<TYPE>::clear(){
	id_to_keys.clear();
	key_to_ids.clear();
	entries.clear();

	return 0;
}

template <typename TYPE>
int OneDimProp<TYPE>::entry(const int k){
	id_to_keys.push_back(k);
	key_to_ids[k] = int(id_to_keys.size())-1;
	entries.push_back(TYPE(0));

	return 0;
}

template <typename TYPE>
TYPE OneDimProp<TYPE>::get(const int k) const{
	if(key_to_ids.find(k) == key_to_ids.end()){
		return 0;
	}

	int i_k = key_to_ids.at(k);

	return entries[i_k];
}

template <typename TYPE>
int OneDimProp<TYPE>::set(const int k, const TYPE value){
	if(key_to_ids.find(k) == key_to_ids.end()){
		entry(k);
	}

	int i_k = key_to_ids[k];

	entries[i_k] = value;

    return 0;
}

template <typename TYPE>
int OneDimProp<TYPE>::add(const int k, const TYPE value){
    if(key_to_ids.find(k) == key_to_ids.end()){
		entry(k);
	}

	int i_k = key_to_ids[k];

	entries[i_k] += value;

    return 0;
}

template <typename TYPE>
int OneDimProp<TYPE>::subtract(const int k, const TYPE value){
	if(key_to_ids.find(k) == key_to_ids.end()){
		entry(k);
	}

	int i_k = key_to_ids[k];

	entries[i_k] -= value;

    return 0;
}

template <typename TYPE>
int OneDimProp<TYPE>::get_keys(std::vector<int> &keys) const{
	keys.clear();
	keys = std::vector<int>(id_to_keys);

	return 0;
}

//two-dimensional property (e.g., joint degree distribution)
template <typename TYPE> class TwoDimProp{
	public:
		std::unordered_map<int, int> key_to_ids;
		std::vector<int> id_to_keys;
		std::vector<std::vector<TYPE>> entries;

		TwoDimProp();
		~TwoDimProp();

		int clear();
		int entry(const int k);
		TYPE get(const int k, const int l) const;
		int set(const int k, const int l, const TYPE value);
		int add(const int k, const int l, const TYPE value);
		int subtract(const int k, const int l, const TYPE value);
		TYPE sum_line(const int k) const;
		int get_keys(std::vector<int> &keys) const;
};

template <typename TYPE>
TwoDimProp<TYPE>::TwoDimProp(){
	key_to_ids = std::unordered_map<int, int>();
	id_to_keys = std::vector<int>();
    entries = std::vector<std::vector<TYPE>>();
}

template <typename TYPE>
TwoDimProp<TYPE>::~TwoDimProp(){
	std::unordered_map<int, int>().swap(key_to_ids);
	std::vector<int>().swap(id_to_keys);
    std::vector<std::vector<TYPE>>().swap(entries);
}

template <typename TYPE>
int TwoDimProp<TYPE>::clear(){
	key_to_ids.clear();
	id_to_keys.clear();
	entries.clear();

	return 0;
}

template <typename TYPE>
int TwoDimProp<TYPE>::entry(const int k){
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
TYPE TwoDimProp<TYPE>::get(const int k, const int l) const{
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
int TwoDimProp<TYPE>::set(const int k, const int l, const TYPE value){
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
int TwoDimProp<TYPE>::add(const int k, const int l, const TYPE value){
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
int TwoDimProp<TYPE>::subtract(const int k, const int l, const TYPE value){
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

template <typename TYPE>
TYPE TwoDimProp<TYPE>::sum_line(const int k) const{
	if(key_to_ids.find(k) == key_to_ids.end()){
		return 0;
	}
	
	int i_k = key_to_ids.at(k);

	TYPE sum = std::accumulate(entries[i_k].begin(), entries[i_k].end(), TYPE(0)); 

	return sum;
}

template <typename TYPE>
int TwoDimProp<TYPE>::get_keys(std::vector<int> &keys) const{
	keys.clear();
	keys = std::vector<int>(id_to_keys);

	return 0;
}

int generate_rand(const int N);

int random_choice(const std::vector<int> &v);

int calc_relative_error_scalar(const double emp, const double gen, double &re);

int calc_relative_error_vector(const OneDimProp<double> emp, const OneDimProp<double> gen, double &re);

int calc_kolmogorov_smirnov_distance(const std::vector<double> emp, const std::vector<double> gen, double &ksdist);

int calc_mean(const std::vector<double> &v, double &mean);

int calc_standard_deviation(const std::vector<double> &v, double &sd);

#endif
