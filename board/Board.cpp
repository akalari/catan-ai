#include <iostream> // std::cout, std::endl, etc
#include <vector>

#include "Board.h"

using namespace std;

Tile::Tile():
    adjCorners {-1, -1, -1, -1, -1, -1},
    adjEdges {-1, -1, -1, -1, -1, -1},
    robber(false), num(-1), resource(-1), index(-1)
{}

int (&Tile::getAdjCorners())[6] { return adjCorners; }
int (&Tile::getAdjEdges())[6] { return adjEdges; }
int Tile::getNum() { return num; }
void Tile::setNum(int n) { num = n; }
int Tile::getResource() { return resource; }
void Tile::setResource(int res) { resource = res; }
int Tile::getIndex() { return index; }
void Tile::setIndex(int ind) { index = ind; }
bool Tile::getRobber() { return robber; }
void Tile::setRobber(bool robberStatus) { robber = robberStatus; }

Edge::Edge():
    road(Edge::NONE)
{}

vector<int> (&Edge::getAdjCorners()) { return adjCorners; }
int Edge::getRoad() { return road; }
void Edge::setRoad(int color) { road = color; }
int Edge::getIndex() { return index; }
void Edge::setIndex(int ind) { index = ind; }

Corner::Corner():
    settlement(Corner::NO_SETTLEMENT),
    adjPort(0),
    city(-1)
{}

vector<int> (&Corner::getAdjTiles()) { return adjTiles; }
vector<int> (&Corner::getAdjEdges()) { return adjEdges; }
int Corner::getPort() { return adjPort; }
void Corner::setPort(int trader) { trader = adjPort; }
int Corner::getSettlement() { return settlement; }
void Corner::setSettlement(int color) { settlement = color; }
int Corner::getCity() { return city; }
void Corner::setCity(int color) { city = color; }
int Corner::getIndex() { return index; }
void Corner::setIndex(int ind) { index = ind; }

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

int (&Port::getAdjCorners())[2] { return adjCorners; }
int (Port::getResource()) { return resource; }
void (Port::setResource(int res)) { resource = res; }
int Port::getTerms() { return terms; }
void Port::setTerms(int tms) { terms = tms; }
int Port::getIndex() { return index; }
void Port::setIndex(int ind) { index = ind; }

Board::Board() { this->initBoard(); }
Tile (&Board::getTiles())[NUM_TILES] { return tiles; }
Edge (&Board::getEdges())[NUM_EDGES] { return edges; }
Corner (&Board::getCorners())[NUM_CORNERS] { return corners; }
Port (&Board::getPorts())[NUM_PORTS] { return ports; }

/**
 * Assigns relations between tiles, edges, and corners
 * Populates pointer arrays in each class
 */
void Board::initBoard() {
    initTiles();
    initEdges();
    initCorners();
}

/**
 * Initialize board tiles
 */
void Board::initTiles() {
    for(int t = 0; t < NUM_TILES; t++) {
        tiles[t] = Tile();
        tiles[t].setIndex(t);
    }
}

/**
 * Initialize board edges
 */
void Board::initEdges() {
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
                edges[nextEdge].setIndex(nextEdge);
                tile.getAdjEdges()[e] = nextEdge;
                nextEdge++;
            } // If this is a null shared edge, create a new instance
            else if(tile.getAdjEdges()[e] == -1){
                edges[nextEdge] = Edge();
                edges[nextEdge].setIndex(nextEdge);
                tile.getAdjEdges()[e] = nextEdge;
                // Assign this object to the adjacent tile
                tiles[a].getAdjEdges()[(e+3)%6] = nextEdge;
                nextEdge++;
            }
        }
    }
}

/**
 * Set up board corners
 */
void Board::initCorners() {
    int nextCorner = 0;
    // Loop through all the tiles
    for (int t = 0; t < NUM_TILES; t++) {

        Tile &tile = tiles[t];
        //Loop through all the corners, with indexes of x.5
        for(double c = 0.5; c < 6; c += 1) {
            // If this corner already exists, skip
            if(tile.getAdjCorners()[int(c)] != -1) continue;
            // Create new Corner instance and add to list
            Corner &corner = corners[nextCorner] = Corner();
            corner.setIndex(nextCorner);

            int lEdge = int(c-0.5); // Index of edge counter-clockwise of corner
            int rEdge = int(c+0.5)%6; // Index of edge clockwise of corner

            int adjTA = TILE_ADJACENCIES[t][lEdge]; // Tile adjacent at lEdge
            int adjTB = TILE_ADJACENCIES[t][rEdge]; // Tile adjacent at rEdge

            corner.getAdjTiles().push_back(t); // Add tile to corner's adjacent tiles
            tile.getAdjCorners()[int(c)] = nextCorner; // Add corner to tile's list

            int thirdEdge = -1; // Temporarily store the third edge, if it exists

            if(adjTA != -1) { // If there is an adjacent tile at lEdge
                corner.getAdjTiles().push_back(adjTA); // Add adj tile to corner
                tiles[adjTA].getAdjCorners()[int(c+2)%6] = nextCorner; // Add corner to adj tile
                thirdEdge = tiles[adjTA].getAdjEdges()[(lEdge + 2)%6]; // Get third edge from adjTA
            }
            if(adjTB != -1) { // If there is an adjacent tile at rEdge
                 corner.getAdjTiles().push_back(adjTB); // Add adj tile to corner
                 tiles[adjTB].getAdjCorners()[int(c+4)%6] = nextCorner; // Add corner to adj tile
                 thirdEdge = tiles[adjTB].getAdjEdges()[(rEdge + 4)%6]; // Get third edge from adjTB
            }

            // Add adjacent edges to corner, and add corner to edges
            int el = tile.getAdjEdges()[lEdge];
            corner.getAdjEdges().push_back(el);
            edges[el].getAdjCorners().push_back(nextCorner);
            int er = tile.getAdjEdges()[rEdge];
            corner.getAdjEdges().push_back(er);
            edges[er].getAdjCorners().push_back(nextCorner);
            if(thirdEdge != -1) {
                corner.getAdjEdges().push_back(thirdEdge);
                edges[thirdEdge].getAdjCorners().push_back(nextCorner);
            }

            nextCorner++;
        }
    }
}

/**
 * Set up board ports
 */
void Board::initPorts() {
    for(int p = 0; p < NUM_PORTS; p++) {
        int i[4];
        std::copy(std::begin(BOARD_PORTS[p]), std::end(BOARD_PORTS[p]), std::begin(i));
        Tile &t = tiles[i[0]];
        int e = t.getAdjEdges()[i[1]];
        Edge& edge = edges[e];
        ports[p] = Port(i[2], i[3]);
        ports[p].setIndex(p);

        for(int c = 0; c < 2; c++) {
            ports[p].getAdjCorners()[c] = edge.getAdjCorners().at(c);
            corners[edge.getAdjCorners().at(c)].setPort(p);
        }

    }
}

void Board::buildSettlement(int corner, int player) {
    corners[corner].setSettlement(player);
}

void Board::buildRoad(int edge, int player) {
    edges[edge].setRoad(player);
}
