CC=gcc
CXX=g++
CFLAGS=-I
DEPS=adder.h immediate.h common.h bitset.h mov.h mov_immediate.h mul.h

%.o:%.cpp $(DEPS)
	$(CXX) -c -o $@ $< 
	
main: main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mov.o mov_immediate.o mov_override.o mov_override_immediate.o And.o and_immediate.o andOverride.o and_override_immediate.o Cmp.o cmp_immediate.o cmpOverride.o cmp_override_immediate.o Or.o or_immediate.o orOverride.o or_override_immediate.o Xor.o xor_immediate.o xorOverride.o xor_override_immediate.o mul.o
	$(CXX) -o main main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mov.o mov_immediate.o mov_override.o mov_override_immediate.o And.o and_immediate.o andOverride.o and_override_immediate.o Cmp.o cmp_immediate.o cmpOverride.o cmp_override_immediate.o Or.o or_immediate.o orOverride.o or_override_immediate.o Xor.o xor_immediate.o xorOverride.o xor_override_immediate.o mul.o

	
#adder.o:adder.cpp adder.h 
#	$(CXX) -c -o $@ $^

#g++ -o main main.cpp adder.cpp adder_addOverride.cpp common.cpp immediate.cpp immediate_addOverride.cpp bitset.cpp bitset_addOverride.cpp mov.cpp mov_immediate.cpp mov_override.cpp mov_override_immediate.cpp


	
