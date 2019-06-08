CXXFLAGS =-ggdb -c -std=c++11 -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant
CFLAGS=-std =c99 -pedantic -Wall -Wextra -Werror -ansi -Wwrite-strings

dijkstras: dijkstras.cpp
	g++ -ggdb -std=c++11 dijkstras.cpp -o dijkstras

graph: 
	g++ -ggdb -std=c++11 genGraph.cpp -o genGraph
	genGraph > rdm.graph

clean: 
	/bin/rm *core*
