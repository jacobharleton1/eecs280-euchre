#include "Pack.hpp"

 // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack() {
    int index = 0;

    for (int s = SPADES; s <= DIAMONDS; s++) {
        for (int r = NINE; r <= ACE; r++) {
            cards[index] = Card(static_cast<Rank>(r),static_cast<Suit>(s));
            index++;
        }
    }
    next = 0;
  }

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(std::istream& pack_input) {
    for (int index = 0; index < PACK_SIZE; index++) {
        pack_input >> cards[index];
    }
    next = 0;
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one() {
    Card to_be_dealt = cards[next];
    next++;
    return to_be_dealt;
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset() { 
    next = 0;
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack::shuffle() {
    Card shuffled[PACK_SIZE];
    for (int shuffle_count = 0; shuffle_count < 7; shuffle_count++) {
        
        for (int index = 0; index < PACK_SIZE/2; index++) {
            shuffled[2*index] = cards[index + PACK_SIZE/2];
            shuffled[2*index + 1] = cards[index];
        }
        for (int i = 0; i < PACK_SIZE; i++) {
        cards[i] = shuffled[i];
        }
    }
    next = 0;
  }

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const {
    return next == PACK_SIZE;
  }