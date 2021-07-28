CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3
CXXINCLUDE = -I/Users/kazuki/.pyenv/versions/3.6.3/include/python3.6m/ #include path for Python.h

main: mkdir hyper_dk_series clean

mkdir:
	mkdir -p ../rand_hypergraph
	mkdir -p ../bin

hyper_dk_series: basic_function.o hypergraph.o rewiring.o hyper_dk_series.o
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python -m pybind11 --includes` -undefined dynamic_lookup basic_function.o hypergraph.o rewiring.o hyper_dk_series.o -o hyper_dk_series`python3-config --extension-suffix`
hyper_dk_series.o: ./hyper_dk_series.cpp ./hypergraph.h ./basic_function.h ./rewiring.h
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -c ./hyper_dk_series.cpp -O3
basic_function.o:./basic_function.cpp ./basic_function.h
	$(CXX) $(CXXFLAGS) -c ./basic_function.cpp -O3
hypergraph.o: ./hypergraph.cpp ./hypergraph.h ./basic_function.h
	$(CXX) $(CXXFLAGS) -c ./hypergraph.cpp -O3
rewiring.o: ./rewiring.cpp ./basic_function.h ./hypergraph.h ./rewiring.h
	$(CXX) $(CXXFLAGS) -c ./rewiring.cpp -O3

clean:
	rm *.o

