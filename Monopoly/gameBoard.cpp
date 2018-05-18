#include"gameBoard.h"
#include"realEstate.h"
#include"utility.h"
#include"railway.h"
#include"property.h"
#include"miscSpaces.h"
#include"luckyDip.h"
#include"player.h"
#include<iostream>
#include<algorithm>

using namespace std;
using namespace mnply;

//token constructor
gameBoard::token::token(int pos) :position(pos),tokenOwner(nullptr){}

//gameBoard constructor (takes number of players as parameter and intitializes tokens vector and monopolyBoard vector to the appropriate sizes, places each token on GO)
gameBoard::gameBoard(int numOfPlayers) : tokens(numOfPlayers, 0), boardSize(40), monopolyBoard(40, shared_ptr<gameSpace>(nullptr)){
	//fills the monopolyBoard vector of base class pointers with new derived objects of varied types, initializing each space appropriately
	monopolyBoard[0]  = shared_ptr<GO            >(new             GO());        //GO
	monopolyBoard[1]  = shared_ptr<property      >(new       property(brown,     "Old Kent Road",            60,     { 2,   10,  30,  90,   160,  250 }));
	monopolyBoard[2]  = shared_ptr<communityChest>(new communityChest());        //Community Chest
	monopolyBoard[3]  = shared_ptr<property      >(new       property(brown,     "Whitechapel Road",         60,     { 4,   20,  60,  180,  320,  450 }));
	monopolyBoard[4]  = shared_ptr<tax           >(new            tax(           "Income Tax",               200 ));
	monopolyBoard[5]  = shared_ptr<railway       >(new        railway(stations,  "Kings Cross Station",      200 ));
	monopolyBoard[6]  = shared_ptr<property      >(new       property(lightBlue, "The Angel Islington",      100,    { 6,   30,  90,  270,  400,  550 }));
	monopolyBoard[7]  = shared_ptr<chance        >(new         chance());        //Chance
	monopolyBoard[8]  = shared_ptr<property      >(new       property(lightBlue, "Euston Road",              100,    { 6,   30,  90,  270,  400,  550 }));
	monopolyBoard[9]  = shared_ptr<property      >(new       property(lightBlue, "Pentonville Road",         120,    { 8,   40, 100,  300,  450,  600 }));
	monopolyBoard[10] = shared_ptr<jail          >(new           jail());        //Jail
	monopolyBoard[11] = shared_ptr<property      >(new       property(pink,      "Pall Mall",                140,    { 10,  50, 150,  450,  625,  750 }));
	monopolyBoard[12] = shared_ptr<utility       >(new        utility(utilities, "Electric Company",         150 ));
	monopolyBoard[13] = shared_ptr<property      >(new       property(pink,      "Whitehall",                140,    { 10,  50, 150,  450,  625,  750 }));
	monopolyBoard[14] = shared_ptr<property      >(new       property(pink,      "Northumberland Avenue",    160,    { 12,  60, 180,  500,  700,  900 }));
	monopolyBoard[15] = shared_ptr<railway       >(new        railway(stations,  "Marylebone Station",       200 ));
	monopolyBoard[16] = shared_ptr<property      >(new       property(orange,    "Bow Street",               180,    { 14,  70, 200,  550,  750,  950 }));
	monopolyBoard[17] = shared_ptr<communityChest>(new communityChest());        //Community Chest
	monopolyBoard[18] = shared_ptr<property      >(new       property(orange,    "Marlborough Street",       180,    { 14,  70, 200,  550,  750,  950 }));
	monopolyBoard[19] = shared_ptr<property      >(new       property(orange,    "Vine Street",              200,    { 16,  80, 220,  600,  800, 1000 }));
	monopolyBoard[20] = shared_ptr<freeParking   >(new    freeParking());        //Free Parking
	monopolyBoard[21] = shared_ptr<property      >(new       property(red,       "The Strand",               220,    { 18,  90, 250,  700,  875, 1050 }));
	monopolyBoard[22] = shared_ptr<chance        >(new         chance());        //Chance
	monopolyBoard[23] = shared_ptr<property      >(new       property(red,       "Fleet Street",             220,    { 18,  90, 250,  700,  875, 1050 }));
	monopolyBoard[24] = shared_ptr<property      >(new       property(red,       "Trafalgar Square",         240,    { 20, 100, 300,  750,  925, 1100 }));
	monopolyBoard[25] = shared_ptr<railway       >(new        railway(stations,  "Fenchurch St Station",     200 ));
	monopolyBoard[26] = shared_ptr<property      >(new       property(yellow,    "Leicester Square",         260,    { 22, 110, 330,  800,  975, 1150 }));
	monopolyBoard[27] = shared_ptr<property      >(new       property(yellow,    "Coventry Street",          260,    { 22, 110, 330,  800,  975, 1150 }));
	monopolyBoard[28] = shared_ptr<utility       >(new        utility(utilities, "Water Works",              150 ));
	monopolyBoard[29] = shared_ptr<property      >(new       property(yellow,    "Piccadilly",               280,    { 22, 120, 360,  850, 1025, 1200 }));
	monopolyBoard[30] = shared_ptr<goToJail      >(new       goToJail());        //Go to Jail
	monopolyBoard[31] = shared_ptr<property      >(new       property(green,     "Regent Street",            300,    { 26, 130, 390,  900, 1100, 1275 }));
	monopolyBoard[32] = shared_ptr<property      >(new       property(green,     "Oxford Street",            300,    { 26, 130, 390,  900, 1100, 1275 }));
	monopolyBoard[33] = shared_ptr<communityChest>(new communityChest());        //Community Chest
	monopolyBoard[34] = shared_ptr<property      >(new       property(green,     "Bond Street",              320,    { 28, 150, 450, 1000, 1200, 1400 }));
	monopolyBoard[35] = shared_ptr<railway       >(new        railway(stations,  "Liverpool Street Station", 200 ));
	monopolyBoard[36] = shared_ptr<chance        >(new         chance());        //Chance
	monopolyBoard[37] = shared_ptr<property      >(new       property(darkBlue,  "Park Lane",                350,    { 35, 175, 500, 1100, 1300, 1500 }));
	monopolyBoard[38] = shared_ptr<tax           >(new            tax(           "Super Tax",                100 ));
	monopolyBoard[39] = shared_ptr<property      >(new       property(darkBlue,  "Mayfair",                  400,    { 50, 200, 600, 1400, 1700, 2000 }));
}

