#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>

#include "../board/Board.h"

using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

class Player {
  private:
    vector<int> resHand;
    vector<int> devHand;
    int numRoads = 15;
    int numSettlements = 5;
    int numCities = 4;
    int color = -1;
    int score = 0;
  public:
    void addResCard();
    void takeResCard();
    void addDevCard();
    void takeDevCard();
    void placeRoad(Edge e);
    void placeSettlement(Corner c);
    void placeCity(Corner c);
    Player () {}
};

#endif
