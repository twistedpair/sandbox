#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include "card.hpp"

using namespace std;

#ifndef __CARD_DECK__
#define __CARD_DECK__

class CardDeck {

public:

  CardDeck(int num_decks) {
    add_cards(num_decks);
    //srand(time(NULL));
    shuffle();
    next_card = 0;
  }

  Card* deal_card(Card::card_state_enum state) {
    cards[next_card].set_card_state(state);
    return &cards[next_card++];
  }

protected:

  // collection of cards
  vector<Card> cards;

  // next card to be dealt
  int next_card;

  void add_cards(int num_decks) {
    for (int deck_i = 0; deck_i < num_decks; deck_i++) {
      add_suit_of_deck('S');
      add_suit_of_deck('D');
      add_suit_of_deck('C');
      add_suit_of_deck('H');
    }
  }

  void add_suit_of_deck(char s) {
    for (int i = 1; i < 14; i++) {
      cards.push_back(Card(s,i));
    }
  }

  void print() {
    cout << " Printing cards " << endl;
    for (int i = 0; i < cards.size(); i++)
      cards[i].print();
    cout << " Done printing cards " << endl;
  }

  void shuffle() {
    int num_cards = cards.size();
    for (int i = 0; i < num_cards; i++){
      int j = rand() % num_cards;
      Card tmp = cards[i];
      cards[i] = cards[j];
    }
  }

};

#endif