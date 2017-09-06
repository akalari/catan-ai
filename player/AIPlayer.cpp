#include "../board/Board.h"
#include "Player.h"

#include <vector> // std::vector
#include <cmath> // std::abs
#include <numeric> // std::accumulate, std::inner_product

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
int Player::bestCornerDP(Board &board, vector<double> resWeights, double probWeights) {

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
