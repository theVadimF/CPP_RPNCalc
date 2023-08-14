CC=gcc
CPP=-std=c++17 -lstdc++
EXTRAS=-lm
COMPILE_KILL=-Wall -Wextra -Werror

FILES=validator.cpp calc_stack.cpp calculator.cpp

GTEST=-lm -lgtest
PKG_GTEST=`pkg-config --cflags --libs gtest`

all: clean compile_object_files test

test: clean compile_object_files
	if [ -d "~/.brew" ]; then \
		$(CC) test.cc *.o $(CPP) $(GTEST) $(EXTRAS) -o Test $(COMPILE_KILL); \
	else \
		$(CC) test.cc *.o $(CPP) $(PKG_GTEST) $(EXTRAS) -o Test $(COMPILE_KILL); \
	fi
	./Test

compile_object_files:
	$(CC) -c $(FILES) -std=c++17

debug: clean compile_object_files
	if [ -d "~/.brew" ]; then \
		$(CC) -g test.cc *.o $(CPP) $(GTEST) $(EXTRAS); \
	else \
		$(CC) -g test.cc *.o $(CPP) $(PKG_GTEST) $(EXTRAS); \
	fi

leak_test: clean compile_object_files
	$(CC) -g test.cc *.o $(CPP) $(PKG_GTEST) $(EXTRAS) -o Test
	valgrind --leak-check=full ./Test

clean:
	rm -f Test *.o SmartCalc_2.0.tar.gz *.out *.exe *.a *.gcda *.gcno *.info *.gcov \
	Documentation.aux Documentation.dvi Documentation.log missfont.log
	rm -rf ./Build ./report
