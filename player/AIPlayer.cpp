#include "../board/Board.h"
#include "Player.h"
#include "AIPlayer.h"

#include <string> // std::string
#include <vector> // std::vector
#include <cmath> // std::abs
#include <numeric> // std::accumulate, std::inner_product
#include <iterator> // std::begin

using namespace std;

AIPlayer::AIPlayer(int color, Board &b):
    Player(color, b, AINames[color])
{}

/**
 * Returns the corner index for placing the first settlement`
 */
int AIPlayer::getFirstSett() {
    return 0;
}

/**
 *  Returns the edge index for placing the first road
 */
int AIPlayer::getFirstRoad() {
    return 0;
}

/**
 * Returns the corner index for placing the second settlement
 */
int AIPlayer::getSecondSett() {
    return 0;
}

/**
 * Returns the edge index for placing the second road
 */
int AIPlayer::getSecondRoad() {
    return 0;
}

/**
 * Returns the next move type that the player should take
 */
Move AIPlayer::getNextMove() {
    return BUILD_SETT;
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
 * fills rate with a 2-D array, format:
 * {{giveRes, giveAmt}, {recvRes, RecvAmt}}
 */
void AIPlayer::getTradeRate(int (&rate)[2][2]) {
    rate[0][0] = 0;
    rate[0][1] = 0;
    rate[1][0] = 0;
    rate[1][1] = 0;
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
