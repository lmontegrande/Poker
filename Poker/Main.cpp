#include <iostream>
#include <string>
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif 

using namespace std;

// The Card Struct
struct Card {
	int numericValue;
	int suit; // 1 Heart, 2 Diamond, 3 Spade, 4 Club
};

// The Card Node
struct CardNode {
	CardNode* next;
	Card* card;
};

// The deck arraylist
struct Deck {
	CardNode* head = nullptr;
	int deckSize = 0;
};

// The player
struct Player {
	Card* cards[5]{nullptr, nullptr , nullptr , nullptr , nullptr};
	int mulah = 10;
	int numCards = 0;
};

// Function Prototypes
string getUserInput(string);
void addCardToDeck(Deck* deck, Card* cardPtr);
Deck* generateDeck();
void outputDeck(Deck* deck);
string cardStringValue(Card* card);
void drawCard(Player* player, Deck* deck, Deck* discards, bool isSwapMode);
Card* takeCardFromDeck(Deck* deck, Deck* discards, bool isSwapMode);
void play();
void discardCard(Player* player, int cardNum, Deck* deck, Deck* discards);
void outputHand(Player* player, bool beenSwapped[5]);
void SwapCard(Player* player, int cardNum, Deck* deck, Deck* discards, bool isSwapMode);
void checkForWin(Player* player);
bool isStraight(Card* cards[5]);
bool isSameSuit(Card* cards[5]);
void maxMatch(Card* cards[5], int* maxOfAKind, int* pairCount, int* highestPair);
bool isNum(string line);

// Main Loop
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);

	bool isDone = false;

	play();

	// Repeat logic
	/*while (!isDone) {
		play();

		switch (getUserInput("Play Again? (y/n): ")[0])
		{
		case 'y':
			break;
		case 'n':
			isDone = true;
			break;
		default:
			cout << "Invalid Input\n";
			break;
		}
	}*/
}

// Play sequence
void play() {
	bool isDone = false;
	Player player;
	Deck* deck;
	Deck* discards;

	deck = generateDeck();
	discards = new Deck();

	// Round loop
	while (!isDone) {
		string userInput;
		bool validInput = false;
		cout << "Player Mulah: $" << player.mulah << endl;
		while (!validInput) {
			userInput = getUserInput("Take Money and Leave? (y/n): ");
			if (userInput == "y") {
				isDone = true;
				return;
			} else if (userInput == "n") {
				validInput = true;
			} else {
				cout << "INVALID INPUT" << endl;
			}
		}

		// Deduct $1 from player, then give player 5 cards
		player.mulah--;
		for (int x = 0; x < 5; x++) {
			drawCard(&player, deck, discards, false);
		} 

		// Print Hand, Player chooses which cards to keep, Discard cards to dead deck and replace them in hand, print hand
		bool startRound = false;
		while (!startRound) {

			bool beenSwapped[5]{ false, false, false, false, false };
			outputHand(&player, beenSwapped);

			string userCardSwap;
			string newCardsText = "New Cards:";
			bool isSwapMode = false;
			userCardSwap = getUserInput("Which cards do you want to trade? ([*CardLetter* or NONE] or [DECK]): ");
			if (userCardSwap == "DECK") {
				outputDeck(deck);
				continue;
			}
			if (userCardSwap == "NONE") {
				startRound = true;
				continue;
			}
			for (int x = 0; x < userCardSwap.size(); x++) {
				switch (userCardSwap.at(x))
				{
				case '_':
					isSwapMode = true;
					break;
				case 'A':
					if (beenSwapped[0]) {
						continue;
					}
					SwapCard(&player, 0, deck, discards, isSwapMode);
					newCardsText += cardStringValue(player.cards[0]) + ", ";
					beenSwapped[0] = true;
					startRound = true;
					break;
				case 'B':
					if (beenSwapped[1]) {
						continue;
					}
					SwapCard(&player, 1, deck, discards, isSwapMode);
					newCardsText += cardStringValue(player.cards[1]) + ", ";
					beenSwapped[1] = true;
					startRound = true;
					break;
				case 'C':
					if (beenSwapped[2]) {
						continue;
					}
					SwapCard(&player, 2, deck, discards, isSwapMode);
					newCardsText += cardStringValue(player.cards[2]) + ", ";
					beenSwapped[2] = true;
					startRound = true;
					break;
				case 'D':
					if (beenSwapped[3]) {
						continue;
					}
					SwapCard(&player, 3, deck, discards, isSwapMode);
					newCardsText += cardStringValue(player.cards[3]) + ", ";
					beenSwapped[3] = true;
					startRound = true;
					break;
				case 'E':
					if (beenSwapped[4]) {
						continue;
					}
					SwapCard(&player, 4, deck, discards, isSwapMode);
					newCardsText += cardStringValue(player.cards[4]) + ", ";
					beenSwapped[4] = true;
					startRound = true;
					break;
				default:
					cout << "INVALID INPUT: " << userCardSwap[x] << endl;
					continue;
				}
			}

			cout << newCardsText << endl;
			outputHand(&player, beenSwapped);
		}

		// Calculate any win conditions (best priority), output if any or not, award money if any
		checkForWin(&player);

		// Lose Condition
		if (player.mulah <= 0) {
			isDone = true;
		}

		// Clear player hand
		for (int x = 0; x < 5; x++) {
			discardCard(&player, x, deck, discards);
		}

		//dlskfjdsklfjfl
		//outputDeck(deck);
	}

	// Output end message
	cout << "You earned: $" << player.mulah << endl;

	// Delete deck when done
	delete deck;
	delete discards;
}

