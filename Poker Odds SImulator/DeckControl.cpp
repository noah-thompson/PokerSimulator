
#include <iostream>
#include <vector>
#include <algorithm>

struct Card
{
	Card()
	{
		value = 'Y';
		suit = "Y";
		rank = 0;
	}
	char value;
	std::string suit;
	int rank;
};

struct Hand
{
	std::vector<Card> cards;
};

struct Deck
{
	std::vector<Card> deck;
	char values[13] = { 'A','2','3','4','5','6','7','8','9','T','J','Q','K' };
	std::string suits[4] = { "Hearts", "Diamonds", "Spades", "Clubs" };

	Deck()
	{
		Card card = Card();
		for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
		{
			for (int j = 0; j < sizeof(suits) / sizeof(suits[0]); j++)
			{
				card.value = values[i];
				card.suit = suits[j];
				card.rank = i;
				if (card.rank == 0)
				{
					card.rank = 13;
				}
				deck.push_back(card);
			}
		}
		Shuffle();
	}

	void Shuffle()
	{
		int size = deck.size();
		for (int i = 0; i < size - 1; i++) {
			int j = i + rand() % (size - i);
			std::swap(deck[i], deck[j]);
		}
	}

};

struct Player
{
	Hand hand;
	std::string handName;
	int rank = 8;
	Card played[5];
	int place = 1;
};

struct HandDefinitions
{
	std::string handNames[9] = { "Straight Flush", "Four of a Kind", "Full House", "Flush", "Straight", "Three of a Kind", "Two Pair", "Pair", "High Card" };

	void ResetPlayed(Player& player)
	{
		for (int i = 0; i < 5; i++)
		{
			player.played[i] = Card();
		}
	}


