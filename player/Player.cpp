#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>

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

/**
 * Determines the best corner to build at
 * probWeights: a score between 0-0.5
 * resourceWeights: order: {Brick, Lumber, Wool, Grain, Ore}
 */
int Player::bestCornerDP(Board &board, vector<double> resWeights, double probWeights) {

    pair<double, int> highScore;
    // Sum of Squares of Weights
    double ssW = accumulate(begin(resWeights), end(resWeights),
            0.0, [](double a, double b){ return a = b*b; });

    for(int c = 0; c < NUM_CORNERS; c++) {
        if(!board.canPlaceSettlement(c, -1, false)) continue;
        // Make sure we can place a settlement here

        double score = 0;
        double probs[5] = {0,0,0,0,0};

        for(int t : board.getCorners()[c].getAdjTiles()) {
            Tile &tile = board.getTiles()[t];
            double probability = (6 - abs(tile.getNum() - 7))/36.0;
            probs[tile.getResource()] += probability;
        }

        // Sum of squares of probs
        double ssP = accumulate(begin(probs), end(probs),
                0.0, [](double a, double b){ return a + b*b; });
        if(ssP == 0) continue;

        score = inner_product
          (begin(probs), end(probs), begin(resWeights), 0.0)
          /pow(ssW*ssP, probWeights);

        if(highScore < make_pair(score, c))
            highScore = make_pair(score, c);
    }

    return highScore.second;
}

vector<double> Player::calculateWeights(Board &board) {

  vector<double> probWeights = {0, 0, 0, 0, 0};
  vector<double> numResTiles = {0, 0, 0, 0, 0};
  vector<double> stdWeights = {1, 0.9, 0.4, 0.6, 0.8};
  // Get the average distance from 7 for each resource
  for (Tile &t : board.getTiles()) {
    probWeights[t.getResource()] += abs(7-t.getNum());
    numResTiles[t.getResource()]++;
  }
  // Convert the resource weight to a value lower than 1
  for (int i = 0; i < probWeights.size(); i++) {
    probWeights[i] = (0.2*(probWeights[i]/numResTiles[i]));
    probWeights[i] *= stdWeights[i];
  }

  return probWeights;
}
