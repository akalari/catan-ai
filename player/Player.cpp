#include <iostream>
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

MoveState Player::moveDoMove() {
    Move m = getNextMove();
    bool b = false;
    switch (m) {
    case BUILD_SETT: b = moveBuildSettlement();
        break;
    case BUILD_ROAD: b = moveBuildRoad();
        break;
    case BUILD_CITY: b = moveBuildCity();
        break;
    case BUY_DEV: b = moveBuyDev();
        break;
    case PLAY_DEV: b = movePlayDev();
        break;
    case OFFER_TRADE: b = moveOfferTrade();
        break;
    case USE_PORT: b = moveUsePort();
        break;
    case END_TURN: return MOVE_ENDTURN;
    }
    return b ? MOVE_SUCCESS : MOVE_UNSUCCESSFUL;
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

bool Player::moveBuyDev() {
    return false;
}

bool Player::movePlayDev() {
    int dev = getMoveDev();
    return false;
}

bool Player::moveOfferTrade() {
    int rate[2][2];
    getTradeRate(rate);
    return false;
}

bool Player::moveUsePort() {
    int rate[2][2];
    getTradeRate(rate);
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

    do r = getSecondRoad();
    while(!board.canPlaceRoad(r, color));
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

void Player::startTurn() {
    MoveState state = MOVE_SUCCESS;
    while(state != MOVE_ENDTURN) {
        state = moveDoMove();
        if(state == MOVE_UNSUCCESSFUL)
            cout << "Move Unsuccessful" << endl;
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
