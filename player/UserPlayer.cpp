#include <iostream>

using namespace std;

UserPlayer::UserPlayer(int color, Board b):
    Player(c, b)
{}

string UserPlayer::inputName() {
    string n;
    cout << "My color is " << COLOR_STRINGS[color] << ". What is my name? ";
    cin >> n;
    return n;
}
