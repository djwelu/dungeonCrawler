all:dungeon

dungeon: dungeon.cpp
	gcc -Wall -Werror -c  heap.c -o c.o
	g++ -ggdb -std=c++11 -lncurses dungeon.cpp paths.cpp monsters.cpp dice.cpp utils.cpp object_descriptions.cpp c.o -o dungeon

clean:
	rm -f *.o cf *~core dungeon
