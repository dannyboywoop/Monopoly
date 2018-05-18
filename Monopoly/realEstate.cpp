#include"realEstate.h"
#include"input.h"
#include"player.h"
#include"monopoly.h"
#include<iostream>
#include<sstream>
#include<vector>

using namespace std;
using namespace mnply;
using namespace inputmethods;

//realEstate constructor (invokes base class constructor) and destructor
realEstate::realEstate(groupEnum set, string name, int purchasePrice):gameSpace(name), price(purchasePrice), owner(nullptr), group(set), mortgaged(false), mortgagePrice((price / 2 > 50) ? price / 2 : 50), mortgageInterest(1.1){}
realEstate::~realEstate(){}

//accessor for group
groupEnum realEstate::getGroup()const{
	return group;
}

//virtual method to return whether a realEstate object has houses, will return false unless overridden to do otherwise
bool realEstate::hasHouses()const{
	return false;
}

//override of gameSpace's printDetails method, 
void realEstate::printDetails(ostream& os)const{
	SetConsoleTextAttribute(hConsole, groupColour[group]); //sets the console font colour to represent the realEstate group
	os << name << " [" << ((owner == nullptr) ? "Unowned" : (owner->getName() + "'s")); //output the name, owner ("unowned" if no owner) and mortgaged state of the realEstate
	if (mortgaged) os << " - Mortgaged";
	os << ']';
	SetConsoleTextAttribute(hConsole, 15);//sets the console font colour back to white
}

//override of gameSpace's land method, run whenever the realEstate is landed on by a player (takes the board containing the realEstate and the plyr who landed as parameters)
void realEstate::land(const shared_ptr<player>& plyr, gameBoard& board){
	printLandMessage(plyr);
	if (owner == nullptr){//if no owner
		offerPurchase(plyr);//allow player to purchase calling realEstate object
	}
	else if (owner == plyr){//else if the player who landed on the realEstate space is the owner of it, output this
		cout << plyr->getName() << ": You own this real estate!" << endl << endl;
	}
	else{//if the realEstate is owned, but not by the player who landed on it
		cout << owner->getName() << " owns this real estate!" << endl;//output the owner of the realEstate
		if (mortgaged){//dont charge rent if the realEstate is mortgaged
			cout << "This real estate is mortgaged!" << endl;
		}
		else if (owner->inJail()){//dont charge rent if the owner of the realEstate is in jail
			cout << owner->getName() << " is in jail, so your stay is free this time!" << endl;
		}
		else{
			this->chargeRent(plyr);//otherwise, charge rent (different derived classes will charge rent differently using polymorphism)
		}
	}
}

//allows user to purchase the calling realEstate object
void realEstate::offerPurchase(const shared_ptr<player>& plyr){
	cout << "This real estate is up for grabs!" << " Price: " << pound << price << endl;//output the price
	if (plyr->getMoney() < price){
		cout << "Sadly, you can't afford it!" << endl << endl;//if the player can't afford to purchase, output this
	}
	else{//if the player has enough money
		cout << plyr->getName() << ": You have " << pound << plyr->getMoney() << endl; // output the players current balance
		vector<char> options{ 'y', 'Y', 'n', 'N' };//vector of acceptable user inputs
		char choice = takeChoice<vector, char>("Would you like to purchase this real estate (y/n)?: ", options); //take character choice from user out of 'options'
		if (choice == 'y' || choice == 'Y'){//if the player chose yes
			plyr->takeMoney(price);//take the sale price for the realEstate from the player
			plyr->addRealEstate(shared_from_this()); //add a pointer to the calling realEstate object to the players inventory
			owner = plyr;//set the owner of the property to the player who has just purchased it
		}
	}

}

//mutator for owner
void realEstate::changeOwner(shared_ptr<player> newOwner){
	owner = newOwner;
}

//method to provide player with the option to mortgage/unmortgage the realEsate, as applicable
void realEstate::manage(){
	cout << owner->getName() << ": You have " << pound << owner->getMoney() << endl;//print current balance
	cout << "Manage: " << *this << endl;//output the details of the realEstate being managed
	stringstream inputRequest;
	if (mortgaged) inputRequest << "Would you like to unmortgage this real estate for " << pound << mortgagePrice*mortgageInterest << " (y/n)?: ";//if mortgaged, set the input request to ask if the player wishes to unmortgage the realEstate
	else inputRequest << "Would you like to mortgage this real estate for " << pound << mortgagePrice << " (y/n)?: ";//if not mortgaged, set the input request to ask if the player wishes to mortgage the realEstate

	vector<char> options{ 'y', 'Y', 'n', 'N' };//vector of acceptable user inputs
	char choice = takeChoice<vector, char>(inputRequest.str(), options);//take character choice from user out of 'options'
	if (choice == 'y' || choice == 'Y'){//if the user chose yes
		if (mortgaged) unMortgage(); //unMortgage the realEstate if it is currently mortgaged
		else mortgage();//Mortgage the realEstate if it is currently not mortgaged
	}
}

//mortgages realEstate, paying the owner the mortgagePrice
void realEstate::mortgage(){
	try{
		owner->addMoney(mortgagePrice);
		mortgaged = true;
	}
	catch (domain_error &de){
		cerr << "domain_error: " << de.what() << endl;
	}
}

//unMortgages realEstate, if the owner can afford the price to unMortgage, taking the appropriate amount of money from the owner
void realEstate::unMortgage(){
	if (owner->getMoney() < mortgagePrice*mortgageInterest){//if the owner cannot affor to unMortgage, print a message
		cout << "Error: You cannot afford to unmortgage this real estate!" << endl << endl;
	}
	else{
		owner->takeMoney(static_cast<int>(mortgagePrice*mortgageInterest));//charge the owner the original mortgage price x mortgageInterest
		mortgaged = false;
	}
}
