#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////

// card members functions

//EFFECTS Initializes Card to the Two of Spades
Card::Card() {
  this->rank = TWO;
  this->suit = SPADES;
}

//EFFECTS Initializes Card to specified rank and suit
Card::Card(Rank rank_in, Suit suit_in) {
  this->rank = rank_in;
  this->suit = suit_in;
}

//EFFECTS Returns the rank
Rank Card::get_rank() const {
  return this->rank;
}

//EFFECTS Returns the suit. Does not consider trump.
Suit Card::get_suit() const {
  return this->suit;
}

//EFFECTS Returns the suit
//HINT: the left bower is the trump suit!
Suit Card::get_suit(Suit trump) const {
  if (is_left_bower(trump)) return trump; 
  return suit; 
}

//EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
bool Card::is_face_or_ace() const {
  if (rank == JACK || rank == QUEEN || rank == KING || rank == ACE) {
    return true;
  }
  return false;
}

//EFFECTS Returns true if card is the Jack of the trump suit
bool Card::is_right_bower(Suit trump) const {
  if (rank == JACK && suit == trump) return true;
  return false;
}

//EFFECTS Returns true if card is the Jack of the next suit
bool Card::is_left_bower(Suit trump) const {
  if (suit == Suit_next(trump) && rank == JACK) return true;
  return false;
}

//EFFECTS Returns true if the card is a trump card. All cards of the trump
// suit are trump cards. The left bower is also a trump card.
bool Card::is_trump(Suit trump) const {
  if (suit == trump) return true;
  if (is_left_bower(trump)) return true;
  return false;
}

// stream operators 

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

//EFFECTS Reads a Card from a stream in the format "Two of Spades"
std::istream & operator>>(std::istream &is, Card &card) {
  string suit_in;
  string rank_in;
  string trash;
  is >> rank_in >> trash >> suit_in; 
  card = Card(string_to_rank(rank_in), string_to_suit(suit_in));
  return is;
}

//comparison operators

//EFFECTS Returns true if lhs is lower value than rhs.
// Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() < rhs.get_rank()) return true;
  if (lhs.get_rank() == rhs.get_rank()) {
    if (lhs.get_suit() < rhs.get_suit()) return true;
  }
  return false;
}

//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs.
// Does not consider trump.
bool operator<=(const Card &lhs, const Card &rhs) {
  return !(rhs < lhs);
}

//EFFECTS Returns true if lhs is higher value than rhs.
// Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs) {
  return rhs < lhs;
}

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
// Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs) {
  return !(lhs < rhs);
}

//EFFECTS Returns true if lhs is same card as rhs.
// Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit()) {
    return true;
  }
  return false;
}

//EFFECTS Returns true if lhs is not the same card as rhs.
// Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs) {
  return !(lhs == rhs);
}

// other functions

//EFFECTS returns the next suit, which is the suit of the same color
Suit Suit_next(Suit suit) {
  Suit next = static_cast<Suit>((suit + 2) % 4);
  return next;
}

//EFFECTS Returns true if a is lower value than b. Uses trump to determine order.
bool Card_less(const Card &a, const Card &b, Suit trump) {
  if (b.is_right_bower(trump)) return true;

  else if (a.is_right_bower(trump)) return false;

  else if (b.is_left_bower(trump)) return true;

  else if (a.is_left_bower(trump)) return false;

  else if (b.is_trump(trump) && a.is_trump(trump)) return a < b;

  else if (b.is_trump(trump)) return true;

  else if (a.is_trump(trump)) return false;

  return a < b;

}

//EFFECTS Returns true if a is lower value than b. Uses both the trump suit
// and the suit led to determine order.
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
  Suit led_suit = led_card.get_suit();
  if (led_card.is_left_bower(trump)) led_suit = trump;


  if (b.is_right_bower(trump)) return true;

  else if (a.is_right_bower(trump)) return false;

  else if (b.is_left_bower(trump)) return true;

  else if (a.is_left_bower(trump)) return false;

  else if (b.is_trump(trump) && a.is_trump(trump)) return a < b;

  else if (b.is_trump(trump)) return true;

  else if (a.is_trump(trump)) return false;

  else if (b.get_suit() == led_suit && a.get_suit() == led_suit) {
    return a < b;
  }

  else if (b.get_suit() == led_suit) return true;

  else if (a.get_suit() == led_suit) return false;

  return a < b;
}