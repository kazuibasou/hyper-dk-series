#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <float.h>
#include <math.h>
#include <numeric>
#include "basic_function.h"

int generate_rand(const int N){
	if(N == 0){
		printf("Error: Given integer is zero.\n");
		exit(0);
	}
    
	srand((unsigned int)time(NULL));
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randN(0, N-1);

    return randN(mt);
}

int random_choice(const std::vector<int> &v){

	int index = generate_rand(int(v.size()));

	return v[index];
}

int calc_relative_error_scalar(const double emp, const double gen, double &re){

	re = double(std::fabs(emp - gen))/emp;

	return 0;
}

int calc_relative_error_vector(const OneDimProp<double> emp, const OneDimProp<double> gen, double &re){

    std::vector<int> emp_keys = emp.id_to_keys;
    std::vector<int> gen_keys = gen.id_to_keys;

    std::vector<int> union_keys;
    std::sort(emp_keys.begin(), emp_keys.end());
    std::sort(gen_keys.begin(), gen_keys.end());
    std::set_union(std::begin(emp_keys), std::end(emp_keys), std::begin(gen_keys), std::end(gen_keys),std::inserter(union_keys, std::end(union_keys)));

    double norm = 0.0;
    double dist = 0.0;

    for(int key:emp_keys){
        norm += std::fabs(emp.get(key));
    }
    
    for(int key:union_keys){
        dist += std::fabs(emp.get(key) - gen.get(key));
    }
    
    re = double(dist)/norm;

    return 0;
}

int calc_kolmogorov_smirnov_distance(const std::vector<double> emp, const std::vector<double> gen, double &ksdist){

    int size = std::max(int(emp.size()), int(gen.size()));
    ksdist = 0;
    for(int i=0; i<size; ++i){
        double dist;
        if(i < int(emp.size()) && i < int(gen.size())){
            dist = std::fabs(emp[i] - gen[i]);
        }
        else if(i < int(emp.size()) && i >= int(gen.size())){
            dist = std::fabs(emp[i]);
        }
        else if(i >= int(emp.size()) && i < int(gen.size())){
            dist = std::fabs(gen[i]);
        }
        else{
            printf("Error: Size of vectors are wrong.\n");
            exit(0);
        }

        if(ksdist < dist){
            ksdist = dist;
        }
    }

    return 0;
}

int calc_mean(const std::vector<double> &v, double &mean){
    
    mean = double(std::accumulate(v.begin(), v.end(), 0.0))/(v.size());

    return 0;
}

int calc_standard_deviation(const std::vector<double> &v, double &sd){
    
    int n = v.size();
    double sum = 0;
    double sum_square = 0;

    for(int i=0; i<n; ++i){
        sum += v[i];
        sum_square += std::pow(v[i], 2);
    }

    double var = double(sum_square)/n - std::pow(double(sum)/n, 2);
    sd = std::sqrt(var);

    return 0;
}