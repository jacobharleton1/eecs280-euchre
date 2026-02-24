#include <iostream>
#include <fstream>
#include <string>
#include "Pack.hpp"
#include "Card.hpp"
#include "Player.hpp"
using namespace std;

class Game {
    public:
    
    Game(std::istream &pack_input,
        const std::string &shuffle_type,
        const std::string &name1, const std::string &type1,
        const std::string &name2, const std::string &type2,
        const std::string &name3, const std::string &type3,
        const std::string &name4, const std::string &type4)
        : pack(pack_input)
    {
        do_shuffle = (shuffle_type == "shuffle");
        dealer = 0;
        hand_number = 0;
        team1_points = 0;
        team2_points = 0;
        trump_maker = -1;
        trump_set = false;
        trump = SPADES;

        players.push_back(Player_factory(name1, type1));
        players.push_back(Player_factory(name2, type2));
        players.push_back(Player_factory(name3, type3));
        players.push_back(Player_factory(name4, type4));
    }

    ~Game(){

    }

    ~Game(){
        for (Player *player : players){
            delete player;
        }
    }

    void play(int points_goal){
        while(team1_points < points_goal && team2_points < points_goal){
            play_hand();
            dealer = (dealer + 1) % 4;
            hand_number = hand_number + 1;
        }
    }

    private:
        std::vector<Player*> players;
        Pack pack;
        bool do_shuffle;
        int dealer;
        int hand_number;
        int team1_points;
        int team2_points;
        int trump_maker;
        bool trump_set;
        Suit trump;
        Card upcard;

    private:
        void shuffle(){
            if(do_shuffle){
                pack.shuffle();
            }
        }

        void reset_hand(){
            trump_maker = -1;
            trump_set = false;
        }

        void deal(int dealer) {
            const int player_number = 4;
            int order[4] = {
                (dealer + 1)%4,
                (dealer + 2)%4,
                (dealer + 3)%4,
                dealer};

            int first[4] = {3,2,3,2};
            int second[4] = {2,3,2,3};

            //dealing first time around
            for (int i = 0; i < player_number; i++) {
                for (int j = 0; j < first[i]; j++) {
                    players[order[i]]->add_card(pack.deal_one());
                }
            }

            //dealing first time around
            for (int i = 0; i < player_number; i++) {
                for (int j = 0; j < second[i]; j++) {
                    players[order[i]]->add_card(pack.deal_one());
                }
            }
            
        }

        void make_trump(int suit) {
        
        }
        void play_hand(/* ... */) {

        }

        void choose_trump() {
            //implementation of logic for how player chooses lead suit
            //break into either human --> prompt ask
            // or simple robot i.e. go through their hand or sum idk
        }

}


int main(int argc, char **argv) {
    string pack_filename = argv[1];
    string shuff_type = argv[2];
    int points_to_win = stoi(argv[3]);
    bool error = false;
    

    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    else if (shuff_type != "shuffle" && shuff_type != "noshuffle") error = true;
    else if (points_to_win < 1 || points_to_win > 100) error = true;

    // loop through to make sure each player type is human or simple
    for (int i = 5; i < 12; i += 2) {
        string type = argv[i];
        if (type != "Simple" && type != "Human") error = true;
    }
    
    // if any generated error will show as true and print error message
    if (error) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    string name1 = argv[4];
    string type1 = argv[5];
    string name2 = argv[6];
    string type2 = argv[7];
    string name3 = argv[8];
    string type3 = argv[9];
    string name4 = argv[10];
    string type4 = argv[11];

    ifstream pack_file(pack_filename);
    if (!pack_file) {
        cout << "Error opening " << pack_filename << endl;
    return 1;
    }

    

  // Read command line args and check for errors
  Game game(pack_filename, shuff_type, name1, type1, name2, type2, name3, type3, name4, type4);
  game.play(points_to_win);

  for (size_t i = 0; i < players.size(); ++i) {
    delete players[i];
    }
}