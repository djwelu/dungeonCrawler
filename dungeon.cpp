#include "paths.h"
#include "monsters.h"
#include "dungeon.h"

using namespace std;

char map[21][80];
char rooms[800][4];
int roomNum = 0;
int hardness[21][80];
char seen[21][80];
WINDOW *board;
WINDOW *topLine;
WINDOW *bottomLine;

void addRoom(char ypos, char xpos, char ylength, char xlength){
  rooms[roomNum][0] = ypos;
  rooms[roomNum][1] = xpos;
  rooms[roomNum][2] = ylength;
  rooms[roomNum][3] = xlength;
  int k,l;
  for(k=ypos; k<ylength+ypos; k++){
    for(l=xpos; l<xlength+xpos; l++){
      map[k][l]='.';
      hardness[k][l]=0;
    }
  }
}

int overlap(char ypos, char xpos, char ylength, char xlength){
  int yper, xper;
  for(yper=ypos-1; yper<ylength+ypos+2; yper++){
    for(xper=xpos-1; xper<xlength+xpos+2; xper++){
      if(map[yper][xper]=='.'){
	return 0;
      }
    }
  }
  return 1;
}

void buildborder(){
  int i;
  for(i=0; i<80; i++){
    map[0][i]='_';
    map[20][i]='_';
    hardness[0][i]=255;
    hardness[20][i]=255;
  }
  for(i=0; i<21; i++){
    map[i][0]='|';
    map[i][79]='|';
    hardness[i][0]=255;
    hardness[i][79]=255;
  }
}

void createRoom(){
  char ypos = rand()%13 + 1;
  char xpos = rand()%71 + 1;
  char ylength = rand()%4 + 3;
  char xlength = rand()%5 + 4;
  int n,p;
  p=1;
  for(n=0; n<roomNum; n++){
    if(overlap(ypos,xpos,ylength,xlength)==0){
      p=0;
      break;
    }
  }
  if(p==1){ 
    addRoom(ypos,xpos,ylength,xlength);
    roomNum++;
  }
}

void save(){
  int length,i,j;
  char *path;
  FILE *f;
  char *home = getenv("HOME");
  unsigned int *version,*size,v,s;
  length = strlen(home) + strlen("/.rlg327/dungeon") + 1;
  path = (char *) malloc(length);
  strcpy(path,home);
  strcat(path,"/.rlg327/dungeon");
  f = fopen(path,"w");
  free(path);
  fwrite("RLG327",6,1,f);
  v = htobe32(0);
  version = &v;
  fwrite(version,4,1,f);
  s = htobe32(sizeof(f));
  size = &s;
  fwrite(size,4,1,f);
  for(i=0; i<21; i++){
      for(j=0; j<80; j++){
        fwrite(&hardness[i][j],1,1,f);
      }
  }
  for(i=0; i<5; i++){
      for(j=0; j<4; j++){
        fwrite(&rooms[i][j],1,1,f);
    }
  }
  fclose(f);
}

void load(){
  FILE *f;
  char *path;
  char *home = getenv("HOME");
  char semantic[6];
  unsigned int version,length,size,*v,*s,i,j;
  s = &size;
  v = &version;
  length = strlen(home) + strlen("/.rlg327/dungeon") + 1;
  path = (char *) malloc(length);
  strcpy(path,home);
  strcat(path,"/.rlg327/dungeon");
  f = fopen(path,"r");
  free(path);
  fread(&semantic,6,1,f);
  fread(v,4,1,f);
  version = be32toh(version);
  fread(s,4,1,f);
  size = be32toh(size);
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      fread(&hardness[i][j],1,1,f);
      if(hardness[i][j]==0){
	map[i][j]='#';
      }
      else{
	map[i][j]=' ';
      }
    }
  }
  i = 0;
  j = 0;
  while(!feof(f)){
    fread(&rooms[i][j],1,1,f);
    j++;
    if(j==4){
      j=0;
      i++;
    }
  }
  for(i=0; i<sizeof(rooms)/4 && rooms[i][0]!=0; i++){
    addRoom(rooms[i][0],rooms[i][1],rooms[i][2],rooms[i][3]);
  }
  roomNum = i;
  fclose(f);
}

void createTunnels(){
  int m,startx,starty,endx,endy,x,y;
  for(m=0; m<4; m++){
    startx = (rand()%(rooms[m][2])) + rooms[m][0];
    starty = (rand()%(rooms[m][3])) + rooms[m][1];
    endx = (rand()%(rooms[m+1][2])) + rooms[m+1][0];
    endy = (rand()%(rooms[m+1][3])) + rooms[m+1][1];
    if(startx<endx){
      for(x=startx; x<=endx; x++){
	if(map[x][starty]!='.'){
	  map[x][starty]='#';
	  hardness[x][starty]=0;
	}
      }
    }
    else{
      for(x=endx; x<=startx; x++){
	if(map[x][starty]!='.'){
	  map[x][starty]='#';
	  hardness[x][starty]=0;
	}
      }
    }
    if(starty<endy){
      for(y=starty; y<=endy; y++){
	if(map[endx][y]!='.'){
	  map[endx][y]='#';
	  hardness[endx][y]=0;
	}
      }
    }
    else{
      for(y=endy; y<=starty; y++){
	if(map[endx][y]!='.'){
	  map[endx][y]='#';
	  hardness[endx][y]=0;
	}
      }
    }
  }
}

void createupstairs(){
  int room,x,y;
  room = (rand() % roomNum);
  y = (rand() % rooms[room][2]) + rooms[room][0];
  x = (rand() % rooms[room][3]) + rooms[room][1];
  map[y][x]='<';
}

