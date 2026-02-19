#include "Player.hpp"

#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;

class SimplePlayer : public Player {
    public:
        SimplePlayer(const std::string &name_in)
            : name(name_in) {}

        
        const std::string & get_name() const override {
            return name;
        }

        void add_card(const Card &c) override {
            hand.push_back(c);
        }

        bool make_trump(const Card &upcard, bool is_dealer, int round, 
            Suit &order_up_suit) const override {
                Suit proposed_suit;
                int good_trump = 0;
                int good_trump_req = 0;

                if (round == 1){
                    proposed_suit = upcard.get_suit();
                    good_trump_req = 2;
                }else if(round == 2){
                    proposed_suit = Suit_next(upcard.get_suit());
                    good_trump_req = 1;
                }

                for (const Card &card_in_hand : hand){
                    if (card_in_hand.is_trump(proposed_suit) 
                    && card_in_hand.is_face_or_ace()){
                        good_trump = good_trump + 1;
                    }
                }
                if (good_trump >= good_trump_req){
                    order_up_suit = proposed_suit;
                    return true;
                }
                if (round == 2 && is_dealer){
                    order_up_suit = proposed_suit;
                    return true;
                }
                return false;
            }
        
        void add_and_discard(const Card &upcard) override {
            hand.push_back(upcard);

            size_t lowest_card_index = 0;
            for (size_t i = 1; i < hand.size(); ++i){
                if (hand[i] < hand[lowest_card_index]){
                    lowest_card_index = i;
                }
            }

            hand.erase(hand.begin() + lowest_card_index);
        }

        Card lead_card(Suit trump) override {
            bool has_non_trump = false;

            for (const Card& card_in_hand : hand){
                if(!card_in_hand.is_trump(trump)){
                    has_non_trump = true;
                }
            }

            size_t best_card_index = 0;

            if(has_non_trump){
                for(size_t index = 0; index< hand.size(); ++index){
                    if(!hand[index].is_trump(trump)){
                        best_card_index = index;
                        break;
                    }
                }

                for(size_t index = 0; index< hand.size(); ++index){
                    if(!hand[index].is_trump(trump) 
                    && hand[best_card_index] < hand[index]){
                        best_card_index = index;
                    }
                }
            }else{
                best_card_index = 0;
                for(size_t index = 0; index< hand.size(); ++index){
                    if(Card_less(hand[best_card_index], hand[index], trump)){
                        best_card_index = index;
                    }
                }
            }

            Card selected_card = hand[best_card_index];
            hand.erase(hand.begin() + best_card_index);
            return selected_card;
        }

        Card play_card(const Card &led_card, Suit trump) override {
            Suit led_suit = led_card.get_suit(trump);

            bool have_suit = false;
            for (const Card &card_in_hand : hand){
                if(card_in_hand.get_suit(trump) == led_suit){
                    have_suit = true;
                    break;
                }
            }

            size_t chosen_card_index = 0;

            if(have_suit){
                for (size_t index = 0; index < hand.size(); ++index){
                    if(hand[index].get_suit(trump) == led_suit){
                        chosen_card_index = index;
                        break;
                    }
                }

                for (size_t index = 0; index < hand.size(); ++index){
                    if (hand[index].get_suit(trump) == led_suit 
                    && Card_less(hand[chosen_card_index], hand[index], led_card, trump)){
                        chosen_card_index = index;
                    }
                }
            }else{
                chosen_card_index = 0;
                for (size_t index = 0; index < hand.size(); ++index){
                    if(Card_less(hand[index], hand[chosen_card_index], led_card, trump)){
                        chosen_card_index = index;
                    }
                }
            }

            Card selected_card = hand[chosen_card_index];
            hand.erase(hand.begin() + chosen_card_index);
            return selected_card;
        }

    private:
        std::string name;
        std::vector<Card> hand;

};

class HumanPlayer : public Player {
    public:
        HumanPlayer(const std::string &name_in)
            : name(name_in) {}

        // 1
        const std::string & get_name() const override {
            return name;
        }
        
        void add_card(const Card &c) override {
            hand.push_back(c);
            sort(hand.begin(), hand.end());
        }

        bool make_trump(const Card &upcard, bool is_dealer, 
        int round, Suit &order_up_suit) const override{
            print_hand();
            cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";

            string decision;
            cin >> decision;

            if(decision != "pass"){
                order_up_suit = string_to_suit(decision);
                return true;
            }

            return false;
        }

        void add_and_discard(const Card &upcard) override {
            hand.push_back(upcard);
            sort(hand.begin(), hand.end());

            print_hand();
            cout << "Discard upcard: [-1]\n";
            cout << "Human player " << name << ", please select a card to discard:\n";

            int index_to_discard;
            cin >> index_to_discard;

            if (index_to_discard == -1){
              for (size_t index = 0; index < hand.size(); ++index){
                    if (hand[index] == upcard){
                        hand.erase(hand.begin() + index);
                        break;
                    }
                }
            }else{
                hand.erase(hand.begin() + index_to_discard);
            }
        }

        Card lead_card(Suit trump) override {
            print_hand();
            cout << "Human player " << name << ", please select a card:\n";
            int index_of_card;
            cin >> index_of_card;

            Card chosen_card = hand[index_of_card];
            hand.erase(hand.begin() + index_of_card);
            return chosen_card;
        }

        Card play_card(const Card &led_card, Suit trump) override {
            print_hand();
            cout << "Human player " << name << ", please select a card:\n";
            int index_of_card;
            cin >> index_of_card;

            Card chosen_card = hand[index_of_card];
            hand.erase(hand.begin() + index_of_card);
            return chosen_card;
        }

    private:
        std::string name;
        std::vector<Card> hand;
        void print_hand() const {
            for (size_t index = 0; index < hand.size(); ++index){
                cout << "Human player " << name << "'s hand: "
                << "[" << index << "]" << hand[index] << "\n";
            }
        }
};




Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  if (strategy == "Human") {
    return new HumanPlayer(name);
  }

  assert(false);
  return nullptr;
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}