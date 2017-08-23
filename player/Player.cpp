#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>

#include "Player.h"
#include "../board/Board.h"

using namespace std;

Player::Player (int c) {
  color = c;
}

void addResCard(int e) {
  resHand.push_back(e);
}

void takeResCard(int e) {
  auto loc = find(resHand.begin(), resHand.end(), e);
  if (loc != resHand.end()) resHand.erase(loc);
}

void addDevCard(int e) {
  devHand.push_back(e);
}

void takeDevCard(int e) {
  auto loc = find(devHand.begin(), devHand.end(), e);
  if (loc != devHand.end()) devHand.erase(loc);
}

void placeRoad(Edge e) {
  e.road = color;
  road--;
}

void placeSettlement(Corner c) {
  c.settlement = color;
  settlement--;
  score++;
}

void placeCity(Corner c) {
  if (c.settlement == color) {
    c.city = color;
  }
  c.settlement = -1;
  settlement++;
  city--;
  score++;
}
