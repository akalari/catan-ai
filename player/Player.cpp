#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

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

int main () {return 0;}


Player::Player (int c, string string_color) {
  color = c;
  numRoads = 15;
  numSettlements = 5;
  numCities = 4;
  score = 0;
  color_string = color;
  switch (color) {
    case RED:
      color_string = "RED";
      cout << "My color is RED. What is my name? ";
      cin >> name;
      break;
    case BLUE:
      color_string = "BLUE";
      cout << "My color is BLUE. What is my name? ";
      cin >> name;
      break;
    case WHITE:
      color_string = "WHITE";
      cout << "My color is WHITE. What is my name? ";
      cin >> name;
      break;
    case ORANGE:
      color_string = "ORANGE";
      cout << "My color is ORANGE. What is my name? ";
      cin >> name;
  }
}

void Player::addResCard(int e) {
  resHand.push_back(e);
}

bool Player::takeResCard(int e) {
  auto loc = find(resHand.begin(), resHand.end(), e);
  if (loc != resHand.end()) {
    resHand.erase(loc);
    return true;
  }
  return false;
}

void Player::addDevCard(int e) {
  devHand.push_back(e);
}

bool Player::takeDevCard(int e) {
  auto loc = find(devHand.begin(), devHand.end(), e);
  if (loc != devHand.end()) {
    devHand.erase(loc);
    return true;
  }
  return false;
}

void Player::placeSettlement(int c, bool isFirstSettlement) {
  if (!board.isTwoAway(c) || corners[c].getSettlement() != NULL) {
    cout << "You cannot place a settlement here." << endl;
    return;
  }
  if (!isFirstSettlement && !board.adjOwnRoad(c, color)) {
    cout << "You cannot place a settlement here." << endl;
    return;
  }
  corners[c].setSettlement(color);
  numSettlements--;
  score++;
}

void Player::inputSettlement(bool isFirstSettlement) {
  int t = -1;
  int compass = -1;
  do {
    cout << "Enter the tile number to place a settlement: ";
    cin >> t;
    cout << endl << "Enter the corner of the settlement: ";
    cin >> compass;
    placeSettlement(tiles[t].getAdjCorners()[compass], isFirstSettlement);
  }while(corners[tiles[t].getAdjCorners()[compass]].getSettlement() != color);
}

void Player::placeCity(int c) {
  if (corners[c].getSettlement() != color) {
    cout << "You cannot place a city here." << endl;
    return;
  }
  corners[c].setCity(color);
  numSettlements++;
  numCities--;
  score++;
}

void Player::inputCity() {
  int t = -1;
  int c = -1;
  do {
    cout << "Enter the tile number to place a city: ";
    cin >> t;
    cout << endl << "Enter the corner of the city: ";
    cin >> c;
    placeCity(tiles[t].getAdjCorners()[c]);
  } while(corners[tiles[t].getAdjCorners()[c]].getCity() != color);
}

void Player::placeRoad(int e) {
  if (!board.adjOwnProperty(e, color)) {
    cout << "You cannot place a road here." << endl;
    return;
  }
  edges[e].setRoad(color);
  numRoads--;
}

void Player::inputRoad() {
  int e = -1;
  do {
    cout << "Enter the edge number to place a road: ";
    cin >> e;
    placeRoad(e);
  } while(edges[e].getRoad() != color);
}

string Player::toString() {
  return name+"("+color_string+")";
}

int Player::getScore() {
  return score;
}