// Check for a win condition
void checkForWin(Player* player) {
	string results = "JUNK";
	int winnings = 0;
	Card* cards[5];

	// Sort
	cards[0] = player->cards[0];
	for (int x = 1; x < 5; x++) {
		Card* currentCard = player->cards[x];
		for (int y = 0; y <= x; y++) {
			if (y == x) {
				cards[y] = currentCard;
			} else if (currentCard->numericValue < cards[y]->numericValue) {
				Card* tempCard = cards[y];
				cards[y] = currentCard;
				currentCard = tempCard;
			}
		}
	}

	/*Card* cards[5];
	cards[0] = new Card{ 1,1 };
	cards[1] = new Card{ 10,1 };
	cards[2] = new Card{ 11,1 };
	cards[3] = new Card{ 12,1 };
	cards[4] = new Card{ 13,1 }; */

	bool hasStraight = isStraight(cards);
	bool hasSameSuit = isSameSuit(cards);
	int maxOfAKind;
	int pairsCount;
	int highestpair;
	maxMatch(cards, &maxOfAKind, &pairsCount, &highestpair);

	//cout << hasStraight << hasSameSuit << maxOfAKind << pairsCount << highestpair;

	//Royal Flush (800) 
	if (hasStraight && cards[0]->numericValue == 1 && hasSameSuit) {
		winnings = 800;
		results = "ROYAL FLUSH";
	}
	//Straight Flush (50)
	else if (hasStraight && hasSameSuit) {
		winnings = 50;
		results = "STRAIGHT FLUSH";
	} 
	//Four of a kind(25)
	else if (maxOfAKind == 4) {
		winnings = 25;
		results = "FOUR OF A KIND";
	}
	//Full House (9)
	else if (maxOfAKind == 3 && pairsCount == 2) {
		winnings = 9;
		results = "FULL HOUSE";
	}
	//Flush(6)
	else if (hasSameSuit) {
		winnings = 6;
		results = "FLUSH";
	}
	//Straight(4)
	else if (hasStraight) {
		winnings = 4;
		results = "STRAIGHT";
	}
	//ThreeofaKind(3)
	else if (maxOfAKind == 3) {
		winnings = 3;
		results = "THREE OF A KIND";
	}
	//TwoPair (2)
	else if (pairsCount >= 2) {
		winnings = 2;
		results = "2 PAIRS";
	}
	//OnePair, Jacks or Better (1)
	else if (pairsCount == 1 && highestpair >= 11) {
		winnings = 1;
		results = "ROYAL PAIR";
	}
	//OnePair(1)
	else if (pairsCount == 1) {
		winnings = 1;
		results = "PAIR";
	}

	player->mulah += winnings;
	cout << "+$" << winnings << " " << results << endl;

	//delete[] cards;
}

// Check if you have a straight
bool isStraight(Card* cards[5]) {
	int value = cards[0]->numericValue;
	if (cards[0]->numericValue == 1 && cards[1]->numericValue == 10 && cards[2]->numericValue == 11 && cards[3]->numericValue == 12 && cards[4]->numericValue == 13) {
		return true;
	}
	for (int x = 1; x < 5; x++) {
		int newValue = cards[x]->numericValue;
		if (value != (newValue - 1)) {
			return false;
		}
		value = newValue;
	}
	return true;
}

// Check if they have the same suit 
bool isSameSuit(Card* cards[5]) {
	int value = cards[0]->suit;
	for (int x = 1; x < 5; x++) {
		int newValue = cards[x]->suit;
		if (value != newValue) {
			return false;
		}
		value = newValue;
	}
	return true;
}

// Check if you have a pair
void maxMatch(Card* cards[5], int* maxOfAKind, int* pairCount, int* highestPair) {
	*maxOfAKind = 1; 
	*pairCount = 0;
	*highestPair = 0;
	int currentMatch = 1;
	for (int x = 1; x < 5; x++) {
		if (cards[x]->numericValue == cards[x - 1]->numericValue) {
			currentMatch++;
			*maxOfAKind = currentMatch > *maxOfAKind ? currentMatch : *maxOfAKind;
			*pairCount = currentMatch == 2 ? *pairCount + 1 : *pairCount;
			*highestPair = *highestPair < cards[x]->numericValue ? cards[x]->numericValue : *highestPair;
		}
		else {
			currentMatch = 1;
		}
	}
}

// Swap a card
void SwapCard(Player* player, int cardNum, Deck* deck, Deck* discards, bool isSwapMode) {
	discardCard(player, cardNum, deck, discards);
	drawCard(player, deck, discards, isSwapMode);
}

