#include"miscSpaces.h"
#include"player.h"
#include"monopoly.h"
#include"gameBoard.h"
#include<iostream>

using namespace std;
using namespace mnply;

//THIS FILE CONTAINS THE FUNCTION DEFINITIONS FOR MULTIPLE SMALL CLASSES: GO, jail, goToJail, freeParking and tax

//--------------------------------------------------------------------- GO member function definitions ----------------------------------------------------------------------
//GO's constructor (invokes base class constructor) and destructor
GO::GO():gameSpace("GO"){}
GO::~GO(){}

//initialise static const goReward to £200
const int GO::goReward{ 200 };

//static method to reward players for passing go
void GO::passGo(const shared_ptr<player>& plyr){
	try{
		cout << plyr->getName() << ": You passed Go! Receive " << pound << goReward << endl << endl; //output reward message
		plyr->addMoney(goReward);//give the player goReward
	}
	catch (domain_error &de){
		cerr << "domain_error: " << de.what() << endl;
	}
}

//override of gameSpace's land method for GO
void GO::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
}

//--------------------------------------------------------------------- jail member function definitions ----------------------------------------------------------------------
//jail's constructor (invokes base class constructor) and destructor
jail::jail():gameSpace("Jail"){} 
jail::~jail(){}

//override of gameSpace's land method for jail
void jail::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	cout << "(Just visiting!)" << endl;
}

//--------------------------------------------------------------------- goToJail member function definitions ----------------------------------------------------------------------
//goToJail's constructor (invokes base class constructor) and destructor
goToJail::goToJail() : gameSpace("Go to Jail"){}
goToJail::~goToJail(){}

//override of gameSpace's land method for goToJail, sends the player directly to jail
void goToJail::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	cout << "You will move directly to Jail! You will not pass GO, you will not collect " << pound << "200" << endl << endl;//output go to jail message
	try{
		board.moveToJail(plyr);//place the players token on the jail space of the game board
		plyr->setJailed(true);//set the players jailed state to true
	}
	catch (invalid_argument &iv){
		cerr << "invalid_argument: " << iv.what() << endl;
	}
}

//--------------------------------------------------------------------- freeParking member function definitions ----------------------------------------------------------------------
//freeParking's constructor (invokes base class constructor) and destructor
freeParking::freeParking() : gameSpace("Free Parking"){}
freeParking::~freeParking(){}

//override of gameSpace's land method for freeParking, no action is taken
void freeParking::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	cout << "You can just chill here for a turn!" << endl << endl;
}

//--------------------------------------------------------------------- tax member function definitions ----------------------------------------------------------------------
//tax's constructor (invokes base class constructor) and destructor
tax::tax(string name, int amount) :gameSpace(name), charge(amount){}
tax::~tax(){}

//override of gameSpace's land method for tax, charges the player 'charge'
void tax::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	cout << pound << charge << " will be removed from your inventory!" << endl << endl;//print charge amount
	plyr->takeMoney(charge);//take 'charge' from player
}