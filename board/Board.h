#ifndef BOARD_H
#define BOARD_H

#include <string> // std::string
#include <vector> // std::vector
#include "../elements/Elements.h"

using namespace std;

const int NUM_TILES = 19;
const int NUM_CORNERS = 54;
const int NUM_EDGES = 72;
const int NUM_PORTS = 9;
const int CORNERS_PER_TILE = 6;
const int EDGES_PER_TILE = 6;

const int TILE_POSSIBILITIES[] =
  {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
const int TILES_SPIRAL[] =
  {0, 1, 2, 6, 11, 15, 18, 17, 16, 12, 7, 3, 4, 5, 10, 14, 13, 8, 9};

const int BOARD_PORTS[][4]
{    // tile, edge, resource, term
    {0, 0, -1, 3},
    {1, 1, GRAIN, 2},
    {3, 5, LUMBER, 2},
    {6, 1, ORE, 2},
    {11, 2, -1, 3},
    {12, 5, BRICK, 2},
    {15, 3, WOOL, 2},
    {16, 4, -1, 3},
    {17, 3, -1, 3}
};

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

class Corner;
class Edge;
class Tile;
class Port;

class Tile {
 private:
   int adjCorners[6];
   int adjEdges[6];
   int num;
   int resource;
   bool robber;
   int index;
 public:
   Tile();
   int (&getAdjCorners())[6];
   int (&getAdjEdges())[6];
   int getNum();
   void setNum(int n);
   int getResource();
   void setResource(int res);
   int getIndex();
   void setIndex(int ind);
   bool getRobber();
   void setRobber(bool robberStatus);
};

class Edge {
 private:
   vector<int> adjCorners;
   int road;
   int index;
 public:
   static const int NONE = -2;
   vector<int> &getAdjCorners();
   int getRoad();
   void setRoad(int color);
   int getIndex();
   void setIndex(int ind);
   Edge();
};

class Corner {
 private:
   vector<int> adjTiles; // Variable length: {1, 2, 3}
   vector<int> adjEdges; // Variable length: {2, 3}
   int adjPort; // Can be null
   int settlement;
   int index;
 public:
   static const int NO_SETTLEMENT = -2;
   vector<int> &getAdjTiles();
   vector<int> &getAdjEdges();
   int getPort();
   void setPort(int trader);
   int getIndex();
   void setIndex(int ind);
   Corner();
};

//TODO: treat bank like a port
class Port {
 private:
   int adjCorners[2];
   int resource;
   int terms; // Number required to trade
   int index;
 public:
   int (&getAdjCorners())[2];
   int getResource();
   void setResource(int res);
   int getTerms();
   void setTerms(int tms);
   int getIndex();
   void setIndex(int ind);
   Port();
   Port(int r, int t);
};

void initBoard();
void initTiles();
void initEdges();
void initCorners();

#endif
