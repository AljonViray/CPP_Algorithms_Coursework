CXXFLAGS =-ggdb -c -std=c++0x -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant
CFLAGS=-std =c99 -pedantic -Wall -Wextra -Werror -ansi -Wwrite-strings

all: test_stack.cpp test_queue.cpp
	g++ -ggdb -std=c++0x test_stack.cpp -o test_stack
	g++ -ggdb -std=c++0x test_queue.cpp -o test_queue

test_stack: test_stack.cpp
	g++ -ggdb -std=c++0x test_stack.cpp -o test_stack

test_queue: test_queue.cpp
	g++ -ggdb -std=c++0x test_queue.cpp -o test_queue

clean: 
	/bin/rm *core*
