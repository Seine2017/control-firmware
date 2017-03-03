all: test_sat

test_sat: test_sat.c ../../src/sat.h
	gcc -g -O2 -Wall -I../../src -o $@ $<

clean:
	rm -f test_sat