	bool isFlush(Player& player)
	{
		int heartCount = 0;
		int diamondCount = 0;
		int spadeCount = 0;
		int clubCount = 0;
		bool isFlush = false;
		int playedCounted = 0;

		for (int i = 0; i < player.hand.cards.size(); i++)
		{
			std::string suit = player.hand.cards[i].suit;
			if (suit == "Hearts")
			{
				heartCount++;
			}
			else if (suit == "Diamonds")
			{
				diamondCount++;
			}
			else if (suit == "Spades")
			{
				spadeCount++;
			}
			else if (suit == "Clubs")
			{
				clubCount++;
			}
			if (heartCount == 5 || diamondCount == 5 || spadeCount == 5 || clubCount == 5)
			{
				isFlush = true;
				break;
			}
		}


		if (isFlush)
		{
			std::string flushSuit;
			if (heartCount == 5)
			{
				flushSuit = "Hearts";
			}
			else if (diamondCount == 5)
			{
				flushSuit = "Diamonds";
			}
			else if (spadeCount == 5)
			{
				flushSuit = "Spades";
			}
			else if (clubCount == 5)
			{
				flushSuit = "Clubs";
			}

			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				if (player.hand.cards[i].suit == flushSuit)
				{
					if (playedCounted < 5)
					{
						player.played[playedCounted] = player.hand.cards[i];
						playedCounted++;
					}
				}
			}
		}
		return isFlush;
	}

	bool isStraight(Player& player)
	{
		int inARow = 0;
		bool isStraight = false;
		int straightAt = -1;
		bool has2 = false;
		bool has3 = false;
		bool has4 = false;
		bool has5 = false;
		bool lowAce = false;
		int playedCount = 0;

		for (int i = 0; i < player.hand.cards.size()-1; i++)
		{
			if (player.hand.cards[i].rank == player.hand.cards[i + 1].rank - 1)
			{
				inARow++;
			}
			else if (player.hand.cards[i].rank == player.hand.cards[i + 1].rank)
			{
				
			}
			else
			{
				inARow = 0;
			}
			if (inARow == 5)
			{
				isStraight = true;
				straightAt = i;
			}
		}

		if (player.hand.cards[0].value == 'A')
		{
			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				if (player.hand.cards[i].value == '2')
				{
					has2 = true;
				}
				if (player.hand.cards[i].value == '3')
				{
					has3 = true;
				}
				if (player.hand.cards[i].value == '4')
				{
					has4 = true;
				}
				if (player.hand.cards[i].value == '5')
				{
					has5 = true;
				}
			}
			if (has2 && has3 && has4 && has5)
			{
				isStraight = true;
				lowAce = true;
				straightAt = player.hand.cards[player.hand.cards.size() - 1].rank;
			}

			if (isStraight)
			{
				for (int i = 0; i < player.hand.cards.size(); i++)
				{
					if (lowAce)
					{
						if (has2)
						{
							if (player.hand.cards[i].rank == 3)
							{
								player.played[playedCount] = player.hand.cards[i];
								has2 = false;
							}
						}
						if (has3)
						{
							if (player.hand.cards[i].rank == 4)
							{
								player.played[playedCount] = player.hand.cards[i];
								has3 = false;
							}
						}
						if (has4)
						{
							if (player.hand.cards[i].rank == 5)
							{
								player.played[playedCount] = player.hand.cards[i];
								has4 = false;
							}
						}
						if (has5)
						{
							if (player.hand.cards[i].rank == 6)
							{
								player.played[playedCount] = player.hand.cards[i];
								has5 = false;
							}
						}
					}
					else
					{
						if (i == straightAt)
						{
							player.played[playedCount] = player.hand.cards[i];
							playedCount++;
						}
						else if (playedCount < 5)
						{
							if (player.played[playedCount - 1].rank == player.hand.cards[i].rank + 1)
							{
								player.played[playedCount] = player.hand.cards[i];
								playedCount++;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		
		return isStraight;
	}

	bool isStraightFlush(Player& player)
	{
		return isStraight(player) && isFlush(player);
	}

	bool isPair(Player& player)
	{
		bool pairFound = false;
		int playedCounted = 0;
		bool cardAdded = false;

		for (int i = 0; i < player.hand.cards.size() - 1; i++)
		{
			for (int j = i + 1; j < player.hand.cards.size(); j++)
			{
				if (player.hand.cards[i].value == player.hand.cards[j].value)
				{
					pairFound = true;
					player.played[playedCounted] = player.hand.cards[i];
					playedCounted++;
					player.played[playedCounted] = player.hand.cards[j];
					playedCounted++;
				}
			}
		}

		if (pairFound)
		{
			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (player.hand.cards[i].value == player.played[j].value && player.hand.cards[i].suit == player.played[j].suit)
					{
						cardAdded = true;
					}
				}
				if (!cardAdded)
				{
					player.played[playedCounted] = player.hand.cards[i];
					playedCounted++;
				}
				if (playedCounted > 4)
				{
					break;
				}
				cardAdded = false;
			}
		}
		return pairFound;
	}

	bool isThreeOfAKind(Player& player)
	{
		int count = 1;
		bool isThree = false;
		int playedCount = 0;
		char threeValue = '0';
		bool cardAdded = false;

		for (int i = 0; i < player.hand.cards.size() - 2; i++)
		{
			for (int j = i + 1; j < player.hand.cards.size(); j++)
			{
				if (player.hand.cards[i].value == player.hand.cards[j].value)
				{
					count++;
				}
			}
			if (count == 3)
			{
				threeValue = player.hand.cards[i].value;
				isThree = true;
				break;
			}
			count = 1;
		}
		if (isThree)
		{
			for (int i =0; i < player.hand.cards.size();i++)
			{
				if (player.hand.cards[i].value == threeValue)
				{
					player.played[playedCount] = player.hand.cards[i];
					playedCount++;
				}
			}

			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				while (player.hand.cards[i].value == threeValue)
				{
					i++;
				}

				player.played[playedCount] = player.hand.cards[i];
				playedCount++;

				if (playedCount > 4)
				{
					break;
				}
				cardAdded = false;
			}
		}
		return isThree;
	}

	bool isFourOfAKind(Player& player)
	{
		int count = 1;
		bool isFour = false;
		int playedCount = 0;
		char fourValue = '0';
		bool cardAdded = false;

		for (int i = 0; i < player.hand.cards.size() - 3; i++)
		{
			for (int j = i + 1; j < player.hand.cards.size(); j++)
			{
				if (player.hand.cards[i].value == player.hand.cards[j].value)
				{
					count++;
				}
			}
			if (count >= 4)
			{
				isFour = true;
				fourValue = player.hand.cards[i].value;
			}
			count = 1;
		}

		if (isFour)
		{
			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				if (player.hand.cards[i].value == fourValue)
				{
					if (playedCount < 5)
					{
						player.played[playedCount] = player.hand.cards[i];
						playedCount++;
					}
				}
			}

			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (player.hand.cards[i].value == player.played[j].value && player.hand.cards[i].suit == player.played[j].suit)
					{
						cardAdded = true;
					}
				}
				if (!cardAdded)
				{
					if (playedCount < 5)
					{
						player.played[playedCount] = player.hand.cards[i];
						playedCount++;
					}
				}
				if (playedCount > 4)
				{
					break;
				}
				cardAdded = false;
			}
		}

		return isFour;
	}

	bool isTwoPair(Player& player)
	{
		bool onePair = false;
		char pairOneValue = '0';
		bool twoPair = false;
		int playedCount = 0;
		bool cardAdded = false;
		int indexToAdd;

		for (int i = 0; i < player.hand.cards.size() - 1; i++)
		{
			while (player.hand.cards[i].value == pairOneValue)
			{
				i ++;
			}
			for (int j = i+1; j < player.hand.cards.size(); j++)
			{
				if (player.hand.cards[i].value == player.hand.cards[j].value)
				{
					if (onePair)
					{
						twoPair = true;
					}
					else
					{
						onePair = true;
						pairOneValue = player.hand.cards[i].value;
						
					}
					player.played[playedCount] = player.hand.cards[i];
					playedCount++;
					player.played[playedCount] = player.hand.cards[j];
					playedCount++;
				}
			}
			if (twoPair)
			{
				break;
			}
		}
		if (twoPair)
		{
			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (player.hand.cards[i].value == player.played[j].value && player.hand.cards[i].suit == player.played[j].suit)
					{
						cardAdded = true;
					}
				}
				if (!cardAdded)
				{
					player.played[playedCount] = player.hand.cards[i];
					break;
				}
				cardAdded = false;
			}
		}

		return twoPair;
	}

	bool isFullHouse(Player& player)
	{
		bool trio = false;
		char trioValue = '0';
		bool endedList = false;
		bool fullHouse = false;
		int count = 1;
		int playedCount = 0;
		bool cardAdded = false;

		for (int i = 0; i < player.hand.cards.size() - 2; i++)
		{
			for (int j = i + 1; j < player.hand.cards.size(); j++)
			{
				if (player.hand.cards[i].value == player.hand.cards[j].value)
				{
					count++;
				}
			}
			if (count > 2)
			{
				trio = true;
				trioValue = player.hand.cards[i].value;
				break;
			}
			count = 1;
		}
		if (trio)
		{

			for (int i = 0; i < player.hand.cards.size() - 1; i++)
			{
				for (int j = i + 1; j < player.hand.cards.size(); j++)
				{
					if (player.hand.cards[i].value != trioValue)
					{
						if (player.hand.cards[i].value == player.hand.cards[j].value && player.hand.cards[i].value != trioValue)
						{
							fullHouse = true;
							player.played[playedCount] = player.hand.cards[i];
							playedCount++;
							player.played[playedCount] = player.hand.cards[j];
							playedCount++;
						}
					}
				}
				if (fullHouse)
				{
					break;
				}
			}
		}
		if (fullHouse)
		{
			for (int i = 0; i < player.hand.cards.size(); i++)
			{
				if (playedCount > 5)
				{
					break;
				}
				else
				{
					if (player.hand.cards[i].value == trioValue)
					{
						if (playedCount < 5)
						{
							player.played[playedCount] = player.hand.cards[i];
							playedCount++;
						}
					}
				}
			}
		}
		return fullHouse;
	}

	void HighCardPlayed(Player &player)
	{
		for (int i = 0; i < player.hand.cards.size(); i++)
		{
			if (i < 5)
				player.played[i] = player.hand.cards[i];
		}
	}

	void OrderCards(Player& player)
	{
		for (int i = 0; i < player.hand.cards.size(); i++)
		{
			for (int j = i + 1; j < player.hand.cards.size(); j++)
			{
				if (player.hand.cards[i].rank < player.hand.cards[j].rank)
				{
					std::swap(player.hand.cards[i], player.hand.cards[j]);
				}
			}
		}
	}
	
	void CardsUsed(Player &player)
	{
		for (int i = 0; i < 5; i++)
		{
			std::cout << player.played[i].value << " of " << player.played[i].suit << std::endl;
		}
	}

	std::string ReturnBestHand(Player& player)
	{
		ResetPlayed(player);

		if (isStraightFlush(player))
		{
			player.rank = 0;
			return handNames[0];
		}
		else if (isFourOfAKind(player))
		{
			player.rank = 1;
			return handNames[1];
		}
		else if (isFullHouse(player))
		{
			player.rank = 2;
			return handNames[2];
		}
		else if (isFlush(player))
		{
			player.rank = 3;
			return handNames[3];
		}
		else if (isStraight(player))
		{
			player.rank = 4;
			return handNames[4];
		}
		else if (isThreeOfAKind(player))
		{
			player.rank = 5;
			return handNames[5];
		}
		else if (isTwoPair(player))
		{
			player.rank = 6;
			return handNames[6];
		}
		else if (isPair(player))
		{
			player.rank = 7;
			return handNames[7];
		}
		else
		{
			HighCardPlayed(player);
			player.rank = 8;
			return handNames[8];
		}
	}

	//returns 1 if player 1 is better 2 if player 2 is better and 0 if even
	int GetBestStraightFlush(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank > player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}

	int GetBestFourOfAKind(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank > player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}

	int GetBestFullHouse(Player& player1, Player& player2)
	{
		if (player1.played[4].rank > player2.played[4].rank)
		{
			return 1;
		}
		else if (player1.played[4].rank < player2.played[4].rank)
		{
			return 2;
		}
		else if (player1.played[0].rank > player2.played[0].rank)
		{
			return 1;
		}
		else if (player1.played[0].rank < player2.played[0].rank)
		{
			return 2;
		}
		return 0;
	}

	int GetBestFlush(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank < player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}

	int GetBestStraight(Player& player1, Player& player2)
	{
		if (player1.played[1].rank > player2.played[1].rank)
		{
			return 1;
		}
		else if (player1.played[1].rank < player2.played[1].rank)
		{
			return 2;
		}
		return 0;
	}

	int GetBestThreeOfAKind(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank < player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}

	int GetBestTwoPair(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank < player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}

	int GetBestPair(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank < player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}

	int GetBestHighCard(Player& player1, Player& player2)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player1.played[i].rank > player2.played[i].rank)
			{
				return 1;
			}
			else if (player1.played[i].rank < player2.played[i].rank)
			{
				return 2;
			}
		}
		return 0;
	}





	void DetermineWinner(std::vector<Player> &players)
	{
		int result = -1;
		for (int i = 0; i < players.size()-1; i++)
		{
			for (int j = i + 1; j < players.size(); j++)
			{
				if (players[i].rank < players[j].rank)
				{
					players[j].place++;
				}
				else if(players[i].rank > players[j].rank)
				{
					players[i].place++;
				}
				else
				{
					if (players[i].rank == 0)
					{
						result = GetBestStraightFlush(players[i], players[j]);
					}
					else if (players[i].rank == 1)
					{
						result = GetBestFourOfAKind(players[i], players[j]);
					}
					else if (players[i].rank == 2)
					{
						result = GetBestFullHouse(players[i], players[j]);
					}
					else if (players[i].rank == 3)
					{
						result = GetBestFlush(players[i], players[j]);
					}
					else if (players[i].rank == 4)
					{
						result = GetBestStraight(players[i], players[j]);
					}
					else if (players[i].rank == 5)
					{
						result = GetBestThreeOfAKind(players[i], players[j]);
					}
					else if (players[i].rank == 6)
					{
						result = GetBestTwoPair(players[i], players[j]);
					}
					else if (players[i].rank == 7)
					{
						result = GetBestPair(players[i], players[j]);
					}
					else if (players[i].rank == 8)
					{
						result = GetBestHighCard(players[i], players[j]);
					}

					if (result == 1)
					{
						players[j].place++;
					}
					else if (result == 2)
					{
						players[i].place++;
					}
				}
			}
		}
	}

	void PrintWinner(std::vector<Player>& players)
	{
		DetermineWinner(players);

		for (int i = 0; i < players.size(); i++)
		{
			std::cout << "PLayer " << i + 1 << " Place: " << players[i].place << std::endl;
		}
	}
};

