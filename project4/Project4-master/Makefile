
### Built with maker.py ###
### source = src ###
### build = build ###
### binary = sim ###

CC=gcc
CXX=g++

CFLAGS= -g -Wall -Wextra
CXXFLAGS= -g -Wall -Wextra -std=c++11

### GENERATED ###

all: sim;

remake:
	/usr/bin/makerpy maker_config.json

clean:
	-rm -r build
	-rm sim

build:
	mkdir -p build/

build/Page.o: src/Page.cpp src/Page.h | build
	$(CXX) $(CXXFLAGS) -c src/Page.cpp -o build/Page.o

build/Simulator.o: src/Simulator.cpp src/Simulator.h | build
	$(CXX) $(CXXFLAGS) -c src/Simulator.cpp -o build/Simulator.o

build/ReplacementAlgorithm.o: src/ReplacementAlgorithm.cpp src/ReplacementAlgorithm.h | build
	$(CXX) $(CXXFLAGS) -c src/ReplacementAlgorithm.cpp -o build/ReplacementAlgorithm.o

build/Process.o: src/Process.cpp src/Process.h | build
	$(CXX) $(CXXFLAGS) -c src/Process.cpp -o build/Process.o

build/Job.o: src/Job.h src/Job.cpp | build
	$(CXX) $(CXXFLAGS) -c src/Job.cpp -o build/Job.o

build/main.o: src/main.cpp | build
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

sim: build/Page.o build/Simulator.o build/ReplacementAlgorithm.o build/Process.o build/Job.o build/main.o
	$(CXX) $(CXXFLAGS) build/Page.o build/Simulator.o build/ReplacementAlgorithm.o build/Process.o build/Job.o build/main.o -o sim
