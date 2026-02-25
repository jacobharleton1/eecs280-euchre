#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "Pack.hpp"
#include "Card.hpp"
#include "Player.hpp"
using namespace std;

struct Game_config {
    bool do_shuffle;
    int points_goal;

    string name[4];
    string type[4];
};

class Game {
    public:
    
    Game(std::istream &pack_input, const Game_config &cfg)
    : pack(pack_input),
      do_shuffle(cfg.do_shuffle),
      points_goal(cfg.points_goal) {
    
        dealer = 0;
        hand_number = 0;
        team1_points = 0;
        team2_points = 0;
        trump_maker = -1;
        trump_set = false;
        trump = SPADES;
        team1_tricks = 0;
        team2_tricks = 0;
        chose_trump = -1;

        //num of players is 4
        for (int i = 0; i < 4; i++) {
        players.push_back(Player_factory(cfg.name[i], cfg.type[i]));
        }
    }

    ~Game(){
        for (Player *player : players){
            delete player;
        }
    }

    void play() {
        int leader = (dealer+1)%4;

        while(team1_points < points_goal && team2_points < points_goal){
            //choose_trump(players[(dealer+1)%4]);
            cout << "Hand " << hand_number << endl;
            cout << players[dealer]->get_name() << " deals" << endl;

            team1_tricks = 0;
            team2_tricks = 0;

            pack.reset();
            shuffle();
            deal();
            
            upcard = pack.deal_one();
            cout << upcard << " turned up" << endl;

            choose_trump();

            leader = (dealer + 1)%4;
            for (int trick_num = 0; trick_num < 5; ++trick_num){
                leader = play_hand(leader, trump);
            }

            int maker_team = (chose_trump % 2);
            int maker_tricks;

            if(maker_team == 0){
                maker_tricks = team1_tricks;
            }
            else{
                maker_tricks = team2_tricks;
            }

            if(team1_tricks > team2_tricks){
                cout << players[0]->get_name() << " and " << players[2]->get_name()
                << " win the hand" << endl;
            }
            else{
                cout << players[1]->get_name() << " and " << players[3]->get_name()
                << " win the hand" << endl;
            }

            scoring(maker_tricks, maker_team);
            dealer = (dealer + 1) % 4;
            hand_number++;

        }

        if (team1_points >= points_goal) {
            cout << players[0]->get_name() << " and " 
            << players[2]->get_name() << " win!" << endl;
        }
        else {
            cout << players[1]->get_name() << " and " 
        << players[3]->get_name() << " win!" << endl;
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
        int points_goal;
        int team1_tricks;
        int team2_tricks;
        int chose_trump;

    private:

        void scoring(int maker_tricks, int maker_team) {
            if (maker_tricks < 3) {
                if (maker_team == 0) team2_points += 2;
                else team1_points += 2;
                cout << "euchred!" << endl;
            }
            else if (maker_tricks == 5) {
                if (maker_team == 0) team1_points += 2;
                else team2_points += 2;
                cout << "march!" << endl;
            }
            else {
                if (maker_team == 0) team1_points += 1;
                else team2_points += 1;
            }
            cout << players[0]->get_name() << " and "
            << players[2]->get_name()
            << " have " << team1_points << " points" << endl;

            cout << players[1]->get_name() << " and "
            << players[3]->get_name()
            << " have " << team2_points << " points" << endl;

            cout << endl;
        }

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

        int play_hand(int leader, Suit trump) {

            Card c1 = players[leader]->lead_card(trump);
            cout << c1 << " led by " << players[leader]->get_name() << endl;

            int player2 = (leader + 1)%4;
            int player3 = (leader + 2)%4;
            int player4 = (leader + 3)%4;
            
            Card c2 = players[player2]->play_card(c1, trump);
            Card c3 = players[player3]->play_card(c1, trump);
            Card c4 = players[player4]->play_card(c1, trump);

            cout << c2 << " played by " << players[player2]->get_name() << endl;
            cout << c3 << " played by " << players[player3]->get_name() << endl;
            cout << c4 << " played by " << players[player4]->get_name() << endl;

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

            cout << players[winner]->get_name() << " takes the trick" << endl;
            cout << endl;

            if (winner == 0 || winner == 2) team1_tricks++;
            else team2_tricks++;

            return winner;
        }

        void choose_trump() {
            trump_set = false;
            chose_trump = -1;
            trump_maker = -1;
            Suit order_up_suit = upcard.get_suit();

            for(int player_index = 0; player_index<4; ++player_index){
                int player_of_focus = (dealer + 1 + player_index) % 4;
                bool is_dealer = (player_of_focus == dealer);

                if(players[player_of_focus]->make_trump(upcard, is_dealer, 
                    1, order_up_suit)){
                    trump = order_up_suit;
                    trump_set = true;
                    chose_trump = player_of_focus;
                    trump_maker = player_of_focus;

                    cout << players[player_of_focus]->get_name() << " orders up " 
                    << trump << "\n\n";
                    players[dealer]->add_and_discard(upcard);
                    return;
                }else{
                    cout << players[player_of_focus]->get_name() 
                    << " passes" << endl;
                }
            }

            for(int player_index = 0; player_index<4; ++player_index){
                int player_of_focus = (dealer + 1 + player_index) % 4;
                bool is_dealer = (player_of_focus == dealer);

                if(players[player_of_focus]->make_trump(upcard,
                     is_dealer, 2, order_up_suit)){
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

    Game_config cfg;
    cfg.do_shuffle = (shuff_type == "shuffle");
    cfg.points_goal = points_to_win;

    cfg.name[0] = name1; 
    cfg.type[0] = type1;
    cfg.name[1] = name2; 
    cfg.type[1] = type2;
    cfg.name[2] = name3; 
    cfg.type[2] = type3;
    cfg.name[3] = name4; 
    cfg.type[3] = type4;

    ifstream pack_file(pack_filename);
    if (!pack_file) {
        cout << "Error opening " << pack_filename << endl;
    return 1;
    }

    cout << " ./euchre.exe ";
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << " ";
    }
    cout << endl;

    // Read command line args and check for errors
    Game game(pack_file, cfg);
    game.play();
    
}