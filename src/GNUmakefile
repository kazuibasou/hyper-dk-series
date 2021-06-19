CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

main: hyper_dk_series mkdir clean

hyper_dk_series: basic_function.o hypergraph.o rewiring.o hyper_dk_series.o
	$(CXX) $(CXXFLAGS) -o ../bin/hyper_dk_series basic_function.o hypergraph.o rewiring.o hyper_dk_series.o -O3
hyper_dk_series.o: ./hyper_dk_series.cpp ./hypergraph.h ./basic_function.h ./rewiring.h
	$(CXX) $(CXXFLAGS) -c ./hyper_dk_series.cpp -O3
basic_function.o:./basic_function.cpp ./basic_function.h
	$(CXX) $(CXXFLAGS) -c ./basic_function.cpp -O3
hypergraph.o: ./hypergraph.cpp ./hypergraph.h ./basic_function.h
	$(CXX) $(CXXFLAGS) -c ./hypergraph.cpp -O3
rewiring.o: ./rewiring.cpp ./basic_function.h ./hypergraph.h ./rewiring.h
	$(CXX) $(CXXFLAGS) -c ./rewiring.cpp -O3

mkdir:
	mkdir -p ../rand_hypergraph

clean:
	rm *.o