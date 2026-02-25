#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "Pack.hpp"
#include "Card.hpp"
#include "Player.hpp"
using namespace std;

class Game {
    public:
    
    Game(std::istream &pack_input,
        const string &shuffle_type, int points_goal_in,
        const string &name1, const string &type1, const string &name2, 
        const string &type2, const string &name3, const string &type3,
        const string &name4, const string &type4)
        : pack(pack_input), points_goal(points_goal_in) {
      
        do_shuffle = (shuffle_type == "shuffle");
        dealer = 0;
        hand_number = 0;
        team1_points = 0;
        team2_points = 0;
        trump_maker = -1;
        trump_set = false;
        trump = SPADES;
        team1_tricks = 0;
        team2_tricks = 0;
        tricks_played = 0;

        players.push_back(Player_factory(name1, type1));
        players.push_back(Player_factory(name2, type2));
        players.push_back(Player_factory(name3, type3));
        players.push_back(Player_factory(name4, type4));
    }

    ~Game(){
        for (Player *player : players){
            delete player;
        }
    }

    void play() {
        int leader = (dealer+1)%4;
        tricks_played = 0;

        while(team1_points < points_goal && team2_points < points_goal){
            //choose_trump(players[(dealer+1)%4]);
            cout << "Hand " << hand_number << endl;
            cout << players[dealer]->get_name() << " deals" << endl;

            team1_tricks = 0;
            team2_tricks = 0;
            tricks_played = 0;

            while(tricks_played < 5) {
                play_hand(leader, trump);
                tricks_played++;
            }
            //if team 1 chooses trump
            if (chose_trump == 0 || chose_trump == 2) {
                if (team1_tricks == 3 || team1_tricks == 4) ++team1_points;
                else if (team1_tricks == 5) team1_points +=2;
                else if (team2_tricks >= 3) team2_points +=2;
            }

            //if team 2 chooses trump
            else {
                if (team2_tricks == 3 || team2_tricks == 4) team2_points +=1;
                else if (team2_tricks == 5) team2_points +=2;
                else if (team1_tricks >= 3) team1_points+=2;
            }


            dealer = (dealer + 1) % 4;
            leader = (dealer+1)%4;
            hand_number +=1;
            team1_tricks = 0;
            team2_tricks = 0;
            tricks_played = 0;
        }
        if (team1_points >= points_goal) {
            cout << players[0] << " and " << players[2] << " win!";
        }
        else cout << players[1] << " and " << players[3] << " win!";
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
        int points_goal;
        int team1_tricks;
        int team2_tricks;
        int tricks_played;
        int chose_trump;

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

        void deal() {
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
        void play_hand(int leader, Suit trump) {

            Card c1 = players[leader]->lead_card(trump);
            Card c2 = players[(leader+1)%4]->play_card(c1, trump);
            Card c3 = players[(leader+2)%4]->play_card(c1, trump);
            Card c4 = players[(leader+3)%4]->play_card(c1, trump);

            array<Card,4> handcards = {c1, c2, c3, c4};

            int winner_offset = 0;
            Card winning_card = handcards[0];

            for (size_t i = 1; i < handcards.size(); ++i) {
                if (Card_less(winning_card, handcards[i], c1, trump)) {
                winning_card = handcards[i];
                winner_offset = static_cast<int>(i);
                }
            }

            int winner = (leader + winner_offset) % 4;

            cout << players[winner]->get_name() << " takes the trick\n";

            if (winner == 0 || winner == 2) team1_tricks++;
            else team2_tricks++;

            tricks_played++;
        }

        void choose_trump() {
            trump_set = false;
            chose_trump = -1;
            trump_maker = -1;
            Suit order_up_suit = upcard.get_suit();

            for(int player_index = 0; player_index<4; ++player_index){
                int player_of_focus = (dealer + 1 + player_index) % 4;
                bool is_dealer = (player_of_focus == dealer);

                if(players[player_of_focus]->make_trump(upcard, is_dealer, 1, order_up_suit)){
                    trump = order_up_suit;
                    trump_set = true;
                    chose_trump = player_of_focus;
                    trump_maker = player_of_focus;

                    cout << players[player_of_focus]->get_name() << " orders up " 
                    << trump << "\n\n";
                    players[dealer]->add_and_discard(upcard);
                    return;
                }else{
                    cout << players[player_of_focus]->get_name() << " passes" << endl;
                }
            }

            for(int player_index = 0; player_index<4; ++player_index){
                int player_of_focus = (dealer + 1 + player_index) % 4;
                bool is_dealer = (player_of_focus == dealer);

                if(players[player_of_focus]->make_trump(upcard, is_dealer, 2, order_up_suit)){
                    trump = order_up_suit;
                    trump_set = true;
                    chose_trump = player_of_focus;
                    trump_maker = player_of_focus;

                    cout << players[player_of_focus]->get_name() << " orders up "
                    << trump << "\n\n";
                    return;
                }else{
                    cout << players[player_of_focus]->get_name() << " passes" << endl;
                }
            }
        }
};


int main(int argc, char **argv) {
    bool error = false;
    
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    string pack_filename = argv[1];
    string shuff_type = argv[2];
    int points_to_win = stoi(argv[3]);


    if (shuff_type != "shuffle" && shuff_type != "noshuffle") error = true;
    else if (points_to_win < 1 || points_to_win > 100) error = true;

    for (int i = 5; i < 12; i += 2) {
        string type = argv[i];
        if (type != "Simple" && type != "Human") error = true;
    }

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
    Game game(pack_file, shuff_type, points_to_win, name1,
              type1, name2, type2, name3, type3, name4, type4);
    game.play();
    
}