#include "heap.h"
#include "monsters.h"
#include "paths.h"
#include "dungeon.h"
#include "object_descriptions.h"
#include "dice.h"
#include <string>

using namespace std;

character *pc;
//npc *mons;
vector<npc*> mons;
vector<npc*> monsters;
vector<bool> uniques;
vector<object_description> object_descriptions;
char monLocations[21][80];
char itemLocations[21][80];
heap_t h;
int nummon;

int parseMonsters(){
  int length,i=0,j;
  char *path;
  char *home = getenv("HOME");
  length = strlen(home) + strlen("/.rlg327/monster_desc.txt") + 1;
  path = (char *) malloc(length);
  strcpy(path,home);
  strcat(path,"/.rlg327/monster_desc.txt");
  ifstream f(path);
  free(path);
  string cur = "";
  //vector<character*> monsters;
  int tempchar2,countMon;
  string temp1,temp2,temp3,colorstr;
  char tempchar;
  while(cur.length()<2){
    getline(f,cur);
  }
  if(cur!="RLG327 MONSTER DESCRIPTION 1"){
    f.close();
    return 1;
  }
  cur = "";
  while(getline(f,cur)){
    while(cur!="BEGIN MONSTER"){
      getline(f,cur);
      if(f.eof()) break;
    }
    if(f.eof()) break;
    monsters.push_back(new npc());
    uniques.push_back(false);
    while(cur!="END"){
      f >> cur;
      if(cur=="NAME"){
	f.get();
        getline(f,(*(monsters[i])).name);
      }
      if(cur=="DESC"){
	f.get();
	cur = "";
        while(cur!="."){
	  (*(monsters[i])).description+=cur;
	  cur+=" ";
	  getline(f,cur);
	}
      }
      if(cur=="HP"){
        f.get();
        temp1 = temp2 = temp3 = "";
	while(true){
	  tempchar = f.get();
	  if(tempchar=='+') break;
	  temp1 += tempchar;
	}
	while(true){
	  tempchar = f.get();
	  if(tempchar=='d') break;
	  temp2 += tempchar;
	}
	while(true){
	  tempchar = f.peek();
	  tempchar2 = tempchar-'0';
	  if(tempchar2>9 || tempchar2<0) break;
	  tempchar = f.get();
	  temp3 += tempchar;
	}
	(*(monsters[i])).hp=new dice(atoi(temp1.c_str()),atoi(temp2.c_str()),atoi(temp3.c_str()));
      }
      if(cur=="SYMB"){
        f.get();
	(*(monsters[i])).letter = f.get();
      }
      if(cur=="COLOR"){
	f.get();
	getline(f,colorstr);
        if(colorstr=="BLACK"){
	  (*(monsters[i])).color = 0;
	}
	else if(colorstr=="RED"){
	  (*(monsters[i])).color = 1;
	}
	else if(colorstr=="GREEN"){
	  (*(monsters[i])).color = 2;
	}
	else if(colorstr=="YELLOW"){
	  (*(monsters[i])).color = 3;
	}
	else if(colorstr=="BLUE"){
	  (*(monsters[i])).color = 4;
	}
	else if(colorstr=="MAGENTA"){
	  (*(monsters[i])).color = 5;
	}
	else if(colorstr=="CYAN"){
	  (*(monsters[i])).color = 6;
	}
	else if(colorstr=="WHITE"){
	  (*(monsters[i])).color = 0;
	}
      }
      if(cur=="ABIL"){
	f.get();
	getline(f,cur);
        if(cur.find("TELE") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("TELE");
	    (*(monsters[i])).characteristics+=16;
	}
	if(cur.find("SMART") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("SMART");
	    (*(monsters[i])).characteristics+=8;
	}
	if(cur.find("PASS") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("PASS");
	    (*(monsters[i])).characteristics+=128;
	}
	if(cur.find("UNIQ") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("UNIQ");
	    (*(monsters[i])).characteristics+=256;
	}
	if(cur.find("TUNNEL") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("TUNNEL");
	    (*(monsters[i])).characteristics+=4;
	}
	if(cur.find("ERRATIC") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("ERRATIC");
	    (*(monsters[i])).characteristics+=2;
	}
	if(cur.find("DESTROY") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("DESTROY");
	    (*(monsters[i])).characteristics+=32;
	}
	if(cur.find("PICKUP") != std::string::npos){
	    (*(monsters[i])).abilities.push_back("PICKUP");
	    (*(monsters[i])).characteristics+=64;
	}
      }
      if(cur=="SPEED"){
	f.get();
	temp1 = temp2 = temp3 = "";
	while(true){
	  tempchar = f.get();
	  if(tempchar=='+') break;
	  temp1 += tempchar;
	}
	while(true){
	  tempchar = f.get();
	  if(tempchar=='d') break;
	  temp2 += tempchar;
	}
	while(true){
	  tempchar = f.peek();
	  tempchar2 = tempchar-'0';
	  if(tempchar2>9 || tempchar2<0) break;
	  tempchar = f.get();
	  temp3 += tempchar;
	}
	(*(monsters[i])).speed=new dice(atoi(temp1.c_str()),atoi(temp2.c_str()),atoi(temp3.c_str()));
      }
      if(cur=="DAM"){
        f.get();
        temp1 = temp2 = temp3 = "";
	while(true){
	  tempchar = f.get();
	  if(tempchar=='+') break;
	  temp1 += tempchar;
	}
	while(true){
	  tempchar = f.get();
	  if(tempchar=='d') break;
	  temp2 += tempchar;
	}
	while(true){
	  tempchar = f.peek();
	  tempchar2 = tempchar-'0';
	  if(tempchar2>9 || tempchar2<0) break;
	  tempchar = f.get();
	  temp3 += tempchar;
	}
	(*(monsters[i])).damage=new dice(atoi(temp1.c_str()),atoi(temp2.c_str()),atoi(temp3.c_str()));
      }
    }
    i++;
  }
  /*for(i=0; i<monsters.size(); i++){
    wattron(board,COLOR_PAIR(6));
    wprintw(board,"%s\n",(*(monsters[i])).name.c_str());
    wprintw(board,"%d\n",monsters.size());
    wrefresh(board);
    printf("%c\n",(*(monsters[i])).letter);
    printf("%d\n",(*(monsters[i])).color);
    printf("%s\n",(*(monsters[i])).description.c_str());
    printf("base:%d dice:%d sides:%d\n",(*(monsters[i])).speed[0],(*(monsters[i])).speed[1],(*(monsters[i])).speed[2]);
    printf("base:%d dice:%d sides:%d\n",(*(monsters[i])).damage[0],(*(monsters[i])).damage[1],(*(monsters[i])).damage[2]);
    printf("base:%d dice:%d sides:%d\n",(*(monsters[i])).hp[0],(*(monsters[i])).hp[1],(*(monsters[i])).hp[2]);
    for(j=0; j<(*(monsters[i])).abilities.size(); j++){
      printf("%s ",(*(monsters[i])).abilities[j].c_str());
    }
    printf("\n\n");
    }*/
  //getch();
  f.close();
  return 0;
}

