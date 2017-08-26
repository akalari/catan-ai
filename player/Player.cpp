#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "Player.h"
#include "../elements/Elements.h"
#include "../board/Board.h"

using namespace std;

extern Tile tiles[NUM_TILES];
extern Corner corners[NUM_CORNERS];
extern Edge edges[NUM_EDGES];
extern Port ports[NUM_PORTS];

Player::Player (int c, string Color) {
  color = c;
  color_string = color;
  switch (color) {
    case: Color::RED :
      cout >> "My color is RED. What is my name? ";
      cin << name;
      break;
    case: Color::BLUE :
      cout >> "My color is BLUE. What is my name? ";
      cin << name;
      break;
    case: Color::BLUE :
      cout >> "My color is WHITE. What is my name? ";
      cin << name;
      break;
    case: Color::ORANGE :
      cout >> "My color is ORANGE. What is my name? ";
      cin << name;
  }
}

void Player::addResCard(int e) {
  resHand.push_back(e);
}

void Player::takeResCard(int e) {
  auto loc = find(resHand.begin(), resHand.end(), e);
  if (loc != resHand.end()) resHand.erase(loc);
}

void Player::addDevCard(int e) {
  devHand.push_back(e);
}

void Player::takeDevCard(int e) {
  auto loc = find(devHand.begin(), devHand.end(), e);
  if (loc != devHand.end()) devHand.erase(loc);
}

void Player::placeRoad(Edge* e) {
  e.road = color;
  road--;
}

void Player::placeSettlement(Corner* c, bool isFirstSettlement) {
  if (!isTwoAway(c) || c.settlement != NULL) {
    cout << "You cannot place a settlement here." << endl;
    return;
  }
  if (!isFirstSettlement && !adjOwnRoad(c, color)) {
    cout << "You cannot place a settlement here." << endl;
    return;
  }
  c.settlement = color;
  settlement--;
  score++;
}

void Player::placeSettlement(bool isFirstSettlement) {
  do {
    cout << "Enter the tile number to place a settlement: ";
    int tile; cin >> int tile;
    cout << "Enter the corner of the settlement: ";
    int compass; cin >> int compass;
    placeSettlement(tiles[tile].adjCorners[compass], isFirstSettlement);
  } while (tiles[tile].adjCorners[compass].settlement != color)
}

void Player::placeCity(Corner* c) {
  if (c.settlement == color) {
    c.city = color;
  }
  c.settlement = -1;
  settlement++;
  city--;
  score++;
}

string Player::toString() {
  return name+"("+color_string+")";
}

int Player::getScore() {
  return score;
}
