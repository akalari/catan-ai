#include <iostream>
#include <algorithm> // std::shuffle
#include <random> // std::default_random_engine

#include "Player.h"
#include "../elements/Elements.h"
#include "../board/Board.h"
#include "../board/SetupBoard.h"

using namespace std;

Board board = Board();
auto corners = board.getCorners();
auto edges = board.getEdges();
auto tiles = board.getTiles();
auto ports = board.getPorts();

Player::Player (int color, Board b):
    numRoads(15), numSettlements(5), numCities(4), score(0),
    this->color(color), board(b), name("unnamed"),
    resHand {0, 0, 0, 0, 0}
{
    name = inputName();
}

bool Player::moveBuildSettlement() {
    int c = getMoveSettlement();
    if(!board.canPlaceSettlement(c, color, true))
        return false;
    board.buildSettlement(c, color);
    return true;
}

bool Player::moveBuildRoad() {
    int edge = getMoveRoad();
    if(!board.canPlaceRoad(edge, color))
        return false;
    board.buildRoad(edge, color);
    return true;
}

bool Player::moveBuildCity() {
    int c = getMoveCity();
    if(!board.canPlaceCity(c, color))
        return false;
    board.buildCity(c, color);
    return true;
}

bool moveBuyDev() {
    return false;
}

bool movePlayDev() {
    int dev = getMoveDev();
    return false;
}

bool moveOfferTrade() {
    int rate[2][2] = getTradeRate();
    return false;
}

bool moveUsePort() {
    int rate[2][2] = getTradeRate();
    return false;
}

// Player-player interactions
/**
 * Offers a trade to this Player
 * returns whether this player accepts or declines
 */
bool offerTrade(int tradeRate[2][2]) {
    return false;
}

/**
 * Steal a resource from this Player
 * returns the resource stolen
 */
int stealResource() {
    vector<int> tempRes;
    for(int res:resHand){
        for(int i = 0; i < resHand[res]; i++)
            tempRes.push_back(res);
    }
    random_device rng;
    mt19937 urng(rng());
    shuffle(tc.begin(), tc.end(), urng);

    int res = tempRes.back();
    resHand[res]--;
    return res;
}

/**
 * Steal all of a certain resource from this player
 * Returns the number of the resource stolen
 */
int stealMonopoly(int res) {
    int count = resHand[res];
    resHand[res] = 0;
    return count;
}

/**
 * [Setup]
 * Build first pair of sett/road
 */
void placeFirstPair() {
    int c, r;

    do c = getFirstSett();
    while(!board.canPlaceSettlement(c, color, false));
    do r = getFirstRoad();
    while(!board.canPlaceRoad(r, color));

    board.buildSettlement(c, color);
    board.buildRoad(r, color);
}

/**
 * [Setup]
 * Build second pair of sett/road
 */
void placeSecondPair() {
    int c, r;

    do c = getSecondSett();
    while(!board.canPlaceSettlement(c, color, false));
    do r = getSecondRoad();
    while(!board.canPlaceRoad(r, color));

    board.buildSettlement(c, color);
    board.buildRoad(r, color);
}

// Turn

/*
 * Collect resources on die roll
 */
void collectFromRoll(int roll) {
    for(int t:board.getMatchingTiles(roll)) {
        int res = board.getTiles()[t].getResource();
        // get all corners adjacent to the matching tiles
        for(int c:board.getTiles()[t].getAdjCorners()) {
            Corner &corn = board.getCorners()[c];
            if(corn.getSettlement() == color) {
                resHand[res]++;
                if(corn.getCity()) resHand[res]++;
            }
        }
    }
}

void startTurn() {}

void moveRobber() {
    int robTile = getRobberMove();
    board.moveRobber(robTile);
}

// Misc.
/* Return # of Victory Points */
int getScore() { return score; }
string getName() { return name; }
int getColor() { return color; }