npc* getMon(int y, int x){
  int i;
  for(i=0; i<monsters.size(); i++){
    if(monsters[i]->y == y && monsters[i]->x ==x){
      return monsters[i];
    }
  }
  return monsters[0];
}

void createObjects(){
  int i,j,k=0,room,randtype,x,y,nooverlap=0;
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      itemLocations[i][j]='g';
    }
  }
  while(k<=12){
    nooverlap = 0;
    while(nooverlap==0){
      room;
      room = (rand() % roomNum);
      y = (rand() % rooms[room][2]) + rooms[room][0];
      x = (rand() % rooms[room][3]) + rooms[room][1];
      if(itemLocations[y][x]=='g'){ 
	nooverlap=1;
      }
    }
    randtype=rand()%object_descriptions.size();
    switch(object_descriptions[randtype].type){
      case 1:
      itemLocations[y][x] = '|';
      break;
    case 2:
      itemLocations[y][x] = ')';
      break;
    case 3:
      itemLocations[y][x] = '}';
      break;
    case 4:
      itemLocations[y][x] = '[';
      break;
    case 5:
      itemLocations[y][x] = ']';
      break;
    case 6:
      itemLocations[y][x] = '(';
      break;
    case 7:
      itemLocations[y][x] = '{';
      break;
    case 8:
      itemLocations[y][x] = '\\';
      break;
    case 9:
      itemLocations[y][x] = '=';
      break;
    case 10:
      itemLocations[y][x] = '"';
      break;
    case 11:
      itemLocations[y][x] = '_';
      break;
    case 12:
      itemLocations[y][x] = '~';
      break;
    case 13:
      itemLocations[y][x] = '?';
      break;
    case 14:
      itemLocations[y][x] = '!';
      break;
    case 15:
      itemLocations[y][x] = '$';
      break;
    case 16:
      itemLocations[y][x] = '/';
      break;
    case 17:
      itemLocations[y][x] = ',';
      break;
    case 18:
      itemLocations[y][x] = '-';
      break;
    case 19:
      itemLocations[y][x] = '%';
      break;
    case 20:
      itemLocations[y][x] = '&';
      break;
    default:
      itemLocations[y][x] = '+';
    }
    k++;
  }
}

char getObjectColor(int i){
  int k=0;
  switch(object_descriptions[i].type){
  case 1:
    return '|';
  case 2:
    return ')';
  case 3:
    return '}';
  case 4:
    return '[';
  case 5:
    return ']';
  case 6:
    return '(';
  case 7:
    return '{';
  case 8:
    return '\\';
  case 9:
    return '=';
  case 10:
    return '"';
  case 11:
    return '_';
  case 12:
    return '~';
  case 13:
    return '?';
  case 14:
    return '!';
  case 15:
    return '$';
  case 16:
    return '/';
  case 17:
    return ',';
  case 18:
    return '-';
  case 19:
    return '%';
  case 20:
    return '&';
  default:
    return '+';
  }
  k++;
}

