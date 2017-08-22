#include <cstddef>
#include <iostream>
#include <stdio.h>

#include "Board.h"

using namespace std;

const int NUM_TILES = 19;
const int NUM_CORNERS = 54;
const int NUM_EDGES = 72;
const int CORNERS_PER_TILE = 6;
const int EDGES_PER_TILE = 6;

Tile tiles[NUM_TILES];
Corner corners[NUM_CORNERS];
Edge edges[NUM_EDGES];

Tile:Tile() {
    adjEdges = {NULL, NULL, NULL, NULL, NULL, NULL};
    adjSettlements = {NULL, NULL, NULL, NULL, NULL, NULL};
}

Edge:Edge() {
    adjCorners = {NULL, NULL};
    road = -1;
}

/**
 * Assigns relations between tiles, edges, and corners
 * Populates pointer arrays in each class
 */
void initBoard() {
    int nextEdgeIndex = 0;

  // Loop through each tile on the board
  for(int t = 0; t < NUM_TILES; t++) {
    Tile tile = tiles[t] = Tile();

    //iterate through all of the edges of the tile, to make sure they are assigned
    for(int e = 0; e < EDGES_PER_TILE; e++) {
        int adjTile = TILE_ADJACENCIES[t][e];
        //if this is an edge not shared with any other tile, add to the list
        if(adjTile == -1){
            edges[nextEdgeIndex] = Edge();
            tile.adjEdges[e] = &edges[nextEdgeIndex++];
        //if this edge is shared but still unassigned, add to the list
        } else if(tiles[adjTile].adjEdges[(e-3)%6] == NULL) {
            edges[nextEdgeIndex] = Edge();
            tile.adjEdges[e] = &edges[nextEdgeIndex++];
        //if this edge is shared and already assigned, copy the pointer
        } else
            tile.adjEdges[e] = tiles[adjTiles].adjEdges[(e-3)%6];
    }

    // // Loop through the corners of each tile
    // for(int e = 0; e < CORNERS_PER_TILE; e++) {
    //   int adj = TILE_ADJACENCIES[t][e];
    //   // Assign the adjacent tile
    //   if(adj < 0) tile.adjTiles[e] = NULL;
    //   else tile.adjTiles[e] = &tiles[adj];
    // }
  }
}

/**
 * Prints out the tiles
 */
void printTiles(char tc[NUM_TILES], int ti[NUM_TILES]) {

    for(int i = 0; i < NUM_TILES; i++) {
      if(i == 0 || i == 16) cout << "\n     ";
      else if(i == 3 || i == 12) cout << "\n   ";
      else if(i == 7)cout << "\n";
      if(tc[i])printf("[%c%02d]", tc[i], ti[i]);
      else printf("[ %1d ]", i);
    }

    cout << "\n\n";
}

/**
 * Allows the user to input the values for the board resources only
 */
void inputBoard() {
  char tc[NUM_TILES] = {0};
  int ti[NUM_TILES] {0};

  for(int i = 0; i < NUM_TILES; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod][1-12] -> ";
    cin >> tc[i] >> ti[i];
  }
}

/**
 * Allows the user to input the values for the board resources and probabilities
 */
void inputBoard(const int possibilitiesSpiral[18]) {
  char tc[NUM_TILES] = {0};
  int ti[NUM_TILES] {0};

  for(int i = 0; i < NUM_TILES; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod]-> ";
    cin >> tc[i];
    cout << endl;
  }

  int c = 0;
  for(int i = 0; i < NUM_TILES; i++){
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
