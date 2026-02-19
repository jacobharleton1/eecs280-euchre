#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

// Add more tests here

TEST(test_make_trump_round1_orders_with_two_face_or_ace_trumps){
    Player * alice = Player_factory("Alice", "Simple");

    Card upcard(TEN, HEARTS);

    alice->add_card(Card(QUEEN, HEARTS));
    alice->add_card(Card(ACE, HEARTS));

    alice->add_card(Card(TEN, CLUBS));
    alice->add_card(Card(TEN, DIAMONDS));
    alice->add_card(Card(NINE, CLUBS));

    Suit order_up_suit = CLUBS;
    bool ordered = alice->make_trump(upcard, false, 1, order_up_suit);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(HEARTS, order_up_suit);
    delete alice;
}

TEST(test_make_trump_round1_pass_with_one_good_trump){
    Player * alice = Player_factory("Alice", "Simple");

    Card upcard(TEN, CLUBS);

    alice->add_card(Card(QUEEN, CLUBS));

    alice->add_card(Card(ACE, HEARTS));
    alice->add_card(Card(TEN, HEARTS));
    alice->add_card(Card(TEN, DIAMONDS));
    alice->add_card(Card(NINE, DIAMONDS));

    Suit order_up_suit = DIAMONDS;
    bool ordered = alice->make_trump(upcard, false, 1, order_up_suit);

    ASSERT_FALSE(ordered);
    ASSERT_EQUAL(DIAMONDS, order_up_suit);
    delete alice;
}

TEST(test_make_trump_round1_multiple_good_trumps){
    Player * alice = Player_factory("Alice", "Simple");

    Card upcard(NINE, HEARTS);

    alice->add_card(Card(JACK, DIAMONDS));
    alice->add_card(Card(KING, HEARTS));
    
    alice->add_card(Card(TEN, HEARTS));
    alice->add_card(Card(TEN, CLUBS));
    alice->add_card(Card(NINE, CLUBS));

    Suit order_up_suit = SPADES;
    bool ordered = alice->make_trump(upcard, false, 1, order_up_suit);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(HEARTS, order_up_suit);
    delete alice;
}

TEST(test_make_trump_round2_same_color){
    Player * alice = Player_factory("Alice", "Simple");

    Card upcard(NINE, SPADES);

    alice->add_card(Card(ACE, CLUBS));

    alice->add_card(Card(NINE, HEARTS));
    alice->add_card(Card(NINE, DIAMONDS));
    alice->add_card(Card(TEN, DIAMONDS));
    alice->add_card(Card(TEN, HEARTS));

    Suit order_up_suit = HEARTS;
    bool ordered = alice->make_trump(upcard, false, 2, order_up_suit);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(CLUBS, order_up_suit);
    delete alice;
}

TEST(test_make_trump_round2_dealer_forced_no_good_trumps){
    Player * alice = Player_factory("Alice", "Simple");
    Card upcard(TEN, DIAMONDS);

    alice->add_card(Card(NINE, CLUBS));
    alice->add_card(Card(TEN, CLUBS));
    alice->add_card(Card(TEN, SPADES));
    alice->add_card(Card(TEN, HEARTS));
    alice->add_card(Card(NINE, DIAMONDS));

    Suit order_up_suit = CLUBS;
    bool ordered = alice->make_trump(upcard, true, 2, order_up_suit);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(HEARTS, order_up_suit);
    delete alice;
}


TEST(test_add_and_discard_removes_lowest_card){
    Player * alice = Player_factory("Alice", "Simple");

    Card upcard(TEN, CLUBS);

    alice->add_card(Card(QUEEN, CLUBS));
    alice->add_card(Card(ACE, HEARTS));
    alice->add_card(Card(TEN, HEARTS));
    alice->add_card(Card(KING, DIAMONDS));
    alice->add_card(Card(NINE, SPADES));

    alice->add_and_discard(upcard);

    bool card_present = false;
    for(int i = 0; i<5;++i){
        Card c = alice->lead_card(CLUBS);
        if(c == Card(NINE, SPADES)){
            card_present = true;
        }
    }

    ASSERT_FALSE(card_present);
    delete alice;
}