void displaymonsterlist(){
  WINDOW *monsterlist = derwin(board,15,30,1,25);
  int i,j,nsdist,wedist,ch,pages,curpage;
  curpage=0;
  for(i=0; i<15; i++){
    if(mons[i]->ded==0){
      wprintw(monsterlist,"  %c, ",mons[i]->letter);
      nsdist = mons[i]->y - pc->y;
      if(nsdist < 0){
	wprintw(monsterlist,"north %d, ", abs(nsdist));
      }
      if(nsdist > 0){
	wprintw(monsterlist,"south %d, ", abs(nsdist));
      }
      wedist = mons[i]->x - pc->x;
      if(wedist < 0){
	wprintw(monsterlist,"west %d \n", abs(wedist));
      }
      else if(wedist > 0){
	wprintw(monsterlist,"east %d \n", abs(wedist));
      }
      else{
	wprintw(monsterlist,"\n");
      }
    }
  }
  pages=(nummon/15)+1;
  wrefresh(monsterlist);
  wclear(monsterlist);
  j=0;
  while(j==0){
    ch = getch();
    if(ch==27){
      delwin(monsterlist);
      wrefresh(monsterlist);
      display();
      wrefresh(board);
      monLocations[pc->y][pc->x]='g';
      j=1;
    }
    if(ch==KEY_UP){
      if(curpage>0){
	curpage--;
	wmove(monsterlist,0,0);
	for(i=(curpage*15); i<(curpage*15)+15; i++){
	  if(mons[i]->ded==0){
	    wprintw(monsterlist,"  %c, ",mons[i]->letter);
	    nsdist = mons[i]->y - pc->y;
	    if(nsdist < 0){
	      wprintw(monsterlist,"north %d, ", abs(nsdist));
	    }
	    if(nsdist > 0){
	      wprintw(monsterlist,"south %d, ", abs(nsdist));
	    }
	    wedist = mons[i]->x - pc->x;
	    if(wedist < 0){
	      wprintw(monsterlist,"west %d \n", abs(wedist));
	    }
	    else if(wedist > 0){
	      wprintw(monsterlist,"east %d \n", abs(wedist));
	    }
	    else{
	      wprintw(monsterlist,"\n");
	    }
	  }
	}
	wrefresh(monsterlist);
	wclear(monsterlist);
      }
    }
    if(ch==KEY_DOWN){
      if(curpage<pages-1){
	curpage++;
	wmove(monsterlist,0,0);
	for(i=curpage*15; i<(curpage*15)+15; i++){
	  if(mons[i]->ded==0){
	    wprintw(monsterlist,"  %c, ",mons[i]->letter);
	    nsdist = mons[i]->y - pc->y;
	    if(nsdist < 0){
	      wprintw(monsterlist,"north %d, ", abs(nsdist));
	    }
	    if(nsdist > 0){
	      wprintw(monsterlist,"south %d, ", abs(nsdist));
	    }
	    wedist = mons[i]->x - pc->x;
	    if(wedist < 0){
	      wprintw(monsterlist,"west %d \n", abs(wedist));
	    }
	    else if(wedist > 0){
	      wprintw(monsterlist,"east %d \n", abs(wedist));
	    }
	    else{
	      wprintw(monsterlist,"\n");
	    }
	  }
	}
	wrefresh(monsterlist);
	wclear(monsterlist);
      }
    }
  }
}

void getinventorylist(){
  WINDOW *inventory = derwin(board,15,30,1,25);
  int k;
  for(k=12; k<22; k++){
    if(pc->equiped[k]!=NULL){
      wprintw(inventory,"%s",pc->equiped[k]->name.c_str());
    }
  }
  wrefresh(inventory);
  getch();
  delwin(inventory);
  display();
  wrefresh(board);
}

void getequipedlist(){
  WINDOW *inventory = derwin(board,15,30,1,25);
  int k;
  for(k=0; k<12; k++){
    if(pc->equiped[k]!=NULL){
      wprintw(inventory,"%s",pc->equiped[k]->name.c_str());
    }
  }
  wrefresh(inventory);
  getch();
  delwin(inventory);
  display();
  wrefresh(board);
}

void promptcarryslot(int i){
  WINDOW *inventory = derwin(board,15,35,1,25);
  int k;
  for(k=12; k<22; k++){
    if(pc->equiped[k]!=NULL){
      if(k==i) wprintw(inventory,"-> ");
      wprintw(inventory,"%s\n",pc->equiped[k]->name.c_str());
    }
    else{
      if(k==i) wprintw(inventory,"-> ");
      wprintw(inventory,"\n");
    }
  }
  wrefresh(inventory);
  delwin(inventory);
}

