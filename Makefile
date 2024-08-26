CC=clang++
CFLAGS=-pedantic-errors -gdwarf-4 -std=c++20
LDFLAGS=-lgtest -lgtest_main -lpthread
SOURCES=test1.cpp code/BMPfile.cpp

all: BMPtests

load:
	sudo apt-get install libgtest-dev && cd /usr/src/gtest && sudo cmake CMakeLists.txt && sudo make && sudo cp lib/*.a /usr/lib && sudo ln -s /usr/lib/libgtest.a /usr/local/lib/libgtest.a && sudo ln -s /usr/lib/libgtest_main.a /usr/local/lib/libgtest_main.a

BMPtests: tests/test1.cpp code/BMPfile.cpp
	$(CC) tests/test1.cpp code/BMPfile.cpp $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -rf *.o BMPtests