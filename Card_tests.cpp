#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_card_default_ctor) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

TEST(test_get_rank) {
    Card d(FOUR, DIAMONDS);
    Card s(FIVE, SPADES);
    Card h(THREE, HEARTS);
    Card c(EIGHT, CLUBS);

    ASSERT_EQUAL(FOUR, d.get_rank());
    ASSERT_EQUAL(FIVE, s.get_rank());
    ASSERT_EQUAL(THREE, h.get_rank());
    ASSERT_EQUAL(EIGHT, c.get_rank());
}

TEST(test_get_suit) {
    Card d(FOUR, DIAMONDS);
    Card s(FIVE, SPADES);
    Card h(THREE, HEARTS);
    Card c(EIGHT, CLUBS);

    ASSERT_EQUAL(DIAMONDS, d.get_suit());
    ASSERT_EQUAL(SPADES, s.get_suit());
    ASSERT_EQUAL(HEARTS, h.get_suit());
    ASSERT_EQUAL(CLUBS, c.get_suit());
}

TEST(test_get_suit_left_bower) {
    Suit trump = SPADES;
    Card c(KING, CLUBS);
    Card l(JACK, CLUBS);
    Card n(JACK, HEARTS);

    ASSERT_EQUAL(CLUBS, c.get_suit(trump));
    ASSERT_EQUAL(HEARTS, n.get_suit(trump));
    ASSERT_EQUAL(SPADES, l.get_suit(trump));
}

TEST(test_is_face_or_ace) {
    Card a(ACE, DIAMONDS);
    Card b(KING, SPADES);
    Card c(QUEEN, HEARTS);
    Card d(JACK, CLUBS);
    Card e(TEN, DIAMONDS);
    Card f(FIVE, SPADES);

    ASSERT_TRUE(a.is_face_or_ace());
    ASSERT_TRUE(b.is_face_or_ace());
    ASSERT_TRUE(c.is_face_or_ace());
    ASSERT_TRUE(d.is_face_or_ace());
    ASSERT_TRUE(!e.is_face_or_ace());
    ASSERT_TRUE(!f.is_face_or_ace());

}

TEST(test_is_right_bower) {
    Suit trump = SPADES;
    Card a(FIVE, CLUBS);
    Card b(JACK, SPADES);
    Card c(JACK, CLUBS);

    ASSERT_TRUE(!a.is_right_bower(trump));
    ASSERT_TRUE(b.is_right_bower(trump));
    ASSERT_TRUE(!c.is_right_bower(trump));

    trump = HEARTS;
    Card e(FIVE, HEARTS);
    Card f(JACK, DIAMONDS);
    Card g(JACK, HEARTS);

    ASSERT_TRUE(!e.is_right_bower(trump));
    ASSERT_TRUE(!f.is_right_bower(trump));
    ASSERT_TRUE(g.is_right_bower(trump));
}

TEST(test_is_left_bower) {
    Suit trump = CLUBS;
    Card a(FIVE, SPADES);
    Card b(JACK, CLUBS);
    Card c(JACK, SPADES);

    ASSERT_TRUE(!a.is_left_bower(trump));
    ASSERT_TRUE(!b.is_left_bower(trump));
    ASSERT_TRUE(c.is_left_bower(trump));

    trump = HEARTS;
    Card e(FIVE, DIAMONDS);
    Card f(JACK, HEARTS);
    Card g(JACK, DIAMONDS);

    ASSERT_TRUE(!e.is_left_bower(trump));
    ASSERT_TRUE(!f.is_left_bower(trump));
    ASSERT_TRUE(g.is_left_bower(trump));
}