//gameBoard destructor
gameBoard::~gameBoard(){}

//method to output the current state of the game board
void gameBoard::displayBoard()const{
	cout << "------------Game Board------------" << endl;
	for (int i = 0; i < boardSize; i++){//for all gameSpaces on the gameBoard
		cout << i << ": " << *monopolyBoard[i]; //output gameSpace data using the overload of operator<< for gameSpaces
		for (unsigned int t = 0; t < tokens.size(); t++){ //for each of the tokens
			if (tokens[t].position == i && !tokens[t].tokenOwner->inJail()){ //if the token is on the current gameSpace and the owner of the token is not bankrupt
				cout << " - " << tokens[t].tokenOwner->getName(); //print the name of the token owner next to the gamespace
				if (i == getSpaceIndex("Jail") && !tokens[t].tokenOwner->inJail()) cout << "(just visiting)"; //if the player is on the jail space but not in jail, print that they are "just visiting"
			}
		}
		cout << endl;
	}
	cout << endl;
}

//assigns each player in playerList to a token in tokens (if playerList and tokens are the same length)
void gameBoard::assignTokens(const vector<shared_ptr<player>>& playerList){
	auto tokenIterator = tokens.begin();
	if (playerList.size() == tokens.size()){//if there are the same number of players as tokens
		for (auto playerIterator = playerList.begin(); playerIterator < playerList.end(); playerIterator++){//iterate through the playerList
			tokenIterator->tokenOwner = *playerIterator; //copy the shared_ptr to a player from playerList to the tokenOwner member of a token
			tokenIterator++;//move to the next token
		}
	}
	else{//if there are a different number of tokens to players throw an exception
		throw invalid_argument("Must be an equal number of players and tokens!");
	}
}

