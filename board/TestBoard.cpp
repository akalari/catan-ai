#include <iostream> // std::cout
#include <ctime>
#include <algorithm>
#include <utility>
#include <cmath>
#include <numeric>

#include "Board.h"
#include "BoardUtils.h"
#include "SetupBoard.h"
#include "TestBoard.h"

using namespace std;

int main() {
    cout << "Testing Board" << endl;
    vector <Board> boards;

    randomBoards(5, boards);

    for(Board &b:boards){
        placeFirstSettDP(b, 1);
        placeSecondSettDP(b, 1);
        b.printBoard();
    }

    return 0;
}

/**
 * Determines the best corner to build at
 * probWeights: a score between 0-0.5
 *
 * resourceWeights: order: {Brick, Lumber, Wool, Grain, Ore}
 */
int bestCornerDP(Board &board, double (&resWeights)[5], double probWeights) {

    pair<double, int> highScore;
    // Sum of Squares of Weights
    double ssW = accumulate(begin(resWeights), end(resWeights),
            0.0, [](double a, double b){ return a = b*b; });

    for(int c = 0; c < NUM_CORNERS; c++) {
        if(!board.canPlaceSetttlement(c, -1, false)) continue;
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
        if(ssP == 0)
          continue;

        score = inner_product
          (begin(probs), end(probs), resWeights, 0.0)/
          (pow(ssW*ssP, probWeights));

        if(highScore < make_pair(score, c))
            highScore = make_pair(score, c);
    }

    return highScore.second;
}

void placeFirstSettDP(Board &b, int player) {
    double resourceWeights[5] = {1,0.9,0.4,0.6,0.8};
    int bestCorner = bestCornerDP(b, resourceWeights, 0.25);
    b.buildSettlement(bestCorner, player);
    int e = b.getCorners()[bestCorner].getAdjEdges().front();
    b.buildRoad(e, player);
}

void placeSecondSettDP(Board &b, int player) {
    double resourceWeights[5] = {0.4, 0.4, 0.9, 0.9, 0.9};
    int bestCorner = bestCornerDP(b, resourceWeights, 0.25);
    b.buildSettlement(bestCorner, player);
    int e = b.getCorners()[bestCorner].getAdjEdges().front();
    b.buildRoad(e, player);
}
