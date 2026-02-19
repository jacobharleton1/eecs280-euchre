#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

TEST(test_pack_ctor) {
    std::istringstream iss(
        "Nine of Clubs\n" 
        "Nine of Diamonds\n" 
        "Nine of Hearts\n"
        "Nine of Spades\n"
        "Ten of Clubs\n"
        "Ten of Diamonds\n"
        "Ten of Hearts\n"
        "Ten of Spades\n"
        "Jack of Clubs\n"
        "Jack of Diamonds\n"
        "Jack of Hearts\n"
        "Jack of Spades\n"
        "Queen of Clubs\n"
        "Queen of Diamonds\n"
        "Queen of Hearts\n"
        "Queen of Spades\n"
        "King of Clubs\n"
        "King of Diamonds\n"
        "King of Hearts\n"
        "King of Spades\n"
        "Ace of Clubs\n"
        "Ace of Hearts\n"
        "Ace of Diamonds\n"
        "Ace of Spades\n"
    );

    Pack p(iss);

    ASSERT_EQUAL(Card(NINE, CLUBS), p.deal_one());
    ASSERT_EQUAL(Card(NINE, DIAMONDS), p.deal_one());
    ASSERT_EQUAL(Card(NINE, HEARTS), p.deal_one());
    ASSERT_EQUAL(Card(NINE, SPADES), p.deal_one());
    ASSERT_EQUAL(Card(TEN, CLUBS), p.deal_one());
}

TEST(test_deal_one) {
    Pack p;
    for (int s = SPADES; s <= DIAMONDS; s++) {
        for (int r = NINE; r <= ACE; r++) {
            ASSERT_EQUAL(Card(static_cast<Rank>(r), static_cast<Suit>(s)), 
            p.deal_one());
        }
    }
}

TEST(test_reset) {
    Pack p;
    for (int i = 0; i < 24; ++i) p.deal_one();

    p.reset();
    ASSERT_FALSE(p.empty());

    for (int i = 0; i < 17; ++i) p.deal_one();
    p.reset();
    ASSERT_EQUAL(Card(NINE, SPADES), p.deal_one());
}

TEST(test_shuffle) {
    Pack p;
    p.shuffle();
    ASSERT_FALSE(Card(NINE, SPADES) == p.deal_one());
}

TEST(test_empty) {
    Pack p;
    
    ASSERT_FALSE(p.empty());

    for (int i = 0; i < 23; ++i) {
    p.deal_one();
    }
    ASSERT_FALSE(p.empty());

    p.deal_one();
    ASSERT_TRUE(p.empty());
}

TEST_MAIN()
