CXX = clang++
CXXFLAGS = -std=c++17 -stdlib=libc++ -Wall -O0 -ggdb3

.PHONY: all clean

bin:
	mkdir -p bin

all: clean chris-vm

chris-vm: chris-vm.o | bin
	mkdir -p bin/
	$(CXX) $(CXXFLAGS) bin/chris-vm.o -o bin/chris-vm

chris-vm.o: chris-vm.cpp | bin
	$(CXX) $(CXXFLAGS) -c ./chris-vm.cpp -o ./bin/chris-vm.o

clean:
	rm -f bin/chris-vm.o bin/chris-vm

run:
	./bin/chris-vm