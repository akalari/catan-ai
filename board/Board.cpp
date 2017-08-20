#include <stddef.h>
#include <iostream>

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

int main(){
  return 0;
}
