CXX = clang++
CXXFLAGS = -std=c++17 -stdlib=libc++ -Wall -O0 -ggdb3

chris-vm: chris-vm.o
	mkdir -p bin/
	$(CXX) $(CXXFLAGS) bin/chris-vm.o -o bin/chris-vm

chris-vm.o: chris-vm.cpp
	$(CXX) $(CXXFLAGS) -c ./chris-vm.cpp -o ./bin/chris-vm.o

clean:
	rm -f bin/chris-vm.o bin/chris-vm

run:
	./bin/chris-vm