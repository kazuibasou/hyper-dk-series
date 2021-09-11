CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

main: mkdir hyper_dk_series clean

mkdir:
	mkdir -p ../../rand_hypergraph
	mkdir -p ../bin

hyper_dk_series: hypergraph.o rewiring.o hyper_dk_series.o
	$(CXX) $(CXXFLAGS) -o ../bin/hyper_dk_series hypergraph.o rewiring.o hyper_dk_series.o -O3
hyper_dk_series.o: ./hyper_dk_series.cpp ./hypergraph.h ./rewiring.h
	$(CXX) $(CXXFLAGS) -c ./hyper_dk_series.cpp -O3
hypergraph.o: ./hypergraph.cpp ./hypergraph.h
	$(CXX) $(CXXFLAGS) -c ./hypergraph.cpp -O3
rewiring.o: ./rewiring.cpp ./hypergraph.h ./rewiring.h
	$(CXX) $(CXXFLAGS) -c ./rewiring.cpp -O3

clean:
	rm *.o