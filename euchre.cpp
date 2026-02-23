#include <iostream>
using namespace std;

class Game {
    public:
    // Game(/* game details */);
    void play();

    private:
  std::vector<Player*> players;
  Pack pack;
  // ...

  void shuffle();
  void deal(/* ... */);
  void make_trump(/* ... */);
  void play_hand(/* ... */);
}


int main(int argc, char **argv) {
  // Read command line args and check for errors
  Game game(/* game details */);
  game.play();

/*  THINGS TO IMPLEMENT

cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;


    // Assume pack_filename is a variable containing
// the specified pack filename from argv
cout << "Error opening " << pack_filename << endl;

at the end of a game:
for (size_t i = 0; i < players.size(); ++i) {
  delete players[i];
}

*/