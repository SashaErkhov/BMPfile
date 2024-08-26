CC=clang++
CFLAGS=-pedantic-errors -gdwarf-4 -std=c++20
LDFLAGS=-lgtest -lgtest_main -lpthread
SOURCES=test1.cpp

all: BMPtests


BMPtests: tests/test1.cpp
	$(CC) tests/test1.cpp $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -rf *.o BMPtests