#include <limits.h> 
#include "paths.h"
#include "monsters.h"
#include "dungeon.h"

int nontun[21][80];
int tun[21][80];
int checkednon[21][80];
int checked[21][80];

void createrandomspot(){
  int room;
  room = (rand() % roomNum);
  pc->y = (rand() % rooms[room][2]) + rooms[room][0];
  pc->x = (rand() % rooms[room][3]) + rooms[room][1];
}

void checksur(int y, int x, int val){
  if(checkednon[y+1][x]>val+1 && hardness[y+1][x]==0){
    checkednon[y+1][x] = val+1;
    checksur(y+1,x,val+1);
  }
  if(checkednon[y-1][x]>val+1 && hardness[y-1][x]==0){
    checkednon[y-1][x] = val+1;
    checksur(y-1,x,val+1);
  }
  if(checkednon[y][x+1]>val+1 && hardness[y][x+1]==0){
    checkednon[y][x+1] = val+1;
    checksur(y,x+1,val+1);
  }
  if(checkednon[y][x-1]>val+1 && hardness[y][x-1]==0){
    checkednon[y][x-1] = val+1;
    checksur(y,x-1,val+1);
  }
  if(checkednon[y+1][x+1]>val+1 && hardness[y+1][x+1]==0){
    checkednon[y+1][x+1] = val+1;
    checksur(y+1,x+1,val+1);
  }
  if(checkednon[y-1][x-1]>val+1 && hardness[y-1][x-1]==0){
    checkednon[y-1][x-1] = val+1;
    checksur(y-1,x-1,val+1);
  }
  if(checkednon[y+1][x-1]>val+1 && hardness[y+1][x-1]==0){
    checkednon[y+1][x-1] = val+1;
    checksur(y+1,x-1,val+1);
  }
  if(checkednon[y-1][x+1]>val+1 && hardness[y-1][x+1]==0){
    checkednon[y-1][x+1] = val+1;
    checksur(y-1,x+1,val+1);
  }
  //nontun[y][x]=val%10;
  nontun[y][x]=val;
}

void createnontunnelingmap(){
  int i,j;
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      nontun[i][j]=' ';
      if(hardness[i][j]==0){
	checkednon[i][j]=INT_MAX;
      }
    }
  }
  checksur(pc->y,pc->x,0);
  nontun[pc->y][pc->x]='@';
  /*for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      if(nontun[i][j]=='@' || nontun[i][j]==' '){
	printf("%c",nontun[i][j]);
      }
      else{
	printf("%d",nontun[i][j]);
      }
    }
    printf("\n");
  }*/
}

void checksur2(int y, int x, int val){
  if(checked[y+1][x]>val+(hardness[y][x]/85)+1 && hardness[y+1][x]!=255){
    checked[y+1][x] = val+(hardness[y][x]/85)+1;
    checksur2(y+1,x,checked[y+1][x]);
  }
  if(checked[y-1][x]>val+(hardness[y][x]/85)+1 && hardness[y-1][x]!=255){
    checked[y-1][x] = val+(hardness[y][x]/85)+1;
    checksur2(y-1,x,checked[y-1][x]);
  }
  if(checked[y][x+1]>val+(hardness[y][x]/85)+1 && hardness[y][x+1]!=255){
    checked[y][x+1] = val+(hardness[y][x]/85)+1;
    checksur2(y,x+1,checked[y][x+1]);
  }
  if(checked[y][x-1]>val+(hardness[y][x]/85)+1 && hardness[y][x-1]!=255){
    checked[y][x-1] = val+(hardness[y][x]/85)+1;
    checksur2(y,x-1,checked[y][x-1]);
  }
  if(checked[y+1][x+1]>val+(hardness[y][x]/85)+1 && hardness[y+1][x+1]!=255){
    checked[y+1][x+1] = val+(hardness[y][x]/85)+1;
    checksur2(y+1,x+1,checked[y+1][x+1]);
  }
  if(checked[y-1][x-1]>val+(hardness[y][x]/85)+1 && hardness[y-1][x-1]!=255){
    checked[y-1][x-1] = val+(hardness[y][x]/85)+1;
    checksur2(y-1,x-1,checked[y-1][x-1]);
  }
  if(checked[y+1][x-1]>val+(hardness[y][x]/85)+1 && hardness[y+1][x-1]!=255){
    checked[y+1][x-1] = val+(hardness[y][x]/85)+1;
    checksur2(y+1,x-1,checked[y+1][x-1]);
  }
  if(checked[y-1][x+1]>val+(hardness[y][x]/85)+1 && hardness[y-1][x+1]!=255){
    checked[y-1][x+1] = val+(hardness[y][x]/85)+1;
    checksur2(y-1,x+1,checked[y-1][x+1]);
  }
  //tun[y][x]=val%10;
  tun[y][x]=val;
}

void createtunnelingmap(){
  int i,j;
  for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      tun[i][j]=0;
      checked[i][j]=INT_MAX;
      if(hardness[i][j]==255){
	tun[i][j]=' ';
      }
    }
  }
  checksur2(pc->y,pc->x,0);
  tun[pc->y][pc->x]='@';
  /*for(i=0; i<21; i++){
    for(j=0; j<80; j++){
      if(hardness[i][j]==255||(pc.y==i && pc.x==j)){
	printf("%c",tun[i][j]);
      }
      else{
	printf("%d",tun[i][j]);
      }
    }
    printf("\n");
  }*/
}