struct GameManager
{
	std::vector<Player> players;
	int minPlayerCount = 1;
	int maxPlayerCount = 8;

	Card Draw(Deck &deck)
	{	
		Card card = deck.deck[deck.deck.size() - 1];
		deck.deck.pop_back();
		return card;
	}

	Hand Deal(Deck& deck)
	{
		Hand hand;
		hand.cards.push_back(Draw(deck));
		hand.cards.push_back(Draw(deck));
		return hand;
	}

	void DealAllPlayers(Deck& deck)
	{
		int playerCount = 0;
		do
		{
			std::cout << "How many players are there: ";
			std::cin >> playerCount;
			if (playerCount < minPlayerCount)
			{
				std::cout << "Not enough players!" << std::endl;
			}if (playerCount > maxPlayerCount)
			{
				std::cout << "Too many players!" << std::endl;
			}
		} while (playerCount < minPlayerCount || playerCount > maxPlayerCount);

		for (int i = 0; i < playerCount; i++)
		{
			Player player;
			Hand hand;
			hand = Deal(deck);
			player.hand = hand;
			players.push_back(player);
		}
		for (int i = 0; i < playerCount; i++)
		{
			std::cout << "Player " << i+1 << std::endl;
			std::cout << std::endl;
			std::cout << players[i].hand.cards[0].value << " of " << players[i].hand.cards[0].suit << std::endl;
			std::cout << players[i].hand.cards[1].value << " of " << players[i].hand.cards[1].suit << std::endl;
		}
		std::cout << std::endl;
	}

