#include <iostream> // std::cout, std::endl, etc
#include <vector>

#include "Board.h"

using namespace std;

Tile tiles[NUM_TILES];
Corner corners[NUM_CORNERS];
Edge edges[NUM_EDGES];
Port ports[NUM_PORTS];

Tile::Tile():
    adjCorners {0, 0, 0, 0, 0, 0},
    adjEdges {0, 0, 0, 0, 0, 0},
    robber(false), num(-1), resource(-1)
{}

&int[6] Tile::getAdjCorners() { return &adjCorners[6]; }
&int[6] Tile::getAdjEdges() { return &adjEdges[6]; }
int Tile::getNum() { return num; }
void Tile::setNum(int n) { num = n; }
int Tile::getResource() { return resource; }
void Tile::setResource(int res) { resource = res; }
int Tile::getIndex() { return index; }
int Tile::setIndex(int ind) { index = ind; }
bool Tile::getRobber() { return robber; }
void Tile::setRobber(bool robberStatus) { robber = robberStatus; }

Edge::Edge():
    road(Edge::NONE)
{}

&vector<int> Edge::getAdjCorners() { return &adjCorners<int>; }
int Edge::getRoad() { return road; }
void Edge::setRoad(int color) { road = color; }
int Edge::getIndex() { return index; }
int Edge::setIndex(int ind) { index = ind; }

Corner::Corner():
    settlement(Corner::NO_SETTLEMENT),
    adjPort(0)
{}

&vector<int> Corner::getAdjTiles() { return &adjTiles<int>; }
&vector<int> Corner::getAdjEdges() { return &adjEdges<int>; }
int Corner::getPort() { return adjPort; }
void Corner::setPort(int trader) { trader = adjPort; }
int Corner::getSettlement() { return settlement; }
void Corner::setSettlement(int color) { settlement = color; }
int Corner::getIndex() { return index; }
int Corner::setIndex() { index = ind; }

Port::Port():
    adjCorners {0, 0},
    resource(-1),
    terms(4)
{}

Port::Port(int r, int t):
  adjCorners {0, 0},
  resource(r),
  terms(t)
{}

&int[2] Port::getAdjCorners() { return &adjCorners[2]; }
int Port::getResource() { return resource; }
void Port::setResource(int res) { resource = res; }
int Port::getTerms() { return terms; }
void Port::setTerms(int tms) { terms = tms; }
int Port::getIndex() { return index; }
void Port::setIndex() { index = ind; }

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
 * Set up board ports
 */
void initPorts() {
    for(int p = 0; p < NUM_PORTS; p++) {
        int i[4];
        std::copy(std::begin(BOARD_PORTS[p]), std::end(BOARD_PORTS[p]), std::begin(i));
        Tile &t = tiles[i[0]];
        Edge* e = t.adjEdges[i[1]];
        ports[p] = Port(i[2], i[3]);

        for(int c = 0; c < 2; c++) {
            ports[p].adjCorners[c] = e->adjCorners.at(c);
            e->adjCorners.at(c)->adjPort = &ports[p];
        }

    }
}
