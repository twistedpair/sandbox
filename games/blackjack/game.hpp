#include <iostream>
#include "card_deck.hpp"
#include "card.hpp"
#include "player.hpp"

using namespace std;

#ifndef __GAME__
#define __GAME__

class Game {

public:

  enum hand_result_enum {
    PLAYER_WINS,
    DEALER_WINS,
    PUSH,        // aka tie
    UNDETERMINED // not finished
  };

  Game(int num_decks) {
    card_deck = new CardDeck(num_decks);
    player = new Player(Player::PLAYER);
    dealer = new Player(Player::DEALER);
  }

  ~Game() {
    delete card_deck;
    delete player;
    delete dealer;
  }
  
  void play_new_hand() {

    if (card_deck->get_num_cards_left() < 10) {
      card_deck->shuffle();
    }

    int bet = player->place_bet();
    hand_result_enum result;
    
    deal_initial_cards();
    
    result = check_blackjack();
    if (result != Game::UNDETERMINED) {
      handle_blackjack_result(result, bet);
      discard_cards();
      return;
    }
    
    player_turn();
    if (player->get_hand_value() > 21) {
      handle_player_bust(bet);
      discard_cards();
      return;
    }
    dealer_turn();
    result = get_result();

    handle_result(result, bet);
    discard_cards();
  }

  int get_player_chip_count() {
    player->get_chip_count();
  }

protected:

  Player *player;
  Player *dealer;
  CardDeck *card_deck;

  void deal_initial_cards() {
    
    player->receive_card(card_deck->deal_card(Card::PLAYER_FACE_UP));
    player->receive_card(card_deck->deal_card(Card::PLAYER_FACE_UP));

    dealer->receive_card(card_deck->deal_card(Card::DEALER_FACE_DOWN));
    dealer->receive_card(card_deck->deal_card(Card::DEALER_FACE_UP));
  }
  
  hand_result_enum check_blackjack() {

    bool player_has_blackjack = player->has_blackjack();
    bool dealer_has_blackjack = dealer->has_blackjack();

    if (player_has_blackjack && dealer_has_blackjack)
      return Game::PUSH;
    else if (player_has_blackjack && !dealer_has_blackjack)
      return Game::PLAYER_WINS;
    else if (!player_has_blackjack && dealer_has_blackjack)
      return Game::DEALER_WINS;
    else
      return Game::UNDETERMINED;
  }

  void print_table() {
    cout << "You have:        ";
    player->print_cards();
    cout << "The dealer has:  ";
    dealer->print_cards();
  }

  void player_turn(){

    bool hit_card = true;

    while(player->get_hand_value() < 21 && hit_card) {
      char input = ' ';
      while (input != 'y' && input != 'n') {
	print_table();
	cout << "Do you want to hit? (y/n)" << endl;
	cin >> input;
      }
      if (input == 'y')
	player->receive_card(card_deck->deal_card(Card::PLAYER_FACE_UP));
      else
	hit_card = false;
    }
  }

  void dealer_turn(){
    dealer->flip_hole_card();
    cout << "The dealer has:" << endl;
    dealer->print_cards();
  
    while(dealer->get_hand_value() < 17) {
      cout << "Dealer hits!" << endl;
      dealer->receive_card(card_deck->deal_card(Card::DEALER_FACE_UP));
      cout << "The dealer has:" << endl;
      dealer->print_cards();
    }
  }

  Game::hand_result_enum get_result() {
    int player_hand_value = player->get_hand_value();
    int dealer_hand_value = dealer->get_hand_value();
    if (player_hand_value > dealer_hand_value || dealer_hand_value > 21) 
      return Game::PLAYER_WINS;
    else if (player_hand_value < dealer_hand_value || player_hand_value > 21) 
      return Game::DEALER_WINS;
    else if (player_hand_value == dealer_hand_value)
      return Game::PUSH;
  }

  void discard_cards() {
    player->discard_cards();
    dealer->discard_cards();
  }

  // have a few different handling of results to make print outs
  // different and more entertaining

  void handle_blackjack_result(Game::hand_result_enum result, int bet) {

    dealer->flip_hole_card();
    
    print_table();
    
    switch (result){
    case Game::DEALER_WINS: 
      cout << "Dealer has blackjack and wins " << bet << " chips" << endl;
      return;
    case Game::PLAYER_WINS:
      cout << "Blackjack! You win " << bet * 2 << " chips" << endl;
      player->receive_chips(bet * 2);
      return;
    case Game::PUSH:
      cout << "Double Blackjack: Push.  You get back " << bet << " chips" << endl;
      player->receive_chips(bet);
      return;
    }
  }

  void handle_result(Game::hand_result_enum result, int bet) {
    
    print_table();
    
    switch (result){
    case Game::DEALER_WINS: 
      cout << "Dealer wins " << bet << " chips" << endl;
      return;
    case Game::PLAYER_WINS:
      cout << "You win! " << bet * 2 << " chips" << endl;
      player->receive_chips(bet * 2);
      return;
    case Game::PUSH:
      cout << "Push.  You get back " << bet << " chips" << endl;
      player->receive_chips(bet);
      return;
    }
  }



  void handle_player_bust(int bet) {

    cout << "Bust! You had:" << endl;
    player->print_cards();
    cout << endl;
    dealer->flip_hole_card();
    cout << "The dealer reveals that he had:" << endl;
    dealer->print_cards();
    cout << endl;
    cout << "The dealer wins " << bet << " chips" << endl;
  }
  
};

#endif
