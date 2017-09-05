#include <iostream> // std::cout
#include <ctime>
#include <algorithm>

#include "Board.h"
#include "BoardUtils.h"
#include "SetupBoard.h"
#include "TestBoard.h"

using namespace std;


int main() {
    cout << "Testing Board" << endl;
    vector <Board> boards;

    randomBoards(2, boards);
    writeBoards("twoboards.csv", boards);
    loadBoards("twoboards.csv", boards);
    return 0;
}

void firstSett(Board b) {
    double resourceWeights[] = {1,1,0.4,0.6,0.7}; // Brick Lumber Wool Grain ore
    double probWeights = 0.25; // 0: only probability, 0.5: similarity to weights

    int deck[] = {0,0,0,0,0};

    srand(time(0));
    int tile = rand() % 19;
    int corner = rand() % 6;

    int bestC = 0;
    double bestScore = 0;

    double ssW = 0;
    for(double d:resourceWeights)ssW += pow(d,2);

    for(int c = 0; c < NUM_CORNERS; c++) {
        double score = 0;
        double scores[] = {0,0,0,0,0};

        for(int t:b.getCorners()[c].getAdjTiles()){
           Tile &tile = b.getTiles()[t];
           double probability = (6 - abs(tile.getNum() - 7))/36.0;
           scores[tile.getResource()] += probability;
        }
        
        double ssS = 0; //sum of squares
        for(double d:scores)ssS += pow(d,2);
        score = inner_product(scores, scores + 5, resourceWeights, 0.0)/
            (pow(ssS*ssW, probWeights));
        
        if(score > bestScore) {
            bestC = c;
            bestScore = score;
        }
    }

    b.getCorners()[bestC].setSettlement(1);

    cout << "Best score: " << bestScore << endl;

    for(int i = 0; i < 100; i++) {
        int roll = (rand() % 6 + 1) + (rand() % 6 + 1);
        std::vector <int> matches = b.getMatchingTiles(roll);
        for(int t:matches) {
            for(int c:b.getTiles()[t].getAdjCorners()) {
                if(b.getCorners()[c].getSettlement() == 1){
                   deck[b.getTiles()[t].getResource()]++;
                }
            }
        }
    }

    double realScore = 0;
    for(int i = 0; i < 5; i++) {
        realScore += deck[i];
        cout << deck[i] << " ";
    }
    realScore /= 100.0;
    cout << "| Real Score: " << realScore << endl;
}