void promptequipedslot(int i){
  WINDOW *inventory = derwin(board,15,35,1,25);
  int k;
  for(k=0; k<12; k++){
    if(pc->equiped[k]!=NULL){
      if(k==i) wprintw(inventory,"-> ");
      wprintw(inventory,"%s\n",pc->equiped[k]->name.c_str());
    }
    else{
      if(k==i) wprintw(inventory,"-> ");
      wprintw(inventory,"\n");
    }
  }
  wrefresh(inventory);
  delwin(inventory);
}


void wearitem(){
  int j=0;
  int cur=12;
  int ch;
  wmove(topLine,0,0);
  wprintw(topLine,"use up and down arrows to scroll, enter to wear");
  wrefresh(topLine);
  promptcarryslot(cur);
  while(j==0){
    ch = getch();
    if(ch==27){
      display();
      wrefresh(board);
      wclear(topLine);
      wrefresh(topLine);
      j=1;
    }
    if(ch==KEY_UP){
      if(cur>12){
	cur--;
	promptcarryslot(cur);
      }
    }
    if(ch==KEY_DOWN){
      if(cur<21){
	cur++;
	promptcarryslot(cur);
      }
    }
    if(ch==10){
      object_type_t temp;
      object_description *temp2;
      if(pc->equiped[cur]!=NULL){
	temp = pc->equiped[cur]->type;
      }
      if(temp==1){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[0]=temp2;
      } 
      if(temp==2){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[1]=temp2;
      } 
      if(temp==3){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[2]=temp2;
      } 
      if(temp==5){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[3]=temp2;
      } 
      if(temp==6){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[4]=temp2;
      } 
      if(temp==7){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[5]=temp2;
      } 
      if(temp==8){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[6]=temp2;
      } 
      if(temp==9){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[7]=temp2;
      } 
      if(temp==10){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[8]=temp2;
      } 
      if(temp==4){
	temp2 = pc->equiped[cur];
	pc->equiped[cur]=NULL;
	pc->equiped[9]=temp2;
      } 
      if(temp==12){
	if(pc->equiped[10]==NULL || (pc->equiped[10]!=NULL && pc->equiped[11]!=NULL)){
	  temp2 = pc->equiped[cur];
	  pc->equiped[cur]=NULL;
	  pc->equiped[10]=temp2;
	} 
	if(pc->equiped[11]==NULL){
	  temp2 = pc->equiped[cur];
	  pc->equiped[cur]=NULL;
	  pc->equiped[11]=temp2;
	} 
      }
      promptcarryslot(cur);
    }
  }
}

void takeoff(){
  int j=0,k;
  int cur=0;
  int ch;
  wmove(topLine,0,0);
  wprintw(topLine,"use up and down arrows to scroll, enter to takeoff");
  wrefresh(topLine);
  promptequipedslot(cur);
  while(j==0){
    ch = getch();
    if(ch==27){
      display();
      wrefresh(board);
      wclear(topLine);
      wrefresh(topLine);
      j=1;
    }
    if(ch==KEY_UP){
      if(cur>0){
	cur--;
	promptequipedslot(cur);
      }
    }
    if(ch==KEY_DOWN){
      if(cur<12){
	cur++;
	promptequipedslot(cur);
      }
    }
    if(ch==KEY_ENTER){
      for(k=12; k<22; k++){
	if(pc->equiped[k]==NULL){
	  pc->equiped[k]=pc->equiped[cur];
	  pc->equiped[cur]=NULL;
	  break;
	}
      }
      promptequipedslot(cur);
    }
  }
}

void drop(){
  int j=0;
  int cur=12;
  int ch;
  wmove(topLine,0,0);
  wprintw(topLine,"use up and down arrows to scroll, enter to drop");
  wrefresh(topLine);
  promptcarryslot(cur);
  while(j==0){
    ch = getch();
    if(ch==27){
      display();
      wrefresh(board);
      wclear(topLine);
      wrefresh(topLine);
      j=1;
    }
    if(ch==KEY_UP){
      if(cur>12){
	cur--;
	promptcarryslot(cur);
      }
    }
    if(ch==KEY_DOWN){
      if(cur<21){
	cur++;
	promptcarryslot(cur);
      }
    }
    if(ch==10){
      itemLocations[pc->y][pc->x]=getObjectColor(pc->equiped[cur]->type);
      pc->equiped[cur]=NULL;
      promptcarryslot(cur);
    }
  }
}

void expunge(){
  int j=0;
  int cur=12;
  int ch;
  wmove(topLine,0,0);
  wprintw(topLine,"use up and down arrows to scroll, enter to expunge");
  wrefresh(topLine);
  promptcarryslot(cur);
  while(j==0){
    ch = getch();
    if(ch==27){
      display();
      wrefresh(board);
      wclear(topLine);
      wrefresh(topLine);
      j=1;
    }
    if(ch==KEY_UP){
      if(cur>12){
	cur--;
	promptcarryslot(cur);
      }
    }
    if(ch==KEY_DOWN){
      if(cur<21){
	cur++;
	promptcarryslot(cur);
      }
    }
    if(ch==10){
      pc->equiped[cur]=NULL;
      promptcarryslot(cur);
    }
  }
}

