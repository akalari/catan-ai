#include <stddef.h>
#include <iostream>
#include <stdio.h>


#include "Board.h"
#include "Tile.h"
#include "Corner.h"
#include "Edge.h"

using namespace std;

Tile tiles[19];
Corner corners[54];
Edge edges[72];


void initBoard() {
  int nextEdge = 0;

  for(int t = 0; t < 19; t++) {
    Tile tile = tiles[t];

    //load adjacent tiles to tile.adjTiles{}
    for(int e=0; e < 6; e++) {
      int adj = TILE_ADJACENCIES[t][e];
      if(adj == -1) tile.adjTiles[e] = NULL;
      else tile.adjTiles[e] = &tiles[adj];
    }
  }
}

void printTiles(char tc[19], int ti[19]) {
    for(int i = 0; i < 19; i++) {
      if(i == 0 || i == 16) cout << "\n     ";
      else if(i == 3 || i == 12) cout << "\n   ";
      else if(i == 7)cout << "\n";

      if(tc[i])printf("[%c%02d]", tc[i], ti[i]);
      else printf("[ %1d ]", i);
    }
    cout << "\n\n";
}

void inputBoard() {
  char tc[19] = {0};
  int ti[19] {0};

  for(int i = 0; i < 19; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod][1-12] -> ";
    cin >> tc[i] >> ti[i];
  }
}

void inputBoard(const int possibilitiesSpiral[18]) {
  char tc[19] = {0};
  int ti[19] {0};

  for(int i = 0; i < 19; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod]-> ";
    cin >> tc[i];
    cout << endl;
  }

  int c = 0;
  for(int i = 0; i < 19; i++){
      if(tc[TILES_SPIRAL[i]] == 'd')continue;
      ti[TILES_SPIRAL[i]] = possibilitiesSpiral[c++];
  }

  cout << "Game board with assigned possibilities: " << endl;
  printTiles(tc, ti);

}

int main(){
  inputBoard(TILE_POSSIBILITIES);
  return 0;
}
