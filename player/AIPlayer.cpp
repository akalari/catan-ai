#include "../board/Board.h"
#include "Player.h"
#include "AIPlayer.h"

#include <string> // std::string
#include <vector> // std::vector
#include <cmath> // std::abs
#include <numeric> // std::accumulate, std::inner_product
#include <iterator> // std::begin

using namespace std;

vector<PairedMove> AIPlayer::getPossibleMoves() {
    vector<PairedMove> moves;
    for(int c:possibleCities())moves.push_back(PairedMove(BUILD_CITY, c));
    for(int s:possibleSettlements())moves.push_back(PairedMove(BUILD_SETT, s));
    for(int r:possibleRoads())moves.push_back(PairedMove(BUILD_ROAD, r));
    moves.push_back(PairedMove(END_TURN, 0));
    return moves;
}

vector<int> AIPlayer::possibleCities() {
    vector<int> cities;
    if(resHand[ORE] < 3 || resHand[GRAIN] < 2 || numCities < 1) return cities;
    for(int c = 0; c < NUM_CORNERS; c++) {
        if(board.getCorners()[c].getSettlement() == color && !board.getCorners()[c].getCity())
            cities.push_back(c);
    }
    return cities;
}

vector<int> AIPlayer::possibleSettlements() {
    vector<int> setts;
    if(resHand[BRICK] < 1 || resHand[LUMBER] < 1 || resHand[GRAIN] < 1 || resHand[WOOL] < 1 || numSettlements < 1) return setts;
    for(Edge &e:board.getEdges()) {
        if(e.getRoad() == color) {
            for(int c:e.getAdjCorners()) {
                if(board.canPlaceSettlement(c, color, true)) setts.push_back(c);
            }
        }
    }
    return setts;
}

vector<int> AIPlayer::possibleRoads() {
    vector<int> roads;
    if(resHand[BRICK] < 1 || resHand[LUMBER] < 1 || numRoads < 1) return roads;
    for(int e = 0; e < NUM_EDGES; e++) {
        if(board.getEdges()[e].getRoad() == Edge::NONE) {
            if(board.canPlaceRoad(e, color)) roads.push_back(e);
        }
    }
    return roads;
}

AIPlayer::AIPlayer(int color, Board &b):
    Player(color, b, AINames[color])
{}

/**
 * Returns the corner index for placing the first settlement`
 */
int AIPlayer::getFirstSett() {
    return rand() % NUM_CORNERS;
}

/**
 *  Returns the edge index for placing the first road
 */
int AIPlayer::getFirstRoad() {
    return rand() % NUM_EDGES;
}

/**
 * Returns the corner index for placing the second settlement
 */
int AIPlayer::getSecondSett() {
    return rand() % NUM_CORNERS;
}

/**
 * Returns the edge index for placing the second road
 */
int AIPlayer::getSecondRoad() {
    return rand() % NUM_EDGES;
}

/**
 * Returns the next move type that the player should take
 */
PairedMove AIPlayer::getNextMove() {
    vector<PairedMove> possible = getPossibleMoves();
    return possible.at(rand() % possible.size());
}

/**
 * Returns the location of the settlement to build
 */
int AIPlayer::getMoveSettlement() {
    return 0;
}

/**
 * Returns the location of the road to build
 */
int AIPlayer::getMoveRoad() {
    return 0;
}

/**
 * Returns the location of the settlement to upgrade to a city
 */
int AIPlayer::getMoveCity() {
    return 0;
}

/**
 * Returns the rate to trade with either the port,
 * bank, or another player
 */
int AIPlayer::getTradeRate() {
    return 0;
}

/**
 * Returns the dev card to play
 */
int AIPlayer::getMoveDev() {
    return 0;
}

/**
 * Returns the location (tile) to move the robber
 */
int AIPlayer::getRobberMove() {
    return 0;
}

/**
 * Returns half of the deck to discard,
 * if the player has more than 7 cards
 */
vector<int> AIPlayer::robberDiscardCards() {
    return vector<int>();
}

vector<double> AIPlayer::calculateWeights(Board &board) {

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

/**
 * Determines the best corner to build at
 * probWeights: a score between 0-0.5
 * resourceWeights: order: {Brick, Lumber, Wool, Grain, Ore}
 */
int AIPlayer::bestCornerDP(Board &board, vector<double> resWeights, double probWeights) {

    pair<double, int> highScore;
    // Sum of Squares of Weights
    double ssW = accumulate(resWeights.begin(), resWeights.end(),
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