void inspectitem(){
  int j=0;
  int cur=12;
  int ch;
  wmove(topLine,0,0);
  wprintw(topLine,"use up and down arrows to scroll, enter to inspect");
  wrefresh(topLine);
  promptcarryslot(cur);
  while(j==0){
    ch = getch();
    if(ch==27){
      display();
      wrefresh(board);
      wclear(topLine);
      wrefresh(topLine);
      wclear(bottomLine);
      wrefresh(bottomLine);
      j=1;
    }
    if(ch==KEY_UP){
      if(cur>12){
	cur--;
	promptcarryslot(cur);
      }
    }
    if(ch==KEY_DOWN){
      if(cur<21){
	cur++;
	promptcarryslot(cur);
      }
    }
    if(ch==10){
      wmove(bottomLine,0,0);
      wprintw(bottomLine,"%s",pc->equiped[cur]->description.c_str());
      wrefresh(bottomLine);
      promptcarryslot(cur);
    }
  }
}

void teleport(){
  int ch,s,telx,tely,i,j,k;
  s=1;
  telx = (*pc).x;
  tely = (*pc).y;
  while(s==1){
    wmove(board,0,0);
    for(i=0; i<21; i++){
      for(j=0; j<80; j++){
	if(i==tely && j==telx){
	  wprintw(board,"%c",'*');
	}
	else if(monLocations[i][j]!='g'){
	  wprintw(board,"%c",monLocations[i][j]);
	}
	else{
	  wprintw(board,"%c",map[i][j]);
	}
      }
    }
    wrefresh(board);
    ch = getch();
    switch(ch){
    case 'y':
    case '7':
      if(hardness[tely-1][telx-1]!=255){
	tely--;
	telx--;
      }
      break;
    case 'k':
    case '8':
      if(hardness[tely-1][telx]!=255){
	tely--;
      }
      break;
    case 'u':
    case '9':
      if(hardness[tely-1][telx+1]!=255){
	tely--;
	telx++;
      }
      break;
    case 'l':
    case '6':
      if(hardness[tely][telx+1]!=255){
        telx++;
      }
      break;
    case 'n':
    case '3':
      if(hardness[tely+1][telx+1]!=255){
	tely++;
	telx++;
      }
      break;
    case 'j':
    case '2':
      if(hardness[tely+1][telx]!=255){
	tely++;
      }
      break;
    case 'b':
    case '1':
      if(hardness[tely+1][telx-1]!=255){
	tely++;
	telx--;
      }
      break;
    case 'h':
    case '4':
      if(hardness[tely][telx-1]!=255){
        telx--;
      }
      break;
    case 't':
      hardness[tely][telx]=0;
      //map[tely][telx]='#';
      (*pc).y = tely;
      (*pc).x = telx;
      display();
      wrefresh(board);
      s=0;
      break;
    case 'r':
      (*pc).y = rand()%19+1;
      (*pc).x = rand()%78+1;
      display();
      wrefresh(board);
      s=0;
      break;
    }
  }
}

void pcMove(int shortestpath[21][80]){
  int ch,s,i,k,l;
  s=1;
  while(s==1){
    ch = getch();
    switch(ch){
    case 'y':
    case '7':
      if(shortestpath[pc->y-1][pc->x-1]!=' '){
	(*pc).y--;
	(*pc).x--;
	s=0;
      }
      break;
    case 'k':
    case '8':
      if(shortestpath[pc->y-1][pc->x]!=' '){
	(*pc).y--;
	s=0;
      }
      break;
    case 'u':
    case '9':
      if(shortestpath[pc->y-1][pc->x+1]!=' '){
	(*pc).y--;
	(*pc).x++;
	s=0;
      }
      break;
    case 'l':
    case '6':
      if(shortestpath[pc->y][pc->x+1]!=' '){
        (*pc).x++;
	s=0;
      }
      break;
    case 'n':
    case '3':
      if(shortestpath[pc->y+1][pc->x+1]!=' '){
	(*pc).y++;
	(*pc).x++;
	s=0;
      }
      break;
    case 'j':
    case '2':
      if(shortestpath[pc->y+1][pc->x]!=' '){
	(*pc).y++;
	s=0;
      }
      break;
    case 'b':
    case '1':
      if(shortestpath[pc->y+1][pc->x-1]!=' '){
	(*pc).y++;
	(*pc).x--;
	s=0;
      }
      break;
    case 'h':
    case '4':
      if(shortestpath[pc->y][pc->x-1]!=' '){
        (*pc).x--;
	s=0;
      }
      break;
    case '.':
      if(map[pc->y][pc->x]=='>'){
        while(h.size>0){
	  heap_remove_min(&h);
	}
	createnewdungeon();
	createdownstairs();
	map[pc->y][pc->x]='<';
	(*pc).turn=0;
	for(i=0; i<nummon; i++){
	  heap_decrease_key_no_replace(&h,heap_insert(&h,&mons[i]));
	}
        display();
	wrefresh(board);
	monLocations[pc->y][pc->x]='g';
	createnontunnelingmap();
        pcMove(nontun);
	display();
	wrefresh(board);
	s=0;
      }
      break;
    case ',':
      if(map[pc->y][pc->x]=='<'){
        while(h.size>0){
	  heap_remove_min(&h);
	}
	createnewdungeon();
	createupstairs();
	map[pc->y][pc->x]='>';
	(*pc).turn=0;
	for(i=0; i<nummon; i++){
	  heap_decrease_key_no_replace(&h,heap_insert(&h,&mons[i]));
	}
	display();
	wrefresh(board);
	monLocations[pc->y][pc->x]='g';
	createnontunnelingmap();
        pcMove(nontun);
	display();
	wrefresh(board);
	s=0;
      }
      break;
    case '5':
    case ' ':
      s=0;
      break;
    case 'm':
      monLocations[pc->y][pc->x]='@';
      display();
      wrefresh(board);
      displaymonsterlist();
      break;
    case 'q':
      endwin();
      exit(0);
      break;
    case 'f':
      monLocations[pc->y][pc->x]='@';
      displaynofog();
      wrefresh(board);
      monLocations[pc->y][pc->x]='g';
      break;
    case 'g':
      teleport();
      s=0;
      break;
    case 'w':
      wearitem();
      break;
    case 't':
      takeoff();
      break;
    case 'd':
      drop();
      break;
    case 'x':
      expunge();
      break;
    case 'i':
      getinventorylist();
      break;
    case 'e':
      getequipedlist();
      break;
    case 'I':
      inspectitem();
      break;
    case 'L':
      
      break;
    }
  }
  if(itemLocations[pc->y][pc->x]!='g'){
    for(k=12; k<22; k++){
      if(pc->equiped[k]==NULL){
	for(l=0; l<object_descriptions.size(); l++){
	    if(getObjectColor(object_descriptions[l].type)==itemLocations[pc->y][pc->x]){
	      pc->equiped[k]=&object_descriptions[l];
	      break;
	    }
	}
	itemLocations[pc->y][pc->x]='g';
	break;
      }
    }
  }
}

