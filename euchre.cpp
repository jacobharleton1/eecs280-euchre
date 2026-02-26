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

void error_message() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
         << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
         << "NAME4 TYPE4" << endl;
}

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

        for (int i = 0; i < 4; i++) {
            players.push_back(Player_factory(cfg.name[i], cfg.type[i]));
        }
    }

    ~Game() {
        for (Player *player : players) {
            delete player;
        }
    }

    void play() {
        while (!game_over()) {
            play_one_hand();
        }
        print_game_winner();
    }

  private:
    vector<Player*> players;
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
    bool game_over() const {
        return team1_points >= points_goal || team2_points >= points_goal;
    }

    void play_one_hand() {
        print_hand_start();
        reset_tricks();
        reset_pack_shuffle_deal();
        turn_upcard();
        choose_trump();
        play_five_tricks();
        score_and_print_hand();
        advance_dealer_and_hand();
    }

    void print_hand_start() const {
        cout << "Hand " << hand_number << endl;
        cout << players[dealer]->get_name() << " deals" << endl;
    }

    void reset_tricks() {
        team1_tricks = 0;
        team2_tricks = 0;
    }

    void reset_pack_shuffle_deal() {
        pack.reset();
        shuffle();
        deal();
    }

    void shuffle() {
        if (do_shuffle) {
            pack.shuffle();
        }
    }

    void turn_upcard() {
        upcard = pack.deal_one();
        cout << upcard << " turned up" << endl;
    }

    void play_five_tricks() {
        int leader = (dealer + 1) % 4;
        for (int trick_num = 0; trick_num < 5; ++trick_num) {
            leader = play_hand(leader, trump);
        }
    }

    void score_and_print_hand() {
        int maker_team = chose_trump % 2;
        int maker_tricks = (maker_team == 0) ? team1_tricks : team2_tricks;

        print_hand_winner();
        scoring(maker_tricks, maker_team);
    }

    void print_hand_winner() const {
        if (team1_tricks > team2_tricks) {
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " win the hand" << endl;
        } else {
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " win the hand" << endl;
        }
    }

    void advance_dealer_and_hand() {
        dealer = (dealer + 1) % 4;
        hand_number++;
    }

    void print_game_winner() const {
        if (team1_points >= points_goal) {
            cout << players[0]->get_name() << " and "
                 << players[2]->get_name() << " win!" << endl;
        } else {
            cout << players[1]->get_name() << " and "
                 << players[3]->get_name() << " win!" << endl;
        }
    }

    void scoring(int maker_tricks, int maker_team) {
        if (maker_tricks < 3) {
            if (maker_team == 0) team2_points += 2;
            else team1_points += 2;
            cout << "euchred!" << endl;
        } else if (maker_tricks == 5) {
            if (maker_team == 0) team1_points += 2;
            else team2_points += 2;
            cout << "march!" << endl;
        } else {
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

    void deal() {
        const int player_number = 4;
        int order[4] = {
            (dealer + 1) % 4,
            (dealer + 2) % 4,
            (dealer + 3) % 4,
            dealer
        };

        int first[4] = {3, 2, 3, 2};
        int second[4] = {2, 3, 2, 3};

        for (int i = 0; i < player_number; i++) {
            for (int j = 0; j < first[i]; j++) {
                players[order[i]]->add_card(pack.deal_one());
            }
        }

        for (int i = 0; i < player_number; i++) {
            for (int j = 0; j < second[i]; j++) {
                players[order[i]]->add_card(pack.deal_one());
            }
        }
    }

    int play_hand(int leader, Suit trump_suit) {
        Card c1 = players[leader]->lead_card(trump_suit);
        cout << c1 << " led by " << players[leader]->get_name() << endl;

        int player2 = (leader + 1) % 4;
        int player3 = (leader + 2) % 4;
        int player4 = (leader + 3) % 4;

        Card c2 = players[player2]->play_card(c1, trump_suit);
        cout << c2 << " played by " << players[player2]->get_name() << endl;

        Card c3 = players[player3]->play_card(c1, trump_suit);
        cout << c3 << " played by " << players[player3]->get_name() << endl;

        Card c4 = players[player4]->play_card(c1, trump_suit);
        cout << c4 << " played by " << players[player4]->get_name() << endl;

        array<Card, 4> handcards = {c1, c2, c3, c4};

        int winner_offset = 0;
        Card winning_card = handcards[0];

        for (size_t i = 1; i < handcards.size(); ++i) {
            if (Card_less(winning_card, handcards[i], c1, trump_suit)) {
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

    void reset_trump_state() {
        trump_set = false;
        chose_trump = -1;
        trump_maker = -1;
    }

    void print_orders_up(int player_idx) const {
        cout << players[player_idx]->get_name() << " orders up "
             << trump << "\n\n";
    }

    void print_passes(int player_idx) const {
        cout << players[player_idx]->get_name() << " passes" << endl;
    }

    bool try_make_trump_round(int round, Suit &order_up_suit) {
        for (int player_index = 0; player_index < 4; ++player_index) {
            int player_of_focus = (dealer + 1 + player_index) % 4;
            bool is_dealer = (player_of_focus == dealer);

            if (players[player_of_focus]->make_trump(upcard, is_dealer,
                                                     round, order_up_suit)) {
                trump = order_up_suit;
                trump_set = true;
                chose_trump = player_of_focus;
                trump_maker = player_of_focus;

                print_orders_up(player_of_focus);

                if (round == 1) {
                    players[dealer]->add_and_discard(upcard);
                }
                return true;
            }

            print_passes(player_of_focus);
        }
        return false;
    }

    void screw_the_dealer() {
        trump = Suit_next(upcard.get_suit());
        trump_set = true;
        chose_trump = dealer;
        trump_maker = dealer;
        print_orders_up(dealer);
    }

    void choose_trump() {
        reset_trump_state();

        Suit order_up_suit = upcard.get_suit();

        if (try_make_trump_round(1, order_up_suit)) return;
        if (try_make_trump_round(2, order_up_suit)) return;

        screw_the_dealer();
    }
};

int run_game(const string &pack_filename,
             int argc,
             char **argv,
             const Game_config &cfg) {
    ifstream pack_file(pack_filename);
    if (!pack_file) {
        cout << "Error opening " << pack_filename << endl;
        return 1;
    }

    cout << "./euchre.exe ";
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << " ";
    }
    cout << endl;

    Game game(pack_file, cfg);
    game.play();

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 12) {
        error_message();
        return 1;
    }

    string pack_filename = argv[1];
    string shuff_type = argv[2];
    int points_to_win = stoi(argv[3]);

    if (shuff_type != "shuffle" && shuff_type != "noshuffle") {
        error_message();
        return 1;
    }

    if (points_to_win < 1 || points_to_win > 100) {
        error_message();
        return 1;
    }

    for (int i = 5; i < 12; i += 2) {
        string type = argv[i];
        if (type != "Simple" && type != "Human") {
            error_message();
            return 1;
        }
    }

    Game_config cfg;
    cfg.do_shuffle = (shuff_type == "shuffle");
    cfg.points_goal = points_to_win;

    cfg.name[0] = argv[4];  cfg.type[0] = argv[5];
    cfg.name[1] = argv[6];  cfg.type[1] = argv[7];
    cfg.name[2] = argv[8];  cfg.type[2] = argv[9];
    cfg.name[3] = argv[10]; cfg.type[3] = argv[11];

    return run_game(pack_filename, argc, argv, cfg);
}