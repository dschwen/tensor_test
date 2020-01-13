all:	test

test: test.cc
	$(CXX) --std=c++11 test.cc -o test

clean:
	rm test
