#include <iostream> // std::cout

#include "Board.h"
#include "SetupBoard.h"

using namespace std;

int main() {
    cout << "Testing Board" << endl;
    Board board = Board();
    randomBoard(board, TILE_POSSIBILITIES);
    board.printBoard();
    return 0;
}
