#include <iostream>
#include <string>

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
void drawCard(Player* player, Deck* deck, Deck* discards);
Card* takeCardFromDeck(Deck* deck, Deck* discards);
void play();
void discardCard(Player* player, int cardNum, Deck* deck, Deck* discards);
void outputHand(Player* player);
void SwapCard(Player* player, int cardNum, Deck* deck, Deck* discards);
void checkForWin(Player* player);

// Main Loop
int main() {
	bool isDone = false;

	while (!isDone) {
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
	}
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
		cout << "Player Mulah: $" << player.mulah << endl;
		userInput = getUserInput("Take Money and Leave? (y/n): ");
		if (userInput == "y") {
			isDone = true;
			continue;
		}

		// Deduct $1 from player, then give player 5 cards, print hand
		player.mulah--;
		for (int x = 0; x < 5; x++) {
			drawCard(&player, deck, discards);
		}

		outputHand(&player);

		// Player chooses which cards to keep, Discard cards to dead deck and replace them in hand, print hand
		string userCardSwap;
		string newCardsText = "New Cards:";
		cout << "Which cards do you want to trade?: ";
		cin >> userCardSwap;
		for (int x = 0; x < userCardSwap.size(); x++) {
			switch (userCardSwap.at(x))
			{
			case 'A':
				SwapCard(&player, 0, deck, discards);
				newCardsText += cardStringValue(player.cards[4]) + ":";
				break;
			case 'B':
				SwapCard(&player, 1, deck, discards);
				newCardsText += cardStringValue(player.cards[4]) + ":";
				break;
			case 'C':
				SwapCard(&player, 2, deck, discards);
				newCardsText += cardStringValue(player.cards[4]) + ":";
				break;
			case 'D':
				SwapCard(&player, 3, deck, discards);
				newCardsText += cardStringValue(player.cards[4]) + ":";
				break;
			case 'E':
				SwapCard(&player, 4, deck, discards);
				newCardsText += cardStringValue(player.cards[4]) + ":";
				break;
			default:
				cout << "INVALID INPUT: " << userCardSwap[x] << endl;
				break;
			}
		}

		cout << newCardsText << endl;
		outputHand(&player);

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
}

// Check for a win condition
void checkForWin(Player* player) {
	string results = "You Lost";
	int winnings = 0;
	//Royal Flush (800)
	//Straight Flush (50)
	//Four of a kind(25)
	//Full House (9)
	//Flush(6)
	//Straight(4)
	//ThreeofaKind(3)
	//TwoPair (2)
	//OnePair, Jacks or Better (1)
	//OnePair(1)
	cout << "+$" << winnings << " " << results << endl;
}

// Swap a card
void SwapCard(Player* player, int cardNum, Deck* deck, Deck* discards) {
	discardCard(player, cardNum, deck, discards);
	drawCard(player, deck, discards);
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
void drawCard(Player* player, Deck* deck, Deck* discards) {
	// Take card from deck
	Card* drawnCardPtr = takeCardFromDeck(deck, discards);
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
Card* takeCardFromDeck(Deck* deck, Deck* discards) {

	// If deck is empty, swap with discards
	if (deck->deckSize <= 0) {
		CardNode* temp = deck->head;
		deck->head = discards->head;
		discards->head = nullptr;
		deck->deckSize = discards->deckSize;
		discards->deckSize = 0;
	}

	Card* randomCard;
	int randomCardNum = (rand() % deck->deckSize);

	if (deck->deckSize == 1) {
		randomCard = deck->head->card;
		delete deck->head;
		deck->deckSize--;
		return randomCard;
	}

	deck->deckSize--;

	if (randomCardNum == 1) {
		CardNode* current;
		current = deck->head;
		deck->head = current->next;
		randomCard = current->card;
		delete current;
	} else {
		CardNode* previous = deck->head;
		for (int x = 1; x < randomCardNum-1; x++) {
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
	while (n != nullptr) {
		cout << cardStringValue(n->card) << endl;
		n = n->next;
	}
}

// Output hand
void outputHand(Player* player) {
	cout << "Hand: " << endl;
	string a = "ABCDE";
	for (int x = 0; x < 5; x++) {
		cout << a[x] << ": " << cardStringValue(player->cards[x]) << endl;
	}
}

// Helper function for outputting a message and getting back a string
string getUserInput(string message) {
	string userInput = "";
	cout << message;
	cin >> userInput;

	return userInput;
}