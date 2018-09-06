#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>

#include "../board/Board.h"
#include "../board/SetupBoard.h"
#include "../board/BoardUtils.h"

#include "Player.h"
#include "UserPlayer.h"
#include "AIPlayer.h"

using namespace std;

const string OUTFILE_DIR = "currentTrial";
const string BOARD_FILE = OUTFILE_DIR + "/" +"boardfile.csv";
const string PLAYER_FILE = OUTFILE_DIR + "/" +"playerfile.csv";
const string OUT_FILE = OUTFILE_DIR + "/" + "moves_" + to_string(time(NULL)) + ".txt";

const int NUM_TURNS = 100;
int movesMade[4][4] = {0};
int dieRolls[11] = {0};
int turns = 0;

random_device generator;
mt19937 gen(generator());
uniform_int_distribution<int> die(1, 6);

void writeState(string filename, Player* (&gamePlayers)[4], Board &b);
void playGame(int curIndex, Player* (&gamePlayers)[4], Board& b, int turns);
void setupGame(Player* (&gamePlayers)[4], Board& b);
string printColor(int player);
void printData(Player* (&gamePlayers)[4], Board& b);
void loadFromConfig(string filename, Player* (&gamePlayers)[4], Board &b);
double testMove(PairedMove move, int player);
void scoreGame(double (&s)[4], Board &b);

int main() {
    cout << "Testing Player" << endl;
    srand(generator());

    Board b;
    vector<Board> boards;
    readBoards(BOARD_FILE, boards);
    b = boards.at(0);

    Player* gamePlayers[4];
    setupGame(gamePlayers, b);

    playGame(0, gamePlayers, b, 20);

    cout << "Base State:" << endl;
    printData(gamePlayers, b);

    writeState(PLAYER_FILE, gamePlayers, b);
    vector<PairedMove> moves = gamePlayers[0]->getPossibleMoves();

    vector<double> scores;

    for(PairedMove m:moves){
        scores.push_back(testMove(m, 0));
    }

    for(double d:scores)cout << d << " ";
    cout << endl;

    return 0;
}

double testMove(PairedMove move, int player) {
    Player* gamePlayers[4];
    Board b;

    int gamesPlayed = 0;
    double score = 0.0;

    while(gamesPlayed < 100) {

        loadFromConfig(PLAYER_FILE, gamePlayers, b);
        int playerColor = gamePlayers[player]->getColor();

        for(int (&i)[4]:movesMade)for(int &j:i)j = 0;

        move = gamePlayers[player]->moveDoMove(move);
        int nextPlayer = (move.move == END_TURN)?((player+1)%4):player;

        playGame(nextPlayer, gamePlayers, b, 50);

        double scores[4] = {0};
        scoreGame(scores, b);

        double thisScore = 0.0;
        for(int d = 0; d < 4; d++) {
            if(scores[playerColor] > scores[d])thisScore += 0.33;
        }
        thisScore = pow(thisScore, 0.75);

        score += thisScore;
        gamesPlayed++;
    }

    // cout << "Games Played: " << gamesPlayed << ", avg score: " << (score/gamesPlayed) << endl;
    return (score/gamesPlayed);
}

void scoreGame(double (&s)[4], Board &b) {
    for(Corner c:b.getCorners()) {
        if(c.getSettlement() != -1)
            s[c.getSettlement()]++;
        if(c.getCity())
            s[c.getSettlement()]++;
    }
    for(Edge e:b.getEdges()) {
        if(e.getRoad() != -1)
            s[e.getRoad()] += 0.02;
    }
}

void loadFromConfig(string filename, Player* (&gamePlayers)[4], Board &b) {
    ifstream infile(PLAYER_FILE);

    string boardLine;
    getline(infile, boardLine);
    readBoard(boardLine, b);

    string playerLine;
    int color = 0;
    int player = 0;
    while(getline(infile, playerLine)) {
        istringstream is;
        replace(playerLine.begin(), playerLine.end(), ',', ' ');
        replace(playerLine.begin(), playerLine.end(), ':', ' ');
        is.str(playerLine);
        is >> color;

        gamePlayers[player] = new AIPlayer(color, b);
        int (&hand) [5] = gamePlayers[player]->getResHand();

        int temp;
        for(int i = 0; i < 5; i++) {
            is >> temp;
            hand[i] = temp;
        }
        player++;
    }
    infile.close();
}

void playGame(int curIndex, Player* (&gamePlayers)[4], Board &b, int turns) {
    if(turns <= 0)return;

    int dice = die(gen) + die(gen);
    dieRolls[dice-2]++;
    for(Player*p:gamePlayers)
        p->collectFromRoll(dice);

    Player* p = gamePlayers[curIndex];
    PairedMove pMove = PairedMove(BUILD_SETT, MOVE_UNSUCCESSFUL);
    while(pMove.move != END_TURN) {
        pMove = p->moveDoMove();
        movesMade[p->getColor()][pMove.move]++;
    }

    playGame((++curIndex)%4, gamePlayers, b, turns - 1);
}

void setupGame(Player* (&gamePlayers)[4], Board &b) {
    for(int p = 0; p < 4; p++) {
        // if(p == 0)gamePlayers[p] = new UserPlayer(p, b);
        // else
            gamePlayers[p] = new AIPlayer(p, b);
    }

    shuffle(begin(gamePlayers), end(gamePlayers), gen);

    cout << "Let's play catan! Here's our order: " << endl;
    for(int p = 0; p < 4; p++) cout << gamePlayers[p]->getName() << " ";
    cout << endl;

    for(Player* p:gamePlayers) p->placeFirstPair();
    for(int p = 3; p >= 0; p--) gamePlayers[p]->placeSecondPair();

    b.printBoard();
}

string printColor(int player) {
    switch(player) {
        case 0: // Red
            return "\033[31;1m";
        case 1: // Blue
            return "\033[36;1m";
        case 2: // White
            return "\033[37;1m";
        case 3: // Orange (yellow)
            return "\033[33;1m";
        case -2: // Magenta (neutral)
            return "\033[0m\033[35m";
        default:
            return "\033[0m";
    }
}

void writeState(string filename, Player* (&gamePlayers)[4], Board &b) {
    ofstream outfile;
    outfile.open(filename);

    outfile << writeBoard(b);
    for(Player* p:gamePlayers) {
        p->writePlayer(outfile);
    }
    outfile.close();
}

void printData(Player* (&gamePlayers)[4], Board &b) {
    cout << "\n\nDIE ROLLS\n";
    for(int i = 2; i <= 12; i++) {
        cout << i << "\t";
        for(int j = 1; j <= dieRolls[i-2]; j++)cout << "-";
        cout << dieRolls[i-2];
        cout << endl;
    }

    cout << "\n\nRESOURCES LEFT\tBRICK\tLUMBER\tWOOL\tGRAIN\tORE\n";
    for(Player* p:gamePlayers) {
        cout << printColor(p->getColor()) << p->getName() << printColor(-1) << " ";
        for(int i:p->getResHand())
            cout << "\t"<< i;
        cout << endl;
    }

    cout << "\n\nMOVES MADE\tSETT\tROAD\tCITY\tEND\n";
    for(Player *p:gamePlayers) {
        cout << printColor(p->getColor()) << p->getName() << printColor(-1) << " ";
        for(int i:movesMade[p->getColor()])
            cout << "\t" << i;
        cout << endl;
    }

    b.printBoard();
}
