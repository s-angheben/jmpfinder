CXX=g++
OBJ=jmpfinder

.PHONY: all clean

all: $(OBJ)

loader.o: loader/loader.cc
	$(CXX) -std=c++11 -c loader/loader.cc

jmpfinder: loader.o jmpfinder.cc
	$(CXX) -std=c++11 -o jmpfinder jmpfinder.cc loader.o -lbfd -lcapstone

clean:
	rm -f *.o
