#include "../board/Board.h"
#include "Player.h"

#include <vector> // std::vector
#include <cmath> // std::abs
#include <numeric> // std::accumulate, std::inner_product

using namespace std;

vector<double> AIPlayer::calculateWeights() {

  vector<double> probWeights = {0, 0, 0, 0, 0};
  vector<double> numResTiles = {0, 0, 0, 0, 0};
  vector<double> stdWeights = {1, 0.9, 0.4, 0.6, 0.8};
  vector<double> existingWeights = {1, 1, 1, 1, 1}

  // Get the average distance from 7 for each resource
  for (Tile &t : board.getTiles()) {
    probWeights[t.getResource()] += abs(7-t.getNum());
    numResTiles[t.getResource()]++;
  }
  // Modify the weights given existing resources
  for (int sett : settlements) {
    for (int tile : b.getSettlements[sett].getAdjTiles()) {
      Tile& t = b.getTiles()[tile];
      existingWeights[t.getResource()] *= (7-t.getNum())/10.0;
    }
  }
  // Convert the resource weight to a value lower than 1
  for (int i = 0; i < probWeights.size(); i++) {
    probWeights[i] = (0.2*(probWeights[i]/numResTiles[i]));
    probWeights[i] *= stdWeights[i] * existingWeights[i];
  }

  return probWeights;
}

/**
 * Determines the best corner to build at
 * probWeights: a score between 0-0.5
 * resourceWeights: order: {Brick, Lumber, Wool, Grain, Ore}
 */
int AIPlayer::bestCornerDP
  (Board &board, vector<double> resWeights, double probWeights) {

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

int AIPlayer::chooseBestRoad(int corn, bool portWanted) {

  if (portWanted && pathToPort.size() < 4) {
    vector<int> pathToPort = getPathToPort(UNDETERMINED, corn);
    return pathToPort[pathToPort.size()-1];
  }
  else {
    int targetSettement = chooseProximateSettlement(DISTANCE, );
    
  }

}

// Use a breadth first search to find the path to the port
vector<int> AIPlayer::getPathToPort(int resource, int corn) {

  // Vector of pairs of distance and predecessor
  vector<int> search;
  vector<int> path;
  int predecessors[NUM_CORNERS] = {-1};
  int port = -1;

  searchCorners.push_back(corner.getIndex());
  do {
    for (int c : search) {
      for (int adjC : b.getCorners()[c].getAdjEdges()) {
        visited[c] = true;
        // NEEDS ROAD VALIDITY CHECK
        search.erase(remove(search.begin(), search.end(), c), search.end());
        if (adjC.getPort < 0) {
          search.push_back(adjC);
          predecessors[adjC] = c;
        }
        else {
          return predecessorGetsPath(adjC, predecessors, path);
        }
      }
    }
  } while(!cornersToSearch.empty() && port != resource);
}

vector<int> predecessorGetsPath
         (int endPath, int[NUM_CORNERS] preds, vector<int> path) {

  return predecessorGetsPath(preds[endPath], preds, path.push_back(endPath));
}

void Player::placeFirstSettlement() {
  buildSettlement(bestCornerDP(b, calculateWeights(b), 0.25), color);
}
