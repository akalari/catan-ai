#include <iostream> // std::cout
#include <ctime>
#include <algorithm>
#include <utility>

#include "Board.h"
#include "BoardUtils.h"
#include "SetupBoard.h"
#include "TestBoard.h"

using namespace std;

int main() {
    cout << "Testing Board" << endl;
    vector <Board> boards;

    randomBoards(2, boards);

    placeFirstSettDP(boards.front(), 2);

    return 0;
}

int bestCornerDP(Board b) {
    double resourceWeights[] = {1,1,0.4,0.6,0.7}; // Brick Lumber Wool Grain ore
    double probWeights = 0.25; // 0: only probability, 0.5: similarity to weights

    pair<double, int> highScore;

    // sum of squares of weights
    double ssW = accumulate(begin(resourceWeights), end(resourceWeights),
            0.0, [](double a, double b){ return a = b*b; });

    for(int c = 0; c < NUM_CORNERS; c++) {
        if(!b.canPlaceSetttlement(c, -1, false))continue; // Make sure we can place a settlement here

        double score = 0;
        double probs[5] = {0,0,0,0,0};

        for(int t:b.getCorners()[c].getAdjTiles()){
            Tile &tile = b.getTiles()[t];
            double probability = (6 - abs(tile.getNum() - 7))/36.0;
            probs[tile.getResource()] += probability;
        }

        //sum of squares of probs
        double ssP = accumulate(begin(probs), end(probs),
                0.0, [](double a, double b){ return a + b*b; });
        if(ssP == 0)continue; //if we have a score of 0

        score = inner_product(begin(probs), end(probs), resourceWeights, 0.0)/
                    (pow(ssW*ssP, probWeights));

        if(highScore < make_pair(score, c))
            highScore = make_pair(score, c);
    }

    return highScore.second;
}

void placeFirstSettDP(Board b, int player) {
    int bestCorner = bestCornerDP(b);
    b.getCorners()[bestCorner].setSettlement(player);
}
