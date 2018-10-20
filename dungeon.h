#ifndef DUNGEON_H
#define DUNGEON_H

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <endian.h>
#include <cstdint>
#include <unistd.h>
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/*class dungeonclass{
 public:
  char map[21][80];
  char rooms[800][4];
  int roomNum;
  int hardness[21][80];
};*/

//extern dungeonclass dungeon;
extern WINDOW *topLine;
extern WINDOW *board;
extern WINDOW *bottomLine;
extern char map[21][80];
extern char rooms[800][4];
extern int roomNum;
extern int hardness[21][80];

void createnewdungeon();
void createupstairs();
void createdownstairs();
void display();
void displaynofog();

#endif