// Discard Card
void discardCard(Player* player, int cardNum, Deck* deck, Deck* discards) {
	Card* card = player->cards[cardNum];
	addCardToDeck(discards, card);
	player->cards[cardNum] = nullptr;
	/*for (int x = cardNum; x < 4; x++) {
		player->cards[x] = player->cards[x + 1];
	}
	player->cards[4] = nullptr;*/
	player->numCards--;
}

// Draws a card to players hand
void drawCard(Player* player, Deck* deck, Deck* discards, bool isSwapMode) {
	// Take card from deck
	Card* drawnCardPtr = takeCardFromDeck(deck, discards, isSwapMode);
	// Put card in player's hand
	int x = 0;
	while (true) {
		if (player->cards[x] == nullptr) {
			player->cards[x] = drawnCardPtr;
			break;
		}
		x++;
	}
	player->numCards++;
}

// Take card from deck, swap to discards if deck is empty
Card* takeCardFromDeck(Deck* deck, Deck* discards, bool isSwapMode) {

	// If deck is empty, swap with discards
	if (deck->deckSize <= 0) {
		CardNode* temp = deck->head;
		deck->head = discards->head;
		discards->head = nullptr;
		deck->deckSize = discards->deckSize;
		discards->deckSize = 0;
	}

	Card* randomCard;
	int randomCardNum;
	if (isSwapMode) {
		bool validInput = false;
		string input;
		while (!validInput) {
			outputDeck(deck);
			input = getUserInput("Take Card #");
			randomCardNum = atoi(input.c_str());
			if ((randomCardNum <= deck->deckSize-1 && randomCardNum >= 0) && isNum(input)) {
				validInput = true;
			} else {
				cout << "INVALID INPUT" << endl;
			}
		}
	} else {
		randomCardNum = (rand() % deck->deckSize);
	}

	deck->deckSize--;
	if (deck->deckSize == 0) {
		randomCard = deck->head->card;
		delete deck->head; 
		return randomCard;
	} 

	if (randomCardNum == 0) {
		CardNode* current;
		current = deck->head;
		deck->head = current->next;
		randomCard = current->card;
		delete current;
	} else {
		CardNode* previous = deck->head;
		for (int x = 1; x < randomCardNum; x++) {
			previous = previous->next;
		}

		CardNode* current = previous->next;
		previous->next = current->next;
		randomCard = current->card;
		delete current;
	}
	
	return randomCard;
}

// Add cardNode to beginning of deck using the passed in cardPtr as the card value
void addCardToDeck(Deck* deck, Card* cardPtr) {
	// Make a card node
	// Add card node to linkedlist
	CardNode* n = new CardNode;
	n->card = cardPtr;
	n->next = deck->head;
	deck->head = n;
	deck->deckSize++;
}

// Function to generate cards, then have the deck link to them
Deck* generateDeck() {
	Deck* deck = new Deck;
	for (int suit = 1; suit <= 4; suit++) {
		for (int numericVal = 1; numericVal <= 13; numericVal++) {
			addCardToDeck(deck, new Card{ numericVal, suit });
		}
	}
	return deck;
}

// Helper function to easily convert a card to it's user facing name
string cardStringValue(Card* card) {
	// 1 Heart, 2 Diamond, 3 Spade, 4 Club
	string suit;
	string num;

	switch (card->numericValue)
	{
	case 1:
		num = "Ace";
		break;
	case 11:
		num = "Jack";
		break;
	case 12:
		num = "Queen";
		break;
	case 13:
		num = "King";
		break;
	default:
		num = to_string(card->numericValue);
		break;
	}

	switch (card->suit)
	{
	case 1:
		suit = "Hearts";
		break;
	case 2:
		suit = "Diamonds";
		break;
	case 3:
		suit = "Spades";
		break;
	case 4:
		suit = "Clubs";
		break;
	default:
		suit = "Error";
		break;
	}

	return num + " of " + suit;
}

// Output deck to command line
void outputDeck(Deck* deck) {
	CardNode* n = deck->head;
	int x = 0;
	cout << "--------------------------" << endl;
	while (n != nullptr) {
		cout << "(#" << x << ") " << cardStringValue(n->card) << ", ";
		n = n->next;
		x++;
	}
	cout << endl << "--------------------------" << endl;
}

// Output hand
void outputHand(Player* player, bool beenSwapped[]) {
	cout << "Hand: " << endl;
	string a = "ABCDE";
	for (int x = 0; x < 5; x++) { 
		cout << a[x] << ": " << cardStringValue(player->cards[x]);
		if (beenSwapped[x]) {
			cout << " [SWAPPED] ";
		}
		cout << endl;
	}
}

// Helper function for outputting a message and getting back a string
string getUserInput(string message) {
	string userInput = "";
	cout << message;
	cin >> userInput;

	cin.clear();
	cin.ignore(10000, '\n');

	return userInput;
}

// Check if string is a number
bool isNum(string line)
{
	if (line.empty() || ((!isdigit(line[0])) && (line[0] != '-') && (line[0] != '+'))) return false;

	char * p;
	strtol(line.c_str(), &p, 10);

	return (*p == 0);
}