int hasCharacteristic(character m, int bit){
  return (m.characteristics >> bit) & 1;
}

int32_t compare(const void *key, const void *with)
{
  if((*((character *) key)).turn - (*((character *) with)).turn ==0){
    if((*((character *) key)).ispc == 0){
      return -1;
    }
    if((*((character *) with)).ispc == 0){
      return 1;
    }
  }
  return (*((character *) key)).turn - (*((character *) with)).turn;
}

int istunneling(int y, int x){
  if(hardness[y][x]!=0){
    if(hardness[y][x]<=85){
      hardness[y][x]=0;
      map[y][x]='#';
      return 0;
    }
    else{
      hardness[y][x]=hardness[y][x]-85;
      return 1;
    }
  }
  return 0;
}

int insameroom(character a, character b){
  if(abs(a.y - b.y)<6 && abs(a.x - b.y)<6){
    return 0;
  }
  return 1;
}

void moverandomly(character *t,int shortestpath[21][80]){
  int s,counter;
  while(s!=9){
    s = rand()%8;
    switch(s){
    case 0:
      if(shortestpath[t->y+1][t->x]!=' '){
	if(istunneling(t->y+1,t->x)==0){
	  (*t).y=t->y+1;
	}
	s=9;
      }
    case 1:
      if(shortestpath[t->y][t->x+1]!=' '){
	if(istunneling(t->y,t->x+1)==0){
	  (*t).x=t->x+1;
	}
	s=9;
      }
    case 2:
      if(shortestpath[t->y-1][t->x]!=' '){
	if(istunneling(t->y-1,t->x)==0){
	  (*t).y=t->y-1;
	}
	s=9;
      }
    case 3:
      if(shortestpath[t->y][t->x-1]!=' '){
	if(istunneling(t->y,t->x-1)==0){
	  (*t).x=t->x-1;
	}
	s=9;
      }
    case 4:
      if(shortestpath[t->y+1][t->x+1]!=' '){
	if(istunneling(t->y+1,t->x+1)==0){
	  (*t).y=t->y+1;
	  (*t).x=t->x+1;
	}
	s=9;
      }
    case 5:
      if(shortestpath[t->y+1][t->x-1]!=' '){
	if(istunneling(t->y+1,t->x-1)==0){
	  (*t).y=t->y+1;
	  (*t).x=t->x-1;
	}
	s=9;
      }
    case 6:
      if(shortestpath[t->y-1][t->x+1]!=' '){
	if(istunneling(t->y-1,t->x+1)==0){
	  (*t).y=t->y-1;
	  (*t).x=t->x+1;
	}
	s=9;
      }
    case 7:
      if(shortestpath[t->y-1][t->x-1]!=' '){
	if(istunneling(t->y-1,t->x-1)==0){
	  (*t).y=t->y-1;
	  (*t).x=t->x-1;
	}
	s=9;
      }
    }
    counter++;
    if(counter>=10){
      s=9;
    }
  }
}

