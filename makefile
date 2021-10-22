CC=gcc
CXX=g++
CFLAGS=-I
DEPS=adder.h immediate.h common.h

%.o:%.cpp $(DEPS)
	$(CXX) -c -o $@ $< 
	
main: main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o
	$(CXX) -o main main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mov.o mov_immediate.o mov_override.o mov_override_immediate.o inc.o inc_override.o dec.o dec_override.o
	
#adder.o:adder.cpp adder.h 
#	$(CXX) -c -o $@ $^

#g++ -o main main.cpp adder.cpp adder_addOverride.cpp common.cpp immediate.cpp immediate_addOverride.cpp bitset.cpp bitset_addOverride.cpp mov.cpp mov_immediate.cpp mov_override.cpp mov_override_immediate.cpp inc.cpp inc_override.cpp dec.cpp dec_override.cpp


	
