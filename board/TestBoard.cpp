#include <iostream> // std::cout

#include "Board.h"
#include "SetupBoard.h"

int main() {
    std::cout << "Testing Board" << std::endl;
    initBoard();
    randomBoard(TILE_POSSIBILITIES);
    printBoard();
    return 0;
}
