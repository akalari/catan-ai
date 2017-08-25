#include <cstddef>
#include <stdio.h>

#include "Board.h"

extern Tile tiles[NUM_TILES];

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

