#include <vector>

#ifndef BOARD_H
#define BOARD_H

const int TILE_POSSIBILITIES[] =
  {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
const int TILES_SPIRAL[] =
  {0, 1, 2, 6, 11, 15, 18, 17, 16, 12, 7, 3, 4, 5, 10, 14, 13, 8, 9};

const int TILE_ADJACENCIES[19][6] =
  {{-1, -1, 1, 4, 3, -1},    //0
  {-1, -1, 2, 5, 4, 0},      //1
  {-1, -1, -1, 6, 5, 1},     //2
  {-1, 0, 4, 8, 7, -1},      //3
  {0, 1, 5, 9, 8, 3},        //4
  {1, 2, 6, 10, 9, 4},       //5
  {2, -1, -1, 11, 10, 5},    //6
  {-1, 3, 8, 12, -1, -1},    //7
  {3, 4, 9, 13, 12, 7},      //8
  {4, 5, 10, 14, 13, 8},     //9
  {5, 6, 11, 15, 14, 9},     //10
  {6, -1, -1, -1, 15, 10},   //11
  {7, 8, 13, 16, -1, -1},    //12
  {8, 9, 14, 17, 16, 12},    //13
  {8, 10, 15, 18, 17, 13},   //14
  {10, 11, -1, -1, 18, 14},  //15
  {12, 13, 17, -1, -1 , -1}, //16
  {13, 14, 18, -1, -1, 16},  //17
  {14, 15, -1, -1, -1, 17}}; //18

void initBoard();

struct Corner;
struct Edge;
struct Tile;
struct Port;

struct Tile {
  Corner* adjCorners[6];
  Edge* adjEdges[6];
  int num;
  int resource;
  bool robber;

  Tile();
};

struct Edge {
  std::vector<Corner*> adjCorners;
  int road;

  Edge();
};

struct Corner {
  std::vector<Tile*> adjTiles; // Variable length: {1, 2, 3}
  std::vector<Edge*> adjEdges; // Variable length: {2, 3}
  Port* adjPort; // Can be null
  int settlement;

  Corner();
};

struct Port {
  Corner* adjCorners[2];
  int resource;
};

#endif
