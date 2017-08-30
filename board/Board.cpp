#include <cstddef>
#include <iostream>
#include <stdio.h>

#include "Board.h"

Tile tiles[NUM_TILES];
Corner corners[NUM_CORNERS];
Edge edges[NUM_EDGES];
Port ports[NUM_PORTS];

Tile::Tile():
    adjCorners {0, 0, 0, 0, 0, 0},
    adjEdges {0, 0, 0, 0, 0, 0},
    robber(false), num(-1), resource(-1)
{}

Edge::Edge():
	road(Edge::NONE)
{}

Corner::Corner():
    settlement(Corner::NO_SETTLEMENT),
	adjPort(0)
{}

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

/**
 * Returns the corner connected to the edge
 * that is not *c
 */
Corner* getOtherCorner(Edge* e, Corner* c) {
	Corner* front = e->adjCorners.front();
	if (front == c) return e->adjCorners.back();
	else return front;
}

/**
 * Returns a list of tiles with a certain number
 * If the tile has the robber, it isn't included
 */ 
std::vector<Tile*> getTiles(int num) {
	std::vector<Tile*> matches;
	for(Tile &t:tiles) {
		if(t.num == num && !t.robber)
			matches.push_back(&t);
	}
}

/**
 * Returns the list of occupied settlements adjacent
 * to a tile
 */
std::vector<Corner*> getSettlements(Tile *t) {
	std::vector<Corner*> matches;
	for (Corner *c : t->adjCorners)
		if (c->settlement != Corner::NO_SETTLEMENT) matches.push_back(c);
	return matches;
}

/**
 * Returns a list of all of the ports owned by a player
 */
std::vector<Port*> portsOwned(int player) {
	std::vector<Port*> ports;
	for(Corner &c:corners) {
		if(c.settlement == player &&
				c.adjPort != 0)
			ports.push_back(c.adjPort);
	}
	return ports;
}

/**
 * Checks if a settlement (or settlement spot)
 * is >= 2 spots away from other settlements
 */
bool isTwoAway(Corner* settlement) {
	// Check all adjacent edges
	for (Edge *e : settlement->adjEdges) {
		Corner* c = getOtherCorner(e, settlement);
		// If there is a settlement, return false
		if (c->settlement != Corner::NO_SETTLEMENT) return false;
	}
	return true;
}

/**
 * Checks if a new settlement is adjacent to a road
 * occupied by the player
 */
bool adjOwnRoad(Corner* settlement, int player) {
	for (Edge *e : settlement->adjEdges) {
		if(e->road != Edge::NONE && e->road == player) return true;
	}
	return false;
}

/**
 * Checks if a new road is adjacent to a road or settlement occupied
 * by the player
 */
bool adjOwnProperty(Edge* road, int player) {
  for (Corner* c : road->adjCorners) {
    if (c->settlement == player)
      return true;
    for (Edge* e : c->adjEdges)
      if (e->road == player && e != road)
        return true;
  }
  return false;
}

/**
 * Prints the game board in its current state
 * Different segments of each tile are printed, based on its location
 * (see printTile() for more details)
 */ 
void printBoard() {
	std::cout << std::endl << color(-2) << std::endl;
	std::string out[9];
	printStartRow(out, 0);
	printTile(&tiles[0], out, 1,1,0,1);
	printTile(&tiles[1], out, 0,1,0,1);
	printTile(&tiles[2], out, 0,1,0,1);
	printRows(out);
	printStartRow(out, 1);
	printTile(&tiles[3], out, 1,1,0,1);
	printTile(&tiles[4], out, 0,1,0,1);
	printTile(&tiles[5], out, 0,1,0,1);
	printTile(&tiles[6], out, 0,1,0,1);
	printRows(out);
	printStartRow(out, 2);
	printTile(&tiles[7], out, 1,1,1,1);
	printTile(&tiles[8], out, 0,1,1,1);
	printTile(&tiles[9], out, 0,1,1,1);
	printTile(&tiles[10], out, 0,1,1,1);
	printTile(&tiles[11], out, 0,1,1,1);
	printRows(out);
	printStartRow(out, 3);
	printTile(&tiles[12], out, 1,0,1,1);
	printTile(&tiles[13], out, 0,0,1,1);
	printTile(&tiles[14], out, 0,0,1,1);
	printTile(&tiles[15], out, 0,0,1,1);
	printRows(out);
	printStartRow(out, 4);	
	printTile(&tiles[16], out, 1,0,1,1);
	printTile(&tiles[17], out, 0,0,1,1);
	printTile(&tiles[18], out, 0,0,1,1);
	printRows(out);
	std::cout << std::endl << color(0) << std::endl;
}

/**
 * Prints the correct amount of spaces per row, to pad
 * the left side of the board when printing
 *
 * Spaces are appended to each string in out[]
 */
void printStartRow(std::string (&out)[9], int row) {
	int s = 2;
	if(row != 2)s += 7;
	if(row == 0 || row == 4)s += 7;

	for(int i=0; i < 9; i++) {
		out[i] += std::string(s, ' ');
	}
}

/**
 * Outputs the string array representing a row of tiles
 * once the array has been populated
 *
 * The strings in the out[] are cleared following printing
 */
