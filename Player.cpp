#include "Player.hpp"

#include <cassert>
#include <iostream>

using namespace std;

class SimplePlayer : public Player {
    public:
    SimplePlayer(const std::string &name_input) : name(name_input){}

    const std::string & get_name() const override {
        return name;
    }

    void add_card(const Card &c) override {
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, 
        Suit &order_up_suit) const override {
            (void)upcard;
            (void)is_dealer;
            (void)round;
            (void)order_up_suit;
            return false;
        }
    
    void add_and_discard(const Card &upcard) override {
        (void)upcard;
    }

    Card lead_card(Suit trump) override {
        (void)trump;
        return hand[0];
    }

    Card play_card(const Card &led_card, Suit trump) override {
        (void)led_card;
        (void)trump;
        return hand[0];
    }

    private:
    std::string name;
    std::vector<Card> hand;

}






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
  assert(false);
}