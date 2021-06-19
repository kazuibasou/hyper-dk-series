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