TEST(test_add_and_discard_removes_upcard){
        Player * alice = Player_factory("Alice", "Simple");

    Card upcard(NINE, CLUBS);

    alice->add_card(Card(QUEEN, CLUBS));
    alice->add_card(Card(ACE, HEARTS));
    alice->add_card(Card(TEN, HEARTS));
    alice->add_card(Card(KING, DIAMONDS));
    alice->add_card(Card(TEN, SPADES));

    alice->add_and_discard(upcard);

    bool card_present = false;
    for(int i = 0; i<5;++i){
        Card c = alice->lead_card(DIAMONDS);
        if(c == upcard){
            card_present = true;
        }
    }

    ASSERT_FALSE(card_present);
    delete alice;
}

TEST(test_lead_chooses_top_non_trump_over_any_trump){
    Player * alice = Player_factory("Alice", "Simple");

    Suit trump = DIAMONDS;

    alice->add_card(Card(ACE, SPADES));
    alice->add_card(Card(KING, CLUBS));
    alice->add_card(Card(NINE, HEARTS));
    alice->add_card(Card(ACE, DIAMONDS));
    alice->add_card(Card(QUEEN, DIAMONDS));

    Card led = alice->lead_card(trump);
    ASSERT_EQUAL(Card(ACE, SPADES), led);
    delete alice;
}

TEST(test_lead_all_trump_chooses_right_bower){
    Player * alice = Player_factory("Alice", "Simple");

    Suit trump = SPADES;

    alice->add_card(Card(ACE, SPADES));
    alice->add_card(Card(KING, SPADES));
    alice->add_card(Card(JACK, SPADES));
    alice->add_card(Card(JACK, CLUBS));
    alice->add_card(Card(NINE, SPADES));

    Card led = alice->lead_card(trump);
    ASSERT_EQUAL(Card(JACK, SPADES), led);
    delete alice;
}

TEST(test_play_follows_led_suit_with_highest_suited){
    Player * alice = Player_factory("Alice", "Simple");

    Suit trump = HEARTS;
    Card led(TEN,SPADES);

    alice->add_card(Card(KING, SPADES));
    alice->add_card(Card(KING, CLUBS));
    alice->add_card(Card(NINE, SPADES));
    alice->add_card(Card(ACE, DIAMONDS));
    alice->add_card(Card(QUEEN, DIAMONDS));

    Card played = alice->play_card(led, trump);
    ASSERT_EQUAL(Card(KING, SPADES), played);
    delete alice;
}

TEST(test_play_use_lowest_card_with_no_lead_suit){
    Player * alice = Player_factory("Alice", "Simple");

    Suit trump = CLUBS;
    Card led(TEN,HEARTS);

    alice->add_card(Card(KING, SPADES));
    alice->add_card(Card(NINE, CLUBS));
    alice->add_card(Card(TEN, SPADES));
    alice->add_card(Card(ACE, DIAMONDS));
    alice->add_card(Card(QUEEN, DIAMONDS));

    Card played = alice->play_card(led, trump);
    ASSERT_EQUAL(Card(TEN, SPADES), played);
    delete alice;
}

TEST(test_play_led_card_is_left_bower){
    Player * alice = Player_factory("Alice", "Simple");

    Suit trump = HEARTS;
    Card led(JACK,DIAMONDS);

    alice->add_card(Card(ACE, HEARTS));
    alice->add_card(Card(ACE, SPADES));
    alice->add_card(Card(KING, SPADES));
    alice->add_card(Card(QUEEN, SPADES));
    alice->add_card(Card(TEN, SPADES));

    Card played = alice->play_card(led, trump);
    ASSERT_EQUAL(Card(ACE, HEARTS), played);
    delete alice;
}

TEST_MAIN()