	void DealCommunityCard(Deck &deck)
	{
		Card card = deck.deck[deck.deck.size() - 1];
		for (int i = 0; i < players.size(); i++)
		{
			players[i].hand.cards.push_back(card);
		}
		deck.deck.pop_back();
		std::cout << "The card was " << card.value << " of " << card.suit << std::endl;
	}

	void DealSpecificCommunityCard(std::vector<Player> &playerList,Card & card)
	{
		for (int i = 0; i < playerList.size(); i++)
		{
			playerList[i].hand.cards.push_back(card);
		}
	}

	void DealFlop(Deck& deck)
	{
		deck.deck.pop_back();
		DealCommunityCard(deck);
		DealCommunityCard(deck);
		DealCommunityCard(deck);
	}
	void DealTurnOrRiver(Deck& deck)
	{
		deck.deck.pop_back();
		DealCommunityCard(deck);
	}

	void GetAndPrintCurrentHands(HandDefinitions &def)
	{ 
		for (int i = 0; i < players.size(); i++)
		{
			def.OrderCards(players[i]);
			players[i].handName = def.ReturnBestHand(players[i]);
			std::cout << "The best hand for player " << i + 1 << " is " << players[i].handName << std::endl;
		}
	}


	void GetAndPrintCurrentHands(std::vector<Player> &playerList, HandDefinitions& def)
	{
		for (int i = 0; i < playerList.size(); i++)
		{
			def.OrderCards(playerList[i]);
			playerList[i].handName = def.ReturnBestHand(playerList[i]);
			std::cout << "The best hand for player " << i + 1 << " is " << playerList[i].handName << std::endl;
		}
	}

