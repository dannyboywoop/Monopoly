#include"luckyDip.h"
#include"player.h"
#include"gameBoard.h"
#include<iostream>
#include<random>
#include<algorithm>

using namespace std;
using namespace mnply;

//THIS FILE CONTAINS THE METHOD DEFINITIONS FOR luckyDip AND IT'S 2 DERIVED CLASSES: chance AND communityChest

// --------------------------------------------------------------------luckyDip function definitions----------------------------------------------------------------------
//constructor (invokes base class constructor) and destructor for luckyDip
luckyDip::luckyDip(string name):gameSpace(name){}
luckyDip::~luckyDip(){}

//constructor for luckyDip::card struct, with parameters of: a string and a function-like object with a void return type that takes a const std::shared_ptr<player>& and a gameBoard& as parameters
luckyDip::card::card(string mess, function<void(const shared_ptr<player>&, gameBoard&)> act) : message(mess), action(act){}

//method takes a reference to a vector of cards, a string and a function-like object. It creates a new card object and adds it to the vector of cards
void luckyDip::addCard(vector<card>& deck, string mess, function<void(const shared_ptr<player>&, gameBoard&)> act){
	card newCard{ mess, act };//uses card's parameterised constructor to create a new card with the parameters passed
	deck.push_back(newCard);//adds the new card to the deck passed by reference to the method
}

//method takes a vector of cards by reference and randomly shuffles them
void luckyDip::shuffleDeck(vector<card>& deck){
	random_device rd;//acts as seed
	mt19937 g(rd());//random number generator, seeded with rd
	shuffle(deck.begin(), deck.end(), g); //shuffle deck based on random number generator g
}

//takes the first card from a vector of cards, prints its message, performs its action and moves it to the end of the vector
void luckyDip::issueCard(vector<card>& deck, const shared_ptr<player>& plyr, gameBoard& board){
	if (deck.size() > 1){//if there is atleast 2 cards in the deck
		try{
			cout << "The card reads: " << (deck[0]).message << endl << endl;//read the message of the first card in the deck
			deck[0].action(plyr, board);//perform the action of the first card in the deck
			rotate(deck.begin(), deck.begin() + 1, deck.end());//move the first card to the back of the deck
		}
		catch (domain_error &de){//can be thrown by player::addmoney()
			cerr << "domain_error: " << de.what() << endl;
		}
		catch (invalid_argument &iv){//can be thrown by gameBoard member functions
			cerr << "invalid_argument: " << iv.what() << endl;
		}
		catch (bad_function_call &bfc){//could be thrown if an attempt is made to call a null std::function
			cerr << "bad_function_call: " << bfc.what() << endl;
		}
	}
}

// --------------------------------------------------------------------chance function definitions----------------------------------------------------------------------
//constructor (invokes base class constructor) and destructor for chance
chance::chance() :luckyDip("Chance"){}
chance::~chance(){}

//initialise static member data 'deck' using static method makeDeck()
vector<luckyDip::card> chance::deck = chance::makeDeck();