void movetounintelligently(character *t, int shortestpath[21][80]){
  if(pc->y<t->y && shortestpath[t->y-1][t->x]!=' '){
    if(istunneling(t->y-1,t->x)==0){
      (*t).y--;
    }
  }
  if(pc->y>t->y && shortestpath[t->y+1][t->x]!=' '){
    if(istunneling(t->y+1,t->x)==0){
      (*t).y++;
    }
  }
  if(pc->x<t->x && shortestpath[t->y][t->x-1]!=' '){
    if(istunneling(t->y,t->x-1)==0){
      (*t).x--;
    }
  }
  if(pc->x>t->x && shortestpath[t->y][t->x+1]!=' '){
    if(istunneling(t->y,t->x+1)==0){
      (*t).x++;
    }
  }
}

void moveintelligently(character *t, int shortestpath[21][80]){
  int cursmy,cursmx;
  cursmy = t->y+1;
  cursmx = t->x;
  if(shortestpath[t->y+1][t->x-1]!=' ' && shortestpath[t->y+1][t->x-1]<shortestpath[cursmy][cursmx]){
    cursmy = t->y+1;
    cursmx = t->x-1;
  }
  if(shortestpath[t->y][t->x-1]!=' ' && shortestpath[t->y][t->x-1]<shortestpath[cursmy][cursmx]){
    cursmy = t->y;
    cursmx = t->x-1;
  }
  if(shortestpath[t->y-1][t->x-1]!=' ' && shortestpath[t->y-1][t->x-1]<shortestpath[cursmy][cursmx]){
    cursmy = t->y-1;
    cursmx = t->x-1;
  }
  if(shortestpath[t->y-1][t->x]!=' ' && shortestpath[t->y-1][t->x]<shortestpath[cursmy][cursmx]){
    cursmy = t->y-1;
    cursmx = t->x;
  }
  if(shortestpath[t->y-1][t->x+1]!=' ' && shortestpath[t->y-1][t->x+1]<shortestpath[cursmy][cursmx]){
    cursmy = t->y-1;
    cursmx = t->x+1;
  }
  if(shortestpath[t->y][t->x+1]!=' ' && shortestpath[t->y][t->x+1]<shortestpath[cursmy][cursmx]){
    cursmy = t->y;
    cursmx = t->x+1;
  }
  if(shortestpath[t->y+1][t->x+1]!=' ' && shortestpath[t->y+1][t->x+1]<shortestpath[cursmy][cursmx]){
    cursmy = t->y+1;
    cursmx = t->x+1;
  }
  if(istunneling(cursmy,cursmx)==0){
    (*t).y = cursmy;
    (*t).x = cursmx;
  }
}		  

void newmons(){
  int i,nooverlap,randtype;
  while(i<nummon){
    nooverlap = 0;
    while(nooverlap==0){
      mons[i]->characteristics = rand()%16;
      int room;
      room = (rand() % roomNum);
      mons[i]->y = (rand() % rooms[room][2]) + rooms[room][0];
      mons[i]->x = (rand() % rooms[room][3]) + rooms[room][1];
      if(monLocations[mons[i]->y][mons[i]->x]=='g'){ 
	nooverlap=1;
      }
    }
    randtype = rand()%monsters.size();
    mons[i]->name = monsters[randtype]->name;
    mons[i]->letter = monsters[randtype]->letter;
    mons[i]->color = monsters[randtype]->color;
    mons[i]->description = monsters[randtype]->description;
    mons[i]->hp = monsters[randtype]->hp;
    mons[i]->speed = monsters[randtype]->speed;
    mons[i]->damage = monsters[randtype]->damage;
    mons[i]->abilities = monsters[randtype]->abilities;
    mons[i]->ispc=1;
    mons[i]->ded=0;
    mons[i]->turn=0;
    //mons[i].speed = rand()%15 + 5;
    /*switch(mons[i].characteristics & 0xf){
    case 0:
      mons[i].letter = '0';
      break;
    case 1:
      mons[i].letter = '1';
      break;
    case 2:
      mons[i].letter = '2';
      break;
    case 3:
      mons[i].letter = '3';
      break;
    case 4:
      mons[i].letter = '4';
      break;
    case 5:
      mons[i].letter = '5';
      break;
    case 6:
      mons[i].letter = '6';
      break;
    case 7:
      mons[i].letter = '7';
      break;
    case 8:
      mons[i].letter = '8';
      break;
    case 9:
      mons[i].letter = '9';
      break;
    case 10:
      mons[i].letter = 'a';
      break;
    case 11:
      mons[i].letter = 'b';
      break;
    case 12:
      mons[i].letter = 'c';
      break;
    case 13:
      mons[i].letter = 'd';
      break;
    case 14:
      mons[i].letter = 'e';
      break;
    case 15:
      mons[i].letter = 'f';
      break;
      }*/
    if(hasCharacteristic((character)*mons[i],7)){
	uniques[i]=true;
    }
    monLocations[mons[i]->y][mons[i]->x]=mons[i]->letter;
    i++;
  }
}

