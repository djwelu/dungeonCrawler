#ifndef MONSTERS_H
#define MONSTERS_H 

#include <string>
#include <vector>
#include "object_descriptions.h"
#include "dice.h"

class character{
 public:
  int ispc;
  int y;
  int x;
  unsigned int characteristics;
  int ded;
  char letter;
  int turn;
  int color;
  const dice *speed;
  object_description *equiped[22]; 
  //std::string name;
  //std::string description;
  //const dice *hp;
  //const dice *damage;
  //std::vector<std::string> abilities;
  character(){
    ispc = 0;
    y = 0;
    x = 0;
    characteristics = 0;
    ded = 0;
    turn = 0;
    letter = '@';
    color = 0;
    speed = new dice(10,0,0);
  }
  ~character(){
  }
};

class npc: public character{
 public:
  std::string name;
  std::string description;
  const dice *hp;
  const dice *damage;
  std::vector<std::string> abilities;
  npc(){
    name = "";
    description = "";
    hp = new dice(0,0,0);
    damage = new dice(0,0,0);
  }
  ~npc(){
  }
};

class object{
 public:
  std::string name;
  std::string description;
  object_type_t type;
  uint32_t color;
  dice hit;
  dice damage;
  dice dodge;
  dice defence;
  dice weight;
  dice speed;
  dice attrubute;
  dice value;
};

extern character *pc;
extern std::vector<npc*> monsters;
extern char monLocations[21][80];
extern int nummon;
extern std::vector<object_description> object_descriptions;
extern char itemLocations[21][80];

void createMonsters(int num);
int nextTurn();
void newmons();
int parseMonsters();
char getObjectColor(int i);

#endif
