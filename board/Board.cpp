#include <cstddef>
#include <iostream>
#include <stdio.h>

#include "Board.h"

using namespace std;

Tile tiles[NUM_TILES];
Corner corners[NUM_CORNERS];
Edge edges[NUM_EDGES];

Tile::Tile():
    adjCorners {0, 0, 0, 0, 0, 0},
    adjEdges {0, 0, 0, 0, 0, 0},
    robber(false), num(-1), resource(-1)
{}

Edge::Edge():
    road(-1)
{}

Corner::Corner():
    settlement(-1)
{}

/**
 * Assigns relations between tiles, edges, and corners
 * Populates pointer arrays in each class
 */
void initBoard() {
    initTiles();
    initEdges();
    initCorners();
}

/**
 * Initialize board tiles
 */
void initTiles() {
    for(int t = 0; t < NUM_TILES; t++) {
        tiles[t] = Tile();
    }
}

/**
 * Initialize board edges
 */
void initEdges() {
    int nextEdge = 0;
    // Loop through each tile on the board
    for(int t = 0; t < NUM_TILES; t++) {
        Tile &tile = tiles[t];

        // Loop through all of the edges of the tile
        for(int e = 0; e < EDGES_PER_TILE; e++) {
            int a = TILE_ADJACENCIES[t][e]; // Index of adjacent tile

            // If this is an unshared edge, create a new instance
            if(a == -1){
                edges[nextEdge] = Edge();
                tile.adjEdges[e] = &edges[nextEdge++];
            } // If this is a null shared edge, create a new instance
            else if(tile.adjEdges[e] == 0){
                edges[nextEdge] = Edge();
                tile.adjEdges[e] = &edges[nextEdge];
                // Assign this object to the adjacent tile
                tiles[a].adjEdges[(e+3)%6] = &edges[nextEdge];
                nextEdge++;
            }
        }
    }
}

/**
 * Set up board corners
 */
void initCorners() {
    int nextCorner = 0;
    // Loop through all the tiles
    for (int t = 0; t < NUM_TILES; t++) {
        Tile &tile = tiles[t];
        //Loop through all the corners, with indexes of x.5
        for(double c = 0.5; c < 6; c += 1) {
            // If this corner already exists, skip
            if(tile.adjCorners[int(c)] != 0) continue;
            // Create new Corner instance and add to list
            Corner &corner = corners[nextCorner++] = Corner();

            int lEdge = int(c-0.5); // Index of edge counter-clockwise of corner
            int rEdge = int(c+0.5)%6; // Index of edge clockwise of corner

            int adjTA = TILE_ADJACENCIES[t][lEdge]; // Tile adjacent at lEdge
            int adjTB = TILE_ADJACENCIES[t][rEdge]; // Tile adjacent at rEdge

            corner.adjTiles.push_back(&tile); // Add tile to corner's adjacent tiles
            tile.adjCorners[int(c)] = &corner; // Add corner to tile's list
            Edge *thirdEdge = 0; // Temporarily store the third edge, if it exists

            if(adjTA != -1) { // If there is an adjacent tile at lEdge
                corner.adjTiles.push_back(&tiles[adjTA]); // Add adj tile to corner
                tiles[adjTA].adjCorners[int(c+2)%6] = &corner; // Add corner to adj tile
                thirdEdge = tiles[adjTA].adjEdges[(lEdge + 2)%6]; // Get third edge from adjTA
            }
            if(adjTB != -1) { // If there is an adjacent tile at rEdge
                 corner.adjTiles.push_back(&tiles[adjTB]); // Add adj tile to corner
                 tiles[adjTB].adjCorners[int(c+4)%6] = &corner; // Add corner to adj tile
                 thirdEdge = tiles[adjTB].adjEdges[(rEdge + 4)%6]; // Get third edge from adjTB
            }

            // Add adjacent edges to corner, and add corner to edges
            corner.adjEdges.push_back(tile.adjEdges[lEdge]);
            tile.adjEdges[lEdge]->adjCorners.push_back(&corner);
            corner.adjEdges.push_back(tile.adjEdges[rEdge]);
            tile.adjEdges[rEdge]->adjCorners.push_back(&corner);
            if(thirdEdge != 0) {
                corner.adjEdges.push_back(thirdEdge);
                thirdEdge->adjCorners.push_back(&corner);
            }

        }

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
    initBoard();
    return 0;
}
