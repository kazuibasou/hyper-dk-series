CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

main: hyper_dk_series clean

hyper_dk_series: basic_function.o bipartite_graph.o bipartite_generator.o bipartite_rewiring.o hyper_dk_series.o
	$(CXX) $(CXXFLAGS) -o ../bin/hyper_dk_series basic_function.o bipartite_graph.o bipartite_generator.o bipartite_rewiring.o hyper_dk_series.o -O3
hyper_dk_series.o: ./hyper_dk_series.cpp ./bipartite_graph.h ./basic_function.h ./bipartite_generator.h ./bipartite_rewiring.h
	$(CXX) $(CXXFLAGS) -c ./hyper_dk_series.cpp -O3
basic_function.o:./basic_function.cpp ./basic_function.h
	$(CXX) $(CXXFLAGS) -c ./basic_function.cpp -O3
bipartite_graph.o: ./bipartite_graph.cpp ./bipartite_graph.h ./basic_function.h
	$(CXX) $(CXXFLAGS) -c ./bipartite_graph.cpp -O3
bipartite_generator.o: ./bipartite_generator.cpp ./bipartite_generator.h ./basic_function.h ./bipartite_graph.h ./bipartite_rewiring.h
	$(CXX) $(CXXFLAGS) -c ./bipartite_generator.cpp -O3
bipartite_rewiring.o: ./bipartite_rewiring.cpp ./basic_function.h ./bipartite_graph.h ./bipartite_rewiring.h
	$(CXX) $(CXXFLAGS) -c ./bipartite_rewiring.cpp -O3

clean:
	rm *.o