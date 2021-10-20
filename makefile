CC=gcc
CXX=g++
CFLAGS=-I
DEPS=adder.h immediate.h common.h

%.o:%.cpp $(DEPS)
	$(CXX) -c -o $@ $< 
	
main: main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mul.o
	$(CXX) -o main main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mul.o
	
#adder.o:adder.cpp adder.h 
#	$(CXX) -c -o $@ $^



	
