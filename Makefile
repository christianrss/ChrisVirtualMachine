CXX = clang++
CXXFLAGS = -std=c++17 -Wall -ggdb3

chris-vm: chris-vm.o
	mkdir -p bin/
	$(CXX) bin/chris-vm.o -o bin/chris-vm -v

chris-vm.o: chris-vm.cpp
	$(CXX) $(CXXFLAGS) -c ./chris-vm.cpp -o ./bin/chris-vm.o -v

clean:
	rm -f bin/chris-vm.o bin/chris-vm

run:
	./bin/chris-vm