void createdownstairs(){
  int room,x,y;
  room = (rand() % roomNum);
  y = (rand() % rooms[room][2]) + rooms[room][0];
  x = (rand() % rooms[room][3]) + rooms[room][1];
  map[y][x]='>';
}

void sethardness(){
  int i,j;
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      hardness[i][j] = 1 + (rand() % 253);
    }
  }
}

void display(){
  int i,j,k,index=0,index2=0;
  wmove(board,0,0);
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      if(abs(pc->y-i)<5 && abs(pc->x-j)<5){
	if(monLocations[i][j]!='g'){
	  for(k=0; k<monsters.size(); k++){
	    if(monsters[k]->letter==monLocations[i][j]){
	      index=monsters[k]->color;
	    }
	  }
	  wattron(board,COLOR_PAIR(index));
	  wprintw(board,"%c",monLocations[i][j]);
	  wattroff(board,COLOR_PAIR(index));
	}
	else if(itemLocations[i][j]!='g'){
	  for(k=0; k<object_descriptions.size(); k++){
	    if(getObjectColor(object_descriptions[k].type)==itemLocations[i][j]){
	      index2=(int)object_descriptions[k].color;
	    }
	  }
	  wattron(board,COLOR_PAIR(index2));
	  wprintw(board,"%c",itemLocations[i][j]);
	  wattroff(board,COLOR_PAIR(index2));
	}
	else{
	  wprintw(board,"%c",map[i][j]);
	}
	seen[i][j]=map[i][j];
      }
      else{
	wprintw(board,"%c",seen[i][j]);
      }
    }
  }
}

void displaynofog(){
  int i,j,k,index,index2;
  wmove(board,0,0);
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      index=0;
      if(monLocations[i][j]!='g'){
	for(k=0; k<monsters.size(); k++){
	  if(monsters[k]->letter==monLocations[i][j]){
	    index=monsters[k]->color;
	  }
	}
	wattron(board,COLOR_PAIR(index));
	wprintw(board,"%c",monLocations[i][j]);
	wattroff(board,COLOR_PAIR(index));
      }
      else if(itemLocations[i][j]!='g'){
	for(k=0; k<object_descriptions.size(); k++){
	  if(getObjectColor(object_descriptions[k].type)==itemLocations[i][j]){
	    index2=(int)object_descriptions[k].color;
	  }
	}
	wattron(board,COLOR_PAIR(index2));
	wprintw(board,"%c",itemLocations[i][j]);
	wattroff(board,COLOR_PAIR(index2));
      }
      else{
	wprintw(board,"%c",map[i][j]);
      }
    }
  }
}

void cleareverything(){
  int i,j;
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      monLocations[i][j] = 'g';
      map[i][j]=' ';
      seen[i][j]=' ';
    }
  }
  for(i=0; i<=roomNum; i++){
    for(j=0; j<4; j++){
      rooms[i][j]=0;
    }
  }
  roomNum=0;
}

void createnewdungeon(){
  cleareverything();
  sethardness();
  buildborder();
  while(roomNum<5){
    createRoom();
  }
  createTunnels();
  createrandomspot();
  monLocations[pc->y][pc->x]=pc->letter;
  newmons();
  display();
  wrefresh(board);
}

int main(int argc, char *argv[]){
  //dungeonclass dungeon;
  int i,j;
  pc = new character();
  char *input,*input2,*input3,*input4;
  initscr();
  cbreak();
  noecho();
  notimeout(board,TRUE);
  keypad(stdscr,TRUE);
  scrollok(stdscr,TRUE);
  start_color();
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  refresh();
  board = newwin(21,80,1,0);
  topLine = newwin(1,80,0,0);
  bottomLine = newwin(2,80,22,0);
  //parseMonsters();
  if(argc>1){
    input = argv[1];
  }
  else{
    input = (char *)"";
  }
  if(argc>=3){
    input2 = argv[2];
  }
  else{
    input2 = (char *)"";
  }
  if(argc==5){
    input3 = argv[3];
    input4 = argv[4];
  }
  if(argc == 1 || strcmp(input,"--save")==0 || strcmp(input2,"--save")==0 || strcmp(input,"--nummon")==0){
    srand(time(NULL));
    for(i=0; i<21; i++){
      for(j=0; j<80; j++){
	map[i][j] = ' ';
	seen[i][j] = ' ';
      }
    }
    sethardness();
    buildborder();
    while(roomNum<5){
      createRoom();
    }
    createTunnels();
    createupstairs();
    createdownstairs();
    createrandomspot();
    monLocations[pc->y][pc->x]=pc->letter;
    if(strcmp(input,"--nummon")==0){
     createMonsters(atoi(input2));
     nummon = atoi(input2);
    }
    else{
      createMonsters(10);
      nummon = 10;
    }
    display();
    wrefresh(board);
  }
  if(strcmp(input,"--save")==0 || strcmp(input2,"--save")==0){
    save();
  }
  if(strcmp(input,"--load")==0 || strcmp(input2,"--load")==0){
    load();
    buildborder();
    (*pc).y = -1;
    if(strcmp(input2,"--pc")==0){
      (*pc).y = atoi(input3);
      (*pc).x = atoi(input4);
    }
    if((*pc).y==-1){
      createrandomspot();
    }
    if(strcmp(input2,"--nummon")==0){
      createMonsters(atoi(input3));
    }
    else{
      createMonsters(10);
    }
    display();
  }
  //createnontunnelingmap(hardness,roomNum,rooms);
  //createtunnelingmap(hardness,roomNum,rooms);
  while(nextTurn()==0){
    display();
    wrefresh(board);
    createnontunnelingmap();
    createtunnelingmap();
    }
  endwin();
  return 0;
}