void printRows(std::string (&out)[9]) {
	for(int s=0; s < 9; s++){
		// Lines with less than 20 characters shouldn't be printed,
		// as they are left empty due to overlap between tiles
		if(out[s].length() > 20)
			std::cout << out[s] << std::endl;
		// Clear string
		out[s].clear();
	}
}

/**
 * Returns the color associated with a road,
 * at t[r], in ANSI escape string format
 */ 
std::string roadColor(Tile *t, int r) {
	return color(t->adjEdges[r]->road);
}

/**
 * Returns the color associated with a settlement,
 * at t[c], in ANSI escape string format
 */ 
std::string settColor(Tile *t, int c) {
	return color(t->adjCorners[c]->settlement);
}


/**
 * Prints the desired segments of a tile to each of 9 strings
 * (one string is used per line, as each tile is 9 lines tall)
 * 
 * Segments are used to ensure that overlapping regions
 * are only printed once per pair of tiles (see printBoard())
 *
 * segments:
 *	 u
 *	l r	
 *   d
 */
void printTile(Tile *t, std::string (&out)[9], bool l, bool u, bool d, bool r) {
	std::string s13(13, ' ');
	std::string s9(9, ' ');
	std::string s6(6, ' ');
	std::string s5(5, ' ');
	std::string s4(4, ' ');
	std::string s3(3, ' ');
	std::string s2(2, ' ');
	std::string s(1, ' ');
	std::string clr = color(-2);

	if(l && u) {
		out[0] += s;
		out[1] += s;
		out[2] += settColor(t,5) + "S" + clr;
	} if(l) {
		std::string cr5 = roadColor(t,5);
		out[3] += cr5 + "|" + clr;
	 	out[4] += cr5 + "|" + clr;
		out[5] += cr5 + "|" + clr;
	} if(l && d) {
		out[6] += settColor(t,4) + "S" + clr;
		out[7] += s;
		out[8] += s;
	}
	if(u) {
		out[0] += s4 + roadColor(t,0) + "_ " + 
			settColor(t,0) + "S" + roadColor(t,1) + " _" + s4 + clr;
		out[1] += s + roadColor(t,0) + "_--" + s5 + roadColor(t,1) + "--_" + s + clr;
		out[2] += s13;
	}

	printTileMiddle(t, out);
	
	if(d) {
		out[6] += s + roadColor(t,4) + "_" + s9 + roadColor(t,3) + "_" + s + clr;
		out[7] += s2 + roadColor(t,4) + "--_" + s3 + roadColor(t,3) + "_--" + s2 + clr;
		out[8] += s6 + settColor(t,3) + "S" + s6 + clr;
	}

	if(r && u) {
		out[0] += s;
		out[1] += s;
		out[2] += settColor(t,1) + "S" + clr;
	} if(r) {
		std::string cr2 = roadColor(t,2);
		out[3] += cr2 + "|" + clr;
		out[4] += cr2 + "|" + clr;
		out[5] += cr2 + "|" + clr;
	} if(r && d) {
		out[6] += settColor(t,2) + "S" + clr;
		out[7] += s;
		out[8] += s;
	}
}

/**
* Returns the ANSI escape code string to print
* the color associated with each player
*/
std::string color(int player) {
	switch(player) {
		case 1: // Red
			return "\033[31;1m";
		case 2: // Blue
			return "\033[36;1m";
		case 3: // White
			return "\033[37;1m";
		case 4: // Orange (yellow)
			return "\033[33;1m";
		case -2: // Magenta (neutral)
			return "\033[0m\033[35m";
		default:
			return "\033[0m";
	}
}

/**
 * Prints ASCII art representing the middle of each tile
 * depending on the tile's resource type
 */
void printTileMiddle(Tile *t, std::string (&out)[9]) {
	std::string clr = color(0);
	std::string red = "\033[31m";
	std::string green = "\033[32m";
	std::string yellow = "\033[33m";
	std::string grey = "\033[30;1m";

	char buff[10];
	sprintf(buff, "%02d", t->num);
	std::string n = buff;


	switch(t->resource) {
		case 0:
			out[3] += red + "___|___|___|_" + clr;
			out[4] += red + "_|___" + n + "__|___" + clr;
			out[5] += red + "___|___|___|_" + clr;
			return;
		case 1:
			out[3] += green + " /\\ /\\    /\\ " + clr;
			out[4] += green + " /\\ /\\ " + n + " /\\ " + clr;
			out[5] += green + " || ||    || " + clr;
			return;
		case 2:
			out[3] += green + "  " + n + "   _--_  " + clr;
			out[4] += green + "  _--_-    -_" + clr;
			out[5] += green + "_-    -_     " + clr;
			return;
		case 3:
			out[3] += yellow + "    \"        " + clr;
			out[4] += yellow + "   \\|/  " + n + "   " + clr;
			out[5] += yellow + "    |        " + clr;
			return;
		case 4:
			out[3] += grey + "  /\\/\\ /\\    " + clr;
			out[4] += grey + " /  \\ /" + n + "\\   " + clr;
			out[5] += grey + "/    /    \\  " + clr;
			return;
		default:
			out[3] += yellow + "   {}      @ " + clr;
			out[4] += yellow + "--_{}  _{}--_" + clr;
			out[5] += yellow + "   {}_- {}   " + clr;
			return;
	}
}
