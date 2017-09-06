#include <iostream>

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
    this->color(color), board(b)
{
    getName();
}

void Player::getName() {
    cout << "My color is " << COLOR_STRINGS[color] << ". What is my name? ";
    cin >> name;
}


// OLD
/*
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
  if (!board.canPlaceSettlement(c, color, !isFirstSettlement)) {
    cout << "You cannot place a settlement here." << endl;
    return;
  }
  board.buildSettlement(c, color);
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

vector<int> &Player::getSettlements() {
  return settlements;
}

int Player::getColor() {
  return color;
}

 */
