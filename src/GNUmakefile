CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3
CXXINCLUDE = -I/Users/kazuki/.pyenv/versions/3.6.3/include/python3.6m/ #include path for Python.h

main: mkdir hyper_dk_series clean

mkdir:
	mkdir -p ../rand_hypergraph
	mkdir -p ../lib

hyper_dk_series: hypergraph.o rewiring.o hyper_dk_series.o
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python -m pybind11 --includes` -undefined dynamic_lookup hypergraph.o rewiring.o hyper_dk_series.o -o ../lib/hyper_dk_series`python3-config --extension-suffix`
hyper_dk_series.o: ./hyper_dk_series.cpp ./hypergraph.h ./rewiring.h
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -c ./hyper_dk_series.cpp -O3
hypergraph.o: ./hypergraph.cpp ./hypergraph.h
	$(CXX) $(CXXFLAGS) -c ./hypergraph.cpp -O3
rewiring.o: ./rewiring.cpp ./hypergraph.h ./rewiring.h
	$(CXX) $(CXXFLAGS) -c ./rewiring.cpp -O3

clean:
	rm *.o