	void PrintCardsPlayed(HandDefinitions& def)
	{
		for (int i = 0; i < players.size(); i++)
		{
			std::cout << "Player " << i + 1 << std::endl;
			def.CardsUsed(players[i]);
		}
	}


	void RunSimulatedHandsPreFlop(Deck& deck, HandDefinitions& def)
	{
		std::vector<Player> playersClone;
		std::vector<int> winnerCount;
		int totalHands = 0;
		
		for (int i = 0; i < players.size(); i++)
		{
			winnerCount.push_back(0);
		}

		for (int i = 0; i < deck.deck.size()-4; i++)
		{
			for (int j = i + 1; j < deck.deck.size() - 3; j++)
			{
				for (int k = j + 1; k < deck.deck.size() - 2; k++)
				{
					for (int l = k + 1; l < deck.deck.size() - 1; l++)
					{
						for (int m = l + 1; m < deck.deck.size(); m++)
						{
							playersClone = players;
							DealSpecificCommunityCard(playersClone, deck.deck[i]);
							DealSpecificCommunityCard(playersClone, deck.deck[j]);
							DealSpecificCommunityCard(playersClone, deck.deck[k]);
							DealSpecificCommunityCard(playersClone, deck.deck[l]);
							DealSpecificCommunityCard(playersClone, deck.deck[m]);

							for (int n = 0; n < playersClone.size();n++)
							{
								def.ReturnBestHand(playersClone[n]);
							}

							def.DetermineWinner(playersClone);
							totalHands++;
							for (int n = 0; n < playersClone.size(); n++)
							{
								if (playersClone[n].place == 1)
								{
									winnerCount[n]++;									
								}
							}

							playersClone.clear();
						}
					}
				}
			}
		}
		for (int i = 0; i < players.size(); i++)
		{
			std::cout << "Player " << i + 1 << " Would win " << winnerCount[i]/(float)totalHands * 100.0f << "% of Hands" << std::endl;
		}
	}

