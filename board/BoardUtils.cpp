#include <cstddef>
#include <stdio.h>

#include "Board.h"

extern Tile tiles[NUM_TILES];
extern Corner corners[NUM_CORNERS];

Corner* getOtherCorner(Edge* e, Corner* c);
bool isTwoAway(Corner* settlement);

/**
 * Checks if a settlement (or settlement spot)
 * is >= 2 spots away from other settlements
 */
bool isTwoAway(Corner* settlement) {

	// Check all adjacent edges
	for (Edge *e : settlement->adjEdges) {
		Corner* c = getOtherCorner(e, settlement);
		// If there is a settlement, return false
		if (c->settlement > -1) return false;
	}
	return true;
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
 * Returns the list of occupied settlements adjacent
 * to a tile with a specified number
 */ 
std::vector<Corner*> getSettlements(int number) {
	std::vector<Corner*> matches;
	for (Tile t : tiles) {
		if (t.num != number) continue;
		for (Corner* c : t->adjCorners) {
			if (c->settlement > -1) matches.push_back(c);
		}
	}
	return matches;
}

/**
 * Returns a list of all of the ports owned by a player
 */
std::vector<Port*> portsOwned(int player) {
	std::vector<Port*> ports;
	for(Corner c:corners) {
		if(c->settlement == player &&
				c->adjPort != 0)
			ports.push_back(c->adjPort);
	}
	return ports;
}

/**
 * Checks if a new settlement is adjacent to a road
 * occupied by the player
boolean adjOwnRoad(Corner* settlement, int player) {
	for (Edge* e : settlement->adjEdges) {
		if(e->road != -1 && e-> road == player) return true;
	}
	return false;
}

