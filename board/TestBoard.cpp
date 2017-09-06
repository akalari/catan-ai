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
#include "../player/Player.h"
#include "../elements/Elements.h"

using namespace std;

int main() {
    cout << "Testing Board" << endl;
    vector <Board> boards;

    randomBoards(1, boards);

    for(Board &b:boards){
      Player testPlayer = Player(2, "BLUE");
      int bestIndex = testPlayer.bestCornerDP
        (b, testPlayer.calculateWeights(b), 0.25);
      b.buildSettlement(bestIndex, testPlayer.getColor());
      b.printBoard();
    }

    return 0;
}

 /*void placeFirstSettDP(Board &b, int player) {
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
    }*/
