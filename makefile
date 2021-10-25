CC=gcc
CXX=g++
CFLAGS=-I
DEPS=adder.h immediate.h common.h bitset.h mov.h mov_immediate.h mul.h

%.o:%.cpp $(DEPS)
	$(CXX) -c -o $@ $< 


main: main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mov.o mov_immediate.o mov_override.o mov_override_immediate.o And.o and_immediate.o andOverride.o and_override_immediate.o Cmp.o cmp_immediate.o cmpOverride.o cmp_override_immediate.o Or.o or_immediate.o orOverride.o or_override_immediate.o Xor.o xor_immediate.o xorOverride.o xor_override_immediate.o mul.o mul_addOverride.o inc.o inc_override.o dec.o dec_override.o left_shift.o left_shift_override.o right_shift.o right_shift_override.o left_shift_override_immediate.o right_shift_override_immediate.o divider.o divider_addOverride.o not.o not_addOverride.o Neg.o neg_addOverride.o
	$(CXX) -o main main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mov.o mov_immediate.o mov_override.o mov_override_immediate.o And.o and_immediate.o andOverride.o and_override_immediate.o Cmp.o cmp_immediate.o cmpOverride.o cmp_override_immediate.o Or.o or_immediate.o orOverride.o or_override_immediate.o Xor.o xor_immediate.o xorOverride.o xor_override_immediate.o mul.o mul_addOverride.o inc.o inc_override.o dec.o dec_override.o left_shift.o left_shift_override.o right_shift.o right_shift_override.o left_shift_override_immediate.o right_shift_override_immediate.o divider.o divider_addOverride.o not.o not_addOverride.o Neg.o neg_addOverride.o
	$(MAKE) clean

clean:
	rm -f main.o common.o adder.o adder_addOverride.o immediate.o immediate_addOverride.o bitset.o bitset_addOverride.o mov.o mov_immediate.o mov_override.o mov_override_immediate.o And.o and_immediate.o andOverride.o and_override_immediate.o Cmp.o cmp_immediate.o cmpOverride.o cmp_override_immediate.o Or.o or_immediate.o orOverride.o or_override_immediate.o Xor.o xor_immediate.o xorOverride.o xor_override_immediate.o mul.o mul_addOverride.o inc.o inc_override.o dec.o dec_override.o left_shift.o left_shift_override.o right_shift.o right_shift_override.o left_shift_override_immediate.o right_shift_override_immediate.o divider.o divider_addOverride.o not.o not_addOverride.o Neg.o neg_addOverride.o

#adder.o:adder.cpp adder.h 
#	$(CXX) -c -o $@ $^

#g++ -o main main.cpp adder.cpp adder_addOverride.cpp common.cpp immediate.cpp immediate_addOverride.cpp bitset.cpp bitset_addOverride.cpp mov.cpp mov_immediate.cpp mov_override.cpp mov_override_immediate.cpp inc.cpp inc_override.cpp dec.cpp dec_override.cpp left_shift.cpp left_shift_override.cpp right_shift.cpp right_shift_override.cpp left_shift_override_immediate.cpp right_shift_override_immediate.cpp


	
