#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // std::shuffle
#include <random> // std::default_random_engine

#include "Player.h"
#include "../elements/Elements.h"
#include "../board/Board.h"

using namespace std;

Player::Player (int color, Board &b, string name):
    numRoads(15), numSettlements(5), numCities(4), score(0),
    color(color), board(b), name(name),
    resHand {0, 0, 0, 0, 0}
{}

PairedMove Player::moveDoMove() {
    PairedMove m = getNextMove();
    return moveDoMove(m);
}

PairedMove Player::moveDoMove(PairedMove m) {
    bool b = false;
    switch (m.move) {
    case BUILD_SETT: b = moveBuildSettlement(m.parameter);
        break;
    case BUILD_ROAD: b = moveBuildRoad(m.parameter);
        break;
    case BUILD_CITY: b = moveBuildCity(m.parameter);
        break;
    case BUY_DEV: b = moveBuyDev();
        break;
    case PLAY_DEV: b = movePlayDev(m.parameter);
        break;
    case OFFER_TRADE: b = moveOfferTrade(m.parameter);
        break;
    case USE_PORT: b = moveUsePort(m.parameter);
        break;
    case END_TURN: b = true;
    }
    m.state = b ? MOVE_SUCCESS : MOVE_UNSUCCESSFUL;
    return m;
}

bool Player::moveBuildSettlement(int c) {
    if(!board.canPlaceSettlement(c, color, true) ||
        resHand[BRICK] < 1 || resHand[LUMBER] < 1 || resHand[GRAIN] < 1 || resHand[WOOL] < 1 || numSettlements < 1)
        return false;
    resHand[BRICK]--;
    resHand[LUMBER]--;
    resHand[GRAIN]--;
    resHand[WOOL]--;
    numSettlements--;
    board.buildSettlement(c, color);
    return true;
}

bool Player::moveBuildRoad(int edge) {
    if(!board.canPlaceRoad(edge, color) ||
        resHand[BRICK] < 1 || resHand[LUMBER] < 1 || numRoads < 1)
        return false;
    resHand[BRICK]--;
    resHand[LUMBER]--;
    numRoads--;
    board.buildRoad(edge, color);
    return true;
}

bool Player::moveBuildCity(int c) {
    if(!board.canPlaceCity(c, color) ||
        resHand[ORE] < 3 || resHand[GRAIN] < 2 || numCities < 1)
        return false;
    resHand[ORE] -= 3;
    resHand[GRAIN] -= 2;
    numCities--;
    numSettlements++;
    board.buildCity(c, color);
    return true;
}

bool Player::moveBuyDev() {
    return false;
}

bool Player::movePlayDev(int dev) {
    return false;
}

bool Player::moveOfferTrade(int rate) {
    return false;
}

bool Player::moveUsePort(int rate) {
    return false;
}

// Player-player interactions
/**
 * Offers a trade to this Player
 * returns whether this player accepts or declines
 */
bool Player::offerTrade(int tradeRate[2][2]) {
    return false;
}

/**
 * Steal a resource from this Player
 * returns the resource stolen
 */
int Player::stealResource() {
    vector<int> tempRes;
    for(int res:resHand){
        for(int i = 0; i < resHand[res]; i++)
            tempRes.push_back(res);
    }
    random_device rng;
    mt19937 urng(rng());
    shuffle(tempRes.begin(), tempRes.end(), urng);

    int res = tempRes.back();
    resHand[res]--;
    return res;
}

/**
 * Steal all of a certain resource from this player
 * Returns the number of the resource stolen
 */
int Player::stealMonopoly(int res) {
    int count = resHand[res];
    resHand[res] = 0;
    return count;
}

/**
 * [Setup]
 * Build first pair of sett/road
 */
void Player::placeFirstPair() {
    int c, r;

    do c = getFirstSett();
    while(!board.canPlaceSettlement(c, color, false));
    board.buildSettlement(c, color);

    do r = getFirstRoad();
    while(!board.canPlaceRoad(r, color));
    board.buildRoad(r, color);
}

/**
 * [Setup]
 * Build second pair of sett/road
 */
void Player::placeSecondPair() {
    int c, r;

    do c = getSecondSett();
    while(!board.canPlaceSettlement(c, color, false));
    board.buildSettlement(c, color);

    vector<int> adjEdges = board.getCorners()[c].getAdjEdges();

    do
        r = getSecondRoad();
    while(!board.canPlaceRoad(r, color) ||
        (std::find(adjEdges.begin(), adjEdges.end(), r) == adjEdges.end()));

    board.buildRoad(r, color);
}

// Turn

/*
 * Collect resources on die roll
 */
void Player::collectFromRoll(int roll) {
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

void Player::moveRobber() {
    int robTile = getRobberMove();
    board.moveRobber(robTile);
}

// Misc.
/* Return # of Victory Points */
int Player::getScore() { return score; }
string Player::getName() { return name; }
int Player::getColor() { return color; }
int (&Player::getResHand())[5] { return resHand; }

/**
 * Writes a vector of boards to a CSV file at the specified filename
 */
void Player::writePlayer(string filename) {
    ofstream outfile;
    outfile.open(filename, ofstream::out | ofstream::app); // open to append

    outfile << color << ":";
    outfile << resHand[0];
    for(int i = 1; i < 5; i++)outfile << "," << resHand[i];
    outfile << endl;

    outfile.close();
}

/**
 * Writes a vector of boards to a CSV file at the specified filename
 */
void Player::writePlayer(ofstream &outfile) {
    outfile << color << ":";
    outfile << resHand[0];
    for(int i = 1; i < 5; i++)outfile << "," << resHand[i];
    outfile << endl;
}
