CXX=clang++
FLAGS=-Wall -std=c++11 -O2

.PHONY: all
all: test.o
	$(CXX) -o test test.o

%.o: %.cpp
	$(CXX) $(FLAGS) $< -c -o $@

clean:
	rm *.o