TEST(test_is_trump) {
    Suit trump = DIAMONDS;

    Card a(FOUR, DIAMONDS);
    Card b(FIVE, SPADES);
    Card c(THREE, HEARTS);
    Card d(ACE, CLUBS);
    Card e(KING, DIAMONDS);
    Card f(FIVE, SPADES);
    Card g(THREE, HEARTS);
    Card h(JACK, DIAMONDS);

    ASSERT_TRUE(a.is_trump(trump));
    ASSERT_TRUE(!b.is_trump(trump));
    ASSERT_TRUE(!c.is_trump(trump));
    ASSERT_TRUE(!d.is_trump(trump));
    ASSERT_TRUE(e.is_trump(trump));
    ASSERT_TRUE(!f.is_trump(trump));
    ASSERT_TRUE(!g.is_trump(trump));
    ASSERT_TRUE(h.is_trump(trump));
}

// stream tests

TEST(test_output_operator) {
    Card c(ACE, SPADES);
    std::ostringstream oss;

    oss << c;

    ASSERT_EQUAL("Ace of Spades", oss.str());
}

TEST(test_input_operator) {
    std::istringstream iss("King of Hearts");
    Card c;

    iss >> c;

    ASSERT_EQUAL(KING, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

//operator tests

TEST(test_operator_less_than) {
    Card low(NINE, SPADES);
    Card high(TEN, SPADES);
    Card also_high(JACK, CLUBS);

    ASSERT_TRUE(low < high);
    ASSERT_TRUE(low < also_high);
    ASSERT_TRUE(!(high < low));
    ASSERT_TRUE(!(also_high < low));
}

TEST(test_operator_less_suit_tiebreak) {
    // Same rank goes to order: S < H < C < D
    Card s(NINE, SPADES);
    Card h(NINE, HEARTS);
    Card c(NINE, CLUBS);
    Card d(NINE, DIAMONDS);

    ASSERT_TRUE(s < h);
    ASSERT_TRUE(h < c);
    ASSERT_TRUE(c < d);
    ASSERT_TRUE(s < d);

    ASSERT_FALSE(h < s);
    ASSERT_FALSE(c < h);
    ASSERT_FALSE(d < c);
    ASSERT_FALSE(d < s);
}

TEST(test_operator_greater) {
    Card a(ACE, DIAMONDS);
    Card b(KING, DIAMONDS);

    ASSERT_TRUE(a > b);
    ASSERT_TRUE(!(b > a));
}

TEST(test_operator_equal_and_not_equal) {
    Card a(ACE, HEARTS);
    Card b(ACE, HEARTS);
    Card c(ACE, SPADES);

    ASSERT_TRUE(a == b);
    ASSERT_TRUE(!(a != b));

    ASSERT_TRUE(a != c);
    ASSERT_TRUE(!(a == c));
}

TEST(test_operator_leq_and_geq) {
    Card a(QUEEN, CLUBS);
    Card b(QUEEN, CLUBS);
    Card c(KING, CLUBS);
    Card d(KING, SPADES);

    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a >= b);

    ASSERT_TRUE(a <= c);
    ASSERT_FALSE(c <= a);
    ASSERT_TRUE(a <= d);
    ASSERT_FALSE(d <= a);

    ASSERT_TRUE(c >= a);
    ASSERT_FALSE(a >= c);
    ASSERT_TRUE(d >= b);
    ASSERT_FALSE(b >= d);
}

// other functions tests

TEST(test_suit_next) {
    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
    ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
}


TEST(test_card_less_trump_beats_nonutrump) {
    Suit trump = HEARTS;
    Card a(ACE, SPADES);
    Card b(NINE, HEARTS);

    ASSERT_TRUE(Card_less(a, b, trump));
    ASSERT_FALSE(Card_less(b, a, trump));
}

TEST(test_card_less_right_bower_highest) {
    Suit trump = SPADES;
    Card rb(JACK, SPADES);
    Card lb(JACK, CLUBS);
    Card tA(ACE, SPADES);
    Card nA(ACE, HEARTS);

    ASSERT_TRUE(Card_less(lb, rb, trump));
    ASSERT_TRUE(Card_less(tA, rb, trump));
    ASSERT_TRUE(Card_less(nA, rb, trump));

    ASSERT_FALSE(Card_less(rb, lb, trump));
    ASSERT_FALSE(Card_less(rb, tA, trump));
    ASSERT_FALSE(Card_less(rb, nA, trump));
}

TEST(test_card_less_left_bower_highest) {
    Suit trump = SPADES;
    Card lb(JACK, CLUBS);
    Card tA(ACE, SPADES);
    Card tK(KING, SPADES);
    Card nA(ACE, HEARTS);

    ASSERT_TRUE(Card_less(tA, lb, trump));
    ASSERT_TRUE(Card_less(tK, lb, trump));
    ASSERT_TRUE(Card_less(nA, lb, trump));

    ASSERT_FALSE(Card_less(lb, tA, trump));
    ASSERT_FALSE(Card_less(lb, tK, trump));
}

TEST(test_card_less_both_trump) {
    Suit trump = DIAMONDS;
    Card a(QUEEN, DIAMONDS);
    Card b(KING, DIAMONDS);

    ASSERT_TRUE(Card_less(a, b, trump));
    ASSERT_FALSE(Card_less(b, a, trump));
}

TEST(test_card_less_both_nontrump) {
    Suit trump = CLUBS;
    Card a(TEN, HEARTS);
    Card b(JACK, HEARTS);

    ASSERT_TRUE(Card_less(a, b, trump));
    ASSERT_FALSE(Card_less(b, a, trump));
}

TEST(test_card_less_led_and_nontrump) {
    Suit trump = SPADES;
    Card led(NINE, HEARTS);
    Card a(ACE, CLUBS);
    Card b(NINE, HEARTS);

    ASSERT_TRUE(Card_less(a, b, led, trump));
    ASSERT_FALSE(Card_less(b, a, led, trump));
}

TEST(test_card_less_trump_beats_led) {
    Suit trump = SPADES;
    Card led(NINE, HEARTS);
    Card a(ACE, HEARTS);
    Card b(NINE, SPADES);

    ASSERT_TRUE(Card_less(a, b, led, trump));
    ASSERT_FALSE(Card_less(b, a, led, trump));
}

TEST(test_card_less_both_led) {
    Suit trump = SPADES;
    Card led(FOUR, DIAMONDS);
    Card a(NINE, DIAMONDS);
    Card b(TEN, DIAMONDS);

    ASSERT_TRUE(Card_less(a, b, led, trump));
    ASSERT_FALSE(Card_less(b, a, led, trump));
}

TEST(test_card_less_led_is_trump) {
    Suit trump = HEARTS;
    Card led(TEN, HEARTS); 
    Card a(ACE, HEARTS);
    Card b(NINE, HEARTS);

    ASSERT_TRUE(Card_less(b, a, led, trump));
    ASSERT_FALSE(Card_less(a, b, led, trump));
}

TEST(test_card_less_led_left_bower_counts_as_trump_led) {
    Suit trump = SPADES;
    Card led(JACK, CLUBS);
    Card a(ACE, SPADES);
    Card b(ACE, HEARTS); 

    ASSERT_TRUE(Card_less(b, a, led, trump));
    ASSERT_FALSE(Card_less(a, b, led, trump));
}

TEST(test_card_less_right_bower_always_highest_with_led) {
    Suit trump = DIAMONDS;
    Card led(ACE, HEARTS);
    Card rb(JACK, DIAMONDS);
    Card a(ACE, HEARTS);
    Card b(ACE, DIAMONDS);

    ASSERT_TRUE(Card_less(a, rb, led, trump));
    ASSERT_TRUE(Card_less(b, rb, led, trump));
    ASSERT_FALSE(Card_less(rb, a, led, trump));
    ASSERT_FALSE(Card_less(rb, b, led, trump));
}

TEST_MAIN()