// private method that takes a shared_ptr to a player and returns the index of that player's token in the tokens vector
int gameBoard::getTokenIndex(const shared_ptr<player>& plyr)const{
	for (unsigned int t = 0; t < tokens.size(); t++){//loop over all tokens
		if (tokens[t].tokenOwner == plyr){//if a token is found that is owned by the desired player
			return t ;//return the index of that token
		}
	}
	throw invalid_argument("Player does not have a token on the board!");//throw an exception if the player is not found in the tokens vector
}

//private method that takes a string name of a gameSpace and returns the index of that gameSpace in the monopolyBoard vector
int gameBoard::getSpaceIndex(string gameSpaceName)const{
	for (int s = 0; s < boardSize; s++){ //loop over all gameSpaces
		if (monopolyBoard[s]->getName() == gameSpaceName){ //if a gameSpace is found with the desired name
			return s; //return the index of that token
		}
	}
	throw invalid_argument("There are no gameSpaces of this name!");//throw an exception if a gameSpace wiht a matching name is not found on the monopolyBoard
}

//method which takes a pointer to a player and a number of spaces and moves the players token that number of spaces (spaces can be positive or negative)
void gameBoard::advancePlayer(const shared_ptr<player>& plyr, int spaces){
	int tokenIndex{ getTokenIndex(plyr) }; //get the index of the token belonging to the player
	tokens[tokenIndex].position += spaces; //add 'spaces' to the position of the token on the game board

	//if the player has made one or more loops of the board, their tokens position will be >= boardSize. They are rewarded for each passing of GO a their position is reduced back to an index between 0 and boardsize-1
	while (tokens[tokenIndex].position >= boardSize){
		tokens[tokenIndex].position -= boardSize;
		GO::passGo(tokens[tokenIndex].tokenOwner);
	}
	//if a player has moved backwards passed GO, their tokens position will be <0, if so this position is increased back to an index between 0 and boardSize-1
	while (tokens[tokenIndex].position < 0){
		tokens[tokenIndex].position += boardSize;
	}

	//run the land method of the gameSpace landed on (passing a shared_ptr to the player who landed on it, as well as the calling gameBoard object)
	monopolyBoard[tokens[tokenIndex].position]->land(tokens[tokenIndex].tokenOwner, *this);
}

//method which, given a pointer to a player and destination name, advances the player's token to destination on the monopolyBoard
void gameBoard::movePlayer(const shared_ptr<player>& plyr, string destinationName){
	int tokenIndex{ getTokenIndex(plyr) };// get the index of the player's token
	int destination{ getSpaceIndex(destinationName) }; //get the index of the destination gameSpace

	if (destination <= tokens[tokenIndex].position){ //if the gameSpace index is <= the tokens current position, the token will pass go on the way to the destination
		GO::passGo(tokens[tokenIndex].tokenOwner);//reward the player for passing go
	}
	tokens[tokenIndex].position = destination; //move token to destination on monopolyBoard

	//run the land method of the gameSpace landed on (passing a shared_ptr to the player who landed on it, as well as the calling gameBoard object)
	monopolyBoard[destination]->land(tokens[tokenIndex].tokenOwner, *this);
}

//moves a players token directly to the jail gameSpace without passing GO
void gameBoard::moveToJail(const shared_ptr<player>& plyr){
	tokens[getTokenIndex(plyr)].position = getSpaceIndex("Jail");//get the tokenIndex of the players token and set its position to the index of jail
}