	void RunSimulatedHandsPreTurn(Deck& deck, HandDefinitions& def)
	{
		std::vector<Player> playersClone;
		std::vector<int> winnerCount;
		int totalHands = 0;

		for (int i = 0; i < players.size(); i++)
		{
			winnerCount.push_back(0);
		}

		for (int i = 0; i < deck.deck.size()-1; i++)
		{
			for (int j = i + 1; j < deck.deck.size(); j++)
			{
				playersClone = players;
				DealSpecificCommunityCard(playersClone, deck.deck[i]);
				DealSpecificCommunityCard(playersClone, deck.deck[j]);
			}

			for (int k = 0; k < playersClone.size(); k++)
			{
				def.ReturnBestHand(playersClone[k]);
			}

			def.DetermineWinner(playersClone);
			totalHands++;
			for (int k = 0; k < playersClone.size(); k++)
			{
				if (playersClone[k].place == 1)
				{
					winnerCount[k]++;
				}
			}

			playersClone.clear();
		}

		for (int i = 0; i < players.size(); i++)
		{
			std::cout << "Player " << i + 1 << " Would win " << winnerCount[i] / (float)totalHands * 100.0f << "% of Hands" << std::endl;
		}
	}

	void RunSimulatedHandsPreRiver(Deck& deck, HandDefinitions& def)
	{
		std::vector<Player> playersClone;
		std::vector<int> winnerCount;
		int totalHands = 0;

		for (int i = 0; i < players.size(); i++)
		{
			winnerCount.push_back(0);
		}

		for (int i = 0; i < deck.deck.size(); i++)
		{
			playersClone = players;
			DealSpecificCommunityCard(playersClone, deck.deck[i]);

			for (int j = 0; j < playersClone.size(); j++)
			{
				def.ReturnBestHand(playersClone[j]);
			}

			def.DetermineWinner(playersClone);
			totalHands++;
			for (int j = 0; j < playersClone.size(); j++)
			{
				if (playersClone[j].place == 1)
				{
					winnerCount[j]++;
				}
			}

			playersClone.clear();
		}

		for (int i = 0; i < players.size(); i++)
		{
			std::cout << "Player " << i + 1 << " Would win " << winnerCount[i] / (float)totalHands * 100.0f << "% of Hands" << std::endl;
		}
	}
	void DealWholeGame(Deck &deck, HandDefinitions &def)
	{
		DealAllPlayers(deck);
		RunSimulatedHandsPreFlop(deck, def);
		//GetAndPrintCurrentHands(def);
		DealFlop(deck);
		RunSimulatedHandsPreTurn(deck, def);
		//GetAndPrintCurrentHands(def);
		DealTurnOrRiver(deck);
		RunSimulatedHandsPreRiver(deck, def);
		//GetAndPrintCurrentHands(def);
		DealTurnOrRiver(deck);
		//PrintCardsPlayed(def);
		GetAndPrintCurrentHands(def);
		def.PrintWinner(players);
	}
};


