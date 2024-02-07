
#include <iostream>

#include "DeckControl.cpp"
int main()
{
    srand(time(NULL));
    
    GameManager manager;
    Deck deck = Deck();
    HandDefinitions def;

    manager.DealWholeGame(deck, def);
    
}