void createMonsters(int num){
  int i,j,nooverlap,randtype;
  heap_init(&h, compare, free);
  parseMonsters();
  parse_descriptions();
  createObjects();
  heap_decrease_key_no_replace(&h,heap_insert(&h,pc));
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      if(monLocations[i][j]!='@'){
	monLocations[i][j] = 'g';
      }
    }
  }
  i=0;
  //mons = (npc *) malloc(num * sizeof(*mons));
  while(i<num){
    mons.push_back(new npc());
    nooverlap = 0;
    while(nooverlap==0){
      int room;
      room = (rand() % roomNum);
      mons[i]->y = (rand() % rooms[room][2]) + rooms[room][0];
      mons[i]->x = (rand() % rooms[room][3]) + rooms[room][1];
      if(monLocations[mons[i]->y][mons[i]->x]=='g'){ 
	nooverlap=1;
      }
    }
    randtype = rand()%monsters.size();
    while(true){
      if(uniques[randtype]=true) break;
      randtype = rand()%monsters.size();
    }
    mons[i]->name = monsters[randtype]->name;
    mons[i]->letter = monsters[randtype]->letter;
    mons[i]->color = monsters[randtype]->color;
    mons[i]->description = monsters[randtype]->description;
    mons[i]->hp = monsters[randtype]->hp;
    mons[i]->speed = monsters[randtype]->speed;
    mons[i]->damage = monsters[randtype]->damage;
    mons[i]->abilities = monsters[randtype]->abilities;
    mons[i]->ispc = 1;
    mons[i]->ded = 0;
    mons[i]->turn = 0;
    //mons[i].speed = rand()%15 + 5;
    //mons[i].characteristics = rand()%16;
    /*switch(mons[i].characteristics & 0xf){
    case 0:
      mons[i].letter = '0';
      break;
    case 1:
      mons[i].letter = '1';
      break;
    case 2:
      mons[i].letter = '2';
      break;
    case 3:
      mons[i].letter = '3';
      break;
    case 4:
      mons[i].letter = '4';
      break;
    case 5:
      mons[i].letter = '5';
      break;
    case 6:
      mons[i].letter = '6';
      break;
    case 7:
      mons[i].letter = '7';
      break;
    case 8:
      mons[i].letter = '8';
      break;
    case 9:
      mons[i].letter = '9';
      break;
    case 10:
      mons[i].letter = 'a';
      break;
    case 11:
      mons[i].letter = 'b';
      break;
    case 12:
      mons[i].letter = 'c';
      break;
    case 13:
      mons[i].letter = 'd';
      break;
    case 14:
      mons[i].letter = 'e';
      break;
    case 15:
      mons[i].letter = 'f';
      break;
    }*/
    if(hasCharacteristic((character)*mons[i],7)){
	uniques[i]=true;
    }
    monLocations[mons[i]->y][mons[i]->x]=mons[i]->letter;
    heap_decrease_key_no_replace(&h,heap_insert(&h,mons[i]));
    i++;
  }
}

void moveMon(character *t){
  int (*shortestpath)[21][80];
  int r = 0;
  if(t->ispc==0){
    shortestpath = &nontun;
    pcMove(*shortestpath);
  }
  else{
    if(hasCharacteristic(*t,1)==1){
      shortestpath = &tun;
    }
    else{
      shortestpath = &nontun;
    }
    if(hasCharacteristic(*t,0)==1){
      r = rand()%2;
      if(r==1){
        moverandomly(t,*shortestpath);
      }
    }
    if(r==0){
      switch(((*t).characteristics >> 2) & 3){
      case 0:
	if(insameroom(*pc,*t)==0){
	  movetounintelligently(t,*shortestpath);
 	}
	else{
	  moverandomly(t,*shortestpath);
	}
	break;
      case 1:
	movetounintelligently(t,*shortestpath);
	break;
      case 2:
	if(insameroom(*pc,*t)==0){
	  moveintelligently(t,*shortestpath);
	}
	else{
	  moverandomly(t,*shortestpath);
	}
	break;
      case 3:
        moveintelligently(t,*shortestpath);
	break;
      }
    }
  }
  monLocations[t->y][t->x]=t->letter;
  (*t).turn = (*t).turn + (1000/(*((*t).speed)).roll());
}

int nextTurn(){
  character *cur;
  cur = (character *) heap_remove_min(&h);
  if(monLocations[cur->y][cur->x]!=cur->letter){
    (*cur).ded=1;
  }
  else{
    monLocations[cur->y][cur->x]='g';
  }
  if(cur->ded==0){
    moveMon(cur);
    heap_decrease_key_no_replace(&h,heap_insert(&h,(void *)cur));
  }
  if(pc->ded==1){
    wprintw(topLine,"%s\n","lose");
    //free(mons);
    wrefresh(topLine);
    getch();
    return 1;
  }
  if(h.size==1){
    wprintw(topLine,"%s\n","win");
    //free(mons);
    wrefresh(topLine);
    getch();
    return 1;
  }
  return 0;
}