//static method to fill the chance deck with shuffled cards
vector<luckyDip::card> chance::makeDeck(){
	vector<card> generatedDeck;//create a new vector of cards

	//add cards to generatedDeck using the static luckyDip::addCard() method:
	//parameters are as follows: (deck to add card to, string message to put on the card, lambda function to act as the action of the card)
	luckyDip::addCard(generatedDeck, "Advance to \"GO\"", [](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "GO"); });
	luckyDip::addCard(generatedDeck, "Go to jail. Move directly to jail. Do not pass \"GO\". Do not collect \x9C 200", [](const shared_ptr<player>& plyr, gameBoard& board){ board.moveToJail(plyr); plyr->setJailed(true); });
	luckyDip::addCard(generatedDeck, "Advance to Pall Mall. If you pass \"GO\" collection \x9C 200", [](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "Pall Mall"); });
	luckyDip::addCard(generatedDeck, "Take a trip to Marylebone Station and if you pass \"GO\" collect \x9C 200", [](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "Marylebone Station"); });
	luckyDip::addCard(generatedDeck, "Advance to Trafalgar Square. If you pass \"GO\" collect \x9C 200", [](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "Trafalgar Square"); });
	luckyDip::addCard(generatedDeck, "Advance to Mayfair", [](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "Mayfair"); });
	luckyDip::addCard(generatedDeck, "Go back three spaces", [](const shared_ptr<player>& plyr, gameBoard& board){ board.advancePlayer(plyr, -3); });
	luckyDip::addCard(generatedDeck, "Pay school fees of \x9C 150", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->takeMoney(150); });
	luckyDip::addCard(generatedDeck, "\"Drunk in charge\" fine \x9C 20", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->takeMoney(20); });
	luckyDip::addCard(generatedDeck, "Speeding fine \x9C 15", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->takeMoney(15); });
	luckyDip::addCard(generatedDeck, "Your building loan matures. Receive \x9C 150", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(150); });
	luckyDip::addCard(generatedDeck, "You have won a crossword competition. Collect \x9C 100", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(150); });
	luckyDip::addCard(generatedDeck, "Bank pays you dividend of \x9C 50", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(50); });
	luckyDip::addCard(generatedDeck, "Get out of jail free. This card may be kept until needed or sold", [](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addGetOutOfJailFreeCard(); });

	//once all cards have been added to generatedDeck, shuffle the deck and return it
	shuffleDeck(generatedDeck);
	return generatedDeck;
}

//performed when any chance space is landed on
void chance::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	issueCard(deck, plyr, board);//issue a chance card to the player from the static chance deck
}

// --------------------------------------------------------------------communityChest function definitions----------------------------------------------------------------------
//constructor (invokes base class constructor) and destructor for communityChest
communityChest::communityChest() :luckyDip("Community Chest"){}
communityChest::~communityChest(){}

//initialise static member data 'deck' using static method makeDeck()
vector<luckyDip::card> communityChest::deck = communityChest::makeDeck();

//static method to fill the communityChest deck with shuffled cards
vector<luckyDip::card> communityChest::makeDeck(){
	vector<card> generatedDeck;//create a new vector of cards

	//add cards to generatedDeck using the static luckyDip::addCard() method:
	//parameters are as follows: (deck to add card to, string message to put on the card, lambda function to act as the action of the cards)
	
	addCard(generatedDeck, "Advance to \"GO\"",
		[](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "GO"); });

	addCard(generatedDeck, "Advance to Old Kent Road",
		[](const shared_ptr<player>& plyr, gameBoard& board){ board.movePlayer(plyr, "Old Kent Road"); });

	addCard(generatedDeck, "Go to jail. Move directly to jail. Do not pass \"GO\". Do not collect \x9C 200",
		[](const shared_ptr<player>& plyr, gameBoard& board){ board.moveToJail(plyr); plyr->setJailed(true); });

	addCard(generatedDeck, "Pay hospital \x9C 100",
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->takeMoney(100); });

	addCard(generatedDeck, "Doctor's fee. Pay \x9C 50",
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->takeMoney(50); });

	addCard(generatedDeck, "Pay your insurance premium \x9C 50",
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->takeMoney(50); });

	addCard(generatedDeck, "Bank error in your favour. Collect \x9C 200",
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(200); });

	addCard(generatedDeck, "Annuity matures. Collect \x9C 100",
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(100); });

	addCard(generatedDeck, "You inherit \x9C 100", 
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(100); });

	addCard(generatedDeck, "From sale of stock you get \x9C 50", 
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(50); });

	addCard(generatedDeck, "Receive interest on 7% preference shares: \x9C 25", 
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(25); });

	addCard(generatedDeck, "Income tax refund. Collect \x9C 20", 
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(20); });

	addCard(generatedDeck, "You have won second prize in a beauty contest. Collect \x9C 10", 
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addMoney(10); });

	addCard(generatedDeck, "Get out of jail free. This card may be kept until needed or sold", 
		[](const shared_ptr<player>& plyr, gameBoard& board){ plyr->addGetOutOfJailFreeCard(); });
	
	//once all cards have been added to generatedDeck, shuffle the deck and return it
	shuffleDeck(generatedDeck);
	return generatedDeck;
}

//performed when any communityChest space is landed on
void communityChest::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	issueCard(deck, plyr, board);//issue a communityChest card to the player from the static communityChest deck
}