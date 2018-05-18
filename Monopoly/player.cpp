#include"player.h"
#include"monopoly.h"
#include"realEstate.h"
#include"input.h"
#include<iostream>
#include<algorithm>

using namespace std;
using namespace mnply;
using namespace inputmethods;

//player constructor
player::player(string playerName, int startingMoney) :name(playerName), money(startingMoney), getOutOfJailFreeCards(0), jailed(false), bankrupt(false), turnsInJail(0){
	if (startingMoney < 0) throw domain_error("Player cannot be created with negative money!");//throws an exception if an attempt is made to construct a player with negative money
}
//player desctructor
player::~player(){}

//tradeOffer constructor
player::tradeOffer::tradeOffer()noexcept :amountOfMoney(0),outOfJailFreeCards(0){}

//----------------------------------player accessor functions--------------------------------------------------------------------------------------------------------------------

string player::getName()const{
	return name;
}
int player::getMoney()const{
	return money;
}
int player::countGetOutOfJailFreeCards()const{
	return getOutOfJailFreeCards;
}
bool player::inJail()const{
	return jailed;
}
int player::getTurnsInJail()const{
	return turnsInJail;
}
bool player::isBankrupt()const{
	return bankrupt;
}

//method to return the number of realEstate spaces owned by the player from the group 'group'
int player::ownedInGroup(groupEnum group)const{
	//defines inGroup lambda function as local variable, which compares the group of 'prop' to 'group' and returns true if they are the same
	auto inGroup = [group](const shared_ptr<realEstate>& prop){
		return(prop->getGroup() == group);
	};
	//counts the number of realEstate spaces in the players realEstateOwned list that meet the condition of inGroup
	int numOwned{ count_if(realEstateOwned.begin(), realEstateOwned.end(), inGroup) };
	return numOwned;
}

//method to print out all assets owned by the player
void player::printInventory()const{
	cout << "----- Player: " << name << " -----" << endl;
	cout << "Money = " << pound << money << endl;
	cout << "Get out of jail free cards = " << getOutOfJailFreeCards << endl;
	if (realEstateOwned.size() == 0){
		cout << "No real estate!" << endl;
	}
	else{
		cout << "Real Estate:" << endl;
		//iterate through the realEstateOwned list and output each realEstate using realEstate's overload of operator<<
		for_each(realEstateOwned.begin(), realEstateOwned.end(), [&](const shared_ptr<realEstate>& prop){
			cout << (*prop) << endl;
		});
	}
	cout << endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------player mutator functions----------------------------------------------------------------------------------------------------------------

//add 'amount' to players money
void player::addMoney(int amount){
	if (amount < 0) throw domain_error("Cannot add negative money to a player's inventory!");//throws an exception if an attempt is made to add negative money to a player
	money += amount;
}

//subtract 'amount' from players money
void player::takeMoney(int amount){
	while (amount > money && !bankrupt){//until either the player has enough money to pay, or they have declared bankruptcy
		notEnoughFunds(nullptr);//provide the user with options to deal with inadequate funds, nullptr used to tell the function that the amount isnt owed to any other player
	}
	if (!bankrupt){//if they haven't declare bankruptcy, remove 'amount' from the players inventory
		money -= amount;
	}
}

//transfers 'amount' of money from current player to player pointer to by 'plyr'
void player::payPlayer(const shared_ptr<player>& plyr, int amount){
	if (amount < 0) throw domain_error("Cannot pay negative money to a player!");//throws an exception if an attempt is made to pay negative money to a player
	cout << "Paying " << plyr->name << " " << pound << amount << endl << endl;//print payment message
	while (amount > money && !bankrupt){//until either the player has enough money to pay, or they have declared bankruptcy
		notEnoughFunds(plyr);//provide them with options to deal with inadequate funds, passing 'plyr' to let the function know who the player is in debt to
	}
	if (!bankrupt){//if they haven't declare bankruptcy, remove 'amount' from the current players account and add it to the player to be paid
		this->money -= amount;
		plyr->money += amount;
	}
}

//adds a realEstate shared_ptr to the players realEstateOwned list and sorts the list by group
void player::addRealEstate(shared_ptr<realEstate> pDetails){
	realEstateOwned.push_back(pDetails);
	realEstateOwned.sort([](shared_ptr<realEstate> i, shared_ptr<realEstate> j)->bool{return (i->getGroup() < j->getGroup()); });//sort by group
}

//add a get out of jail free card to the players inventory
void player::addGetOutOfJailFreeCard(){
	getOutOfJailFreeCards++;
}

//remove a get out of jail free card from the players inventory
void player::takeGetOutOfJailFreeCard(){
	if (getOutOfJailFreeCards > 0){
		getOutOfJailFreeCards--;
	}
}

void player::setJailed(bool jailedState){
	jailed = jailedState;
}


void player::incrementTurnsInJail(){
	turnsInJail++;
}

//set turnsInJail back to 0
void player::resetTurnsInJail(){
	turnsInJail = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------private player methods to manage insufficient funds when required to make a payment-------------------------------------------------

//runs whenever a payment is required that the player cannot afford
void player::notEnoughFunds(const shared_ptr<player>& playerOwed){
	cout << "You do not have enough to pay!" << endl;
	vector<char> options{ 'm', 'M', 'b', 'B' };//vector of acceptable user inputs
	char choice = takeChoice<vector, char>("Press 'm' to manage your inventory, or 'b' to declare bankruptcy: ", options);//take input from the user out of the list of options
	if (choice == 'm' || choice == 'M'){
		manageInventory();
	}
	else{
		declareBankruptcy(playerOwed);//pass on the pointer to the player who is owed money (nullptr if its not another player e.g. the bank)
	}
}

//transfers all assets to playerOwed (the bank if playerOwed==nullptr) then sets bankrupt to true
void player::declareBankruptcy(const shared_ptr<player>& playerOwed){
	cout << name << ": You have gone bankrupt!" << endl;
	if (playerOwed == nullptr){//if no other player owed, e.g. money owed to the bank
		cout << "All of your assets will be returned to the bank!" << endl;
		money = 0;//take all players money
	}
	else{//else if money owed to a specific player
		try{
			cout << "All of your assets will be transfered to " << playerOwed->getName() << endl;
			payPlayer(playerOwed, money);//pay all remaining money to player owed
			playerOwed->getOutOfJailFreeCards += getOutOfJailFreeCards; //give all Get Out of Jail Free cards to the player owed
		}
		catch (domain_error &de){//could be thrown by payPlayer function
			cerr << "domain_error: " << de.what() << endl;
		}
	}
	//iterate over all real estate owned
	for_each(realEstateOwned.begin(), realEstateOwned.end(), [&](const shared_ptr<realEstate>& prop){
		//transfer ownership of realEstate to the player owed
		if (playerOwed != nullptr) playerOwed->addRealEstate(prop);
		prop->changeOwner(playerOwed);
	});
	realEstateOwned.clear();//empty the players realEstateOwned list
	getOutOfJailFreeCards = 0;//take all the players Get Out of Jail Free cards
	bankrupt = true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//private method to display all owned real estate to the player, have them select one and return a pointer to that realEstate
shared_ptr<realEstate> player::selectRealEstate()const{
	if (realEstateOwned.size() == 0){//if no realEstate owned, output this and return a nullptr
		cout << "You have no Real Estate!" << endl;
		return shared_ptr<realEstate>(nullptr);
	}
	//otherwise..
	cout << "Real Estate:" << endl;
	cout << "0: go back" << endl;
	int i{ 0 };
	//iterate over the realEstateOwned list, printing each realEstate's details along with a unique index (i)
	for_each(realEstateOwned.begin(), realEstateOwned.end(), [&](const shared_ptr<realEstate>& prop){
		i++;
		cout << i << ": " << (*prop) << endl;
	});
	cout << endl;
	try{//request for user to input the index of the realEstate they are interest in purchasing
		int selection = takeInRange<int>("Please enter the number of the real estate you'd like to select: ", 0, i);
		if (selection == 0) return shared_ptr<realEstate>(nullptr); //if the user selected "go back", exit without selecting a property
		auto realEstateIterator = realEstateOwned.begin(); //iterator to point to a realEstate in realEstateOwned
		advance(realEstateIterator, selection-1);//advance the iterator to the user selected realEstate
		return *realEstateIterator;//return the dereferenced iterator
	}
	catch (invalid_argument &iv){//may be thrown by takeInRange<int> function
		cerr << "invalid_argument: " << iv.what() << endl;
		return shared_ptr<realEstate>(nullptr);//returns nullptr if exception thrown
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

//method that allows user to select one of their realEstate and manage it (the details of which are determined by the realEsate's manage() method)
void player::manageInventory()const{
	cout << name << ": You have " << pound << money << endl;//print current balance
	shared_ptr<realEstate> realEstateToManage = selectRealEstate();//return a pointer to a property of the players choosing
	if (realEstateToManage != nullptr){//if th selectRealEstate() return a pointer to a property (not a null ptr)
		realEstateToManage->manage();//manage the selected property
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------player methods for trading---------------------------------------------------------------------------------------------------

//tradeOffer member function to print out all tradeOffer memberData
void player::tradeOffer::printOffer()const{
	cout << "Money offered = " << pound << amountOfMoney << endl;
	cout << "Get out of Jail Free cards offered = " << outOfJailFreeCards << endl;
	if (realEstateOffered.size() > 0){//if realEstateOffered is not empty
		cout << "Real Estate offered:" << endl;
		//iterate through realEstateOffered and print the details of each realEstate using realEstate's overload of operator<<
		for_each(realEstateOffered.begin(), realEstateOffered.end(), [&](const shared_ptr<realEstate>& prop){
			cout << (*prop) << endl;
		});
	}
	else{//if realEstateOffered is empty, output this
		cout << "No real estate offered!" << endl;
	}
	cout << endl;
}

//private member function to display the user options when choosing a trade offer
void player::displayOfferOptions()const{
	cout << name << " - Offer Options:" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "1: Set money offered" << endl;
	cout << "2: Set number of Get out of Jail Free cards offered " << endl;
	cout << "3: Add real estate to offer" << endl;
	cout << "c: Confirm offer" << endl;
	cout << "e: Exit (abort offer)" << endl;
	cout << "-----------------------------------------------------------" << endl;
}

//private method which allows a user to add money, get out of jail free cards and realEstate to a tradeOffer object and returns that object
player::tradeOffer player::chooseTradeOffer()const{
	tradeOffer currentOffer;//create new tradeOffer object, currentOffer
	//initialise currentOffer's member data
	currentOffer.amountOfMoney = 0;
	currentOffer.outOfJailFreeCards = 0;

	auto addProp = [&](){//define a lambda function as a local variable addProp which allows users to select a realEstate shared_ptr from the realEstateOwned list and if appropiate, add it to currentOffer
		shared_ptr<realEstate> selectedRealEstate = selectRealEstate();//allow user to select realEstate
		if (selectedRealEstate == nullptr) return;//exit addProp() if no realEstate selected

		//count the number of realEstate owned of the same group which have houses built on them
		int realEstateWithHouses = count_if(realEstateOwned.begin(), realEstateOwned.end(), [selectedRealEstate](const shared_ptr<realEstate>& prop){
			return (prop->getGroup() == selectedRealEstate->getGroup() && prop->hasHouses());
		});
		//if any realEstate of the same group has houses built on it, this realEstate cannot be traded and isnt added to currentOffer
		if (realEstateWithHouses > 0){
			cout << "Error: Real estate in this group cannot be sold while houses remain on them!" << endl;
			return;//exit addProp()
		}

		//if the realEstate selected has already been added to the offer
		if (count(currentOffer.realEstateOffered.begin(), currentOffer.realEstateOffered.end(), selectedRealEstate) > 0){
			//do not re-add the realEstate to currentOffer
			cout << "Error: Already added to offer!" << endl;
			return;//exit addProp()
		}
		//if reaches here, a valid realEstate was selected, add it to currentOffer.realEstateOffered
		currentOffer.realEstateOffered.push_back(selectedRealEstate);//if a valid realEstate was selected, add it to currentOffer.realEstateOffered
	};

	bool finishedOfferSelection{ false };
	vector<char> options{ '1', '2', '3', 'c', 'C', 'e', 'E' };//vector of valid user inputs
	while (!finishedOfferSelection){//repeat until user chooses otherwise
		cout << "Current Offer:" << endl;
		currentOffer.printOffer();//print current offer
		displayOfferOptions();//display user choices
		char choice = takeChoice<vector, char>("Please select one of the options above: ", options);//take user choice from options
		switch (choice){
		case '1':
			try{
				cout << endl << name << ": you have " << pound << money << endl;
				//have the user enter the player enter the amount of money they would like to offer
				currentOffer.amountOfMoney = takeInRange<int>("Please enter the amount of money you are offering: ", 0, money);
			}catch (invalid_argument &iv){//may be thrown by takeInRange<int>
				cerr << "invalid_argument: " << iv.what() << endl;
			}
			break;
		case '2':
			try{
				cout << endl << name << ": you have " << getOutOfJailFreeCards << " Get out of Jail Free cards" << endl;
				//have the user enter the player enter the number of Get Out of Jail Free cards they would like to offer
				currentOffer.outOfJailFreeCards = takeInRange<int>("Please enter the number of Get out of Jail Free cards you are offering: ", 0, getOutOfJailFreeCards);
			} catch (invalid_argument &iv){//may be thrown by takeInRange<int>
				cerr << "invalid_argument: " << iv.what() << endl;
			}
			break;
		case '3':
			addProp();//add a realEstate to the offer
			break;
		case 'c':
		case 'C':
			cout << "Offer Submitted!" << endl << endl;
			finishedOfferSelection = true;//exit while loop
			break;
		case 'e':
		case 'E':
			cout << "Trade Aborted!" << endl << endl;
			currentOffer.amountOfMoney = -1;//Null value
			finishedOfferSelection = true;//exit while loop
			break;
		}
	}
	return currentOffer;
}

//player method to trade with another player
void player::trade(const vector<shared_ptr<player>>& players){

	//print list of player names, along with a unique index each (i)
	cout << "-----Players-----" << endl;
	cout << "0: go back" << endl;
	for (unsigned int i = 1; i <= players.size(); i++){
		cout << i << ": " << players[i-1]->getName() << endl;
	}
	try{
		//allow player to select tradePartner from list by their index
		int tradePartner = takeInRange<int>("Please enter the number of the player you would like to trade with: ", 0, players.size())-1;
		
		if (tradePartner == -1)return;//if the user selected "go back", exit trade()
		if (players[tradePartner]->getName() == name){//if the player selects themself, display a message and exit trade()
			cout << "C'mon now, you can't trade with yourself!" << endl;
			return;//exit trade()
		}

		tradeOffer playerOffer = chooseTradeOffer();//let player choose their tradeOffer
		if (playerOffer.amountOfMoney == -1) return;//if player aborted offer, exit trade()
		tradeOffer partnerOffer = players[tradePartner]->chooseTradeOffer();//let the trade partner choose their tradeOffer
		if (partnerOffer.amountOfMoney == -1) return;//if partner aborted offer, exit trade()

		//print both players final offers
		cout << "----Final Offers----" << endl;
		cout << name << ":" << endl;
		playerOffer.printOffer();
		cout << players[tradePartner]->getName() << ":" << endl;
		partnerOffer.printOffer();

		//take input as to whether the deal is accepted
		vector<char> options{ 'y', 'Y', 'n', 'N' };//valid input options
		char choice = takeChoice<vector, char>("Do both parties agree to this deal (y/n): ", options);//take input from options list
		if (choice == 'n' || choice == 'N'){//if no selected, abort the trade
			cout << "Trade aborted!" << endl;
			return;//exit trade()
		}//otherwise...

		//perform the trade
		cout << "It's a deal!" << endl;
		//trade cash
		players[tradePartner]->money += playerOffer.amountOfMoney - partnerOffer.amountOfMoney;
		money += partnerOffer.amountOfMoney - playerOffer.amountOfMoney;
		//trade Get out of Jail Free cards
		players[tradePartner]->getOutOfJailFreeCards += playerOffer.outOfJailFreeCards - partnerOffer.outOfJailFreeCards;
		getOutOfJailFreeCards += partnerOffer.outOfJailFreeCards - playerOffer.outOfJailFreeCards;
		//trade realEstate (iterate over the realEstateOffered lists of each players tradeOffer and transfer ownership of each realEstate)
		for_each(playerOffer.realEstateOffered.begin(), playerOffer.realEstateOffered.end(), [&](const shared_ptr<realEstate>& prop){
			players[tradePartner]->addRealEstate(prop);//add players realEstate to tradePartners realEstateOwned list
			prop->changeOwner(players[tradePartner]);//change owner of the realEstate
			realEstateOwned.remove(prop);//remove realEstate from players realEstateOwned list
		});
		for_each(partnerOffer.realEstateOffered.begin(), partnerOffer.realEstateOffered.end(), [&](const shared_ptr<realEstate>& prop){
			addRealEstate(prop);//add tradePartners realEstate to players realEstateOwned list
			prop->changeOwner(shared_from_this());//change owner of the realEstate
			players[tradePartner]->realEstateOwned.remove(prop);//remove realEstate from tradePartners realEstateOwned list
		});
	}
	catch (invalid_argument &iv){//may be thrown by takeInRange<int> at start of block
		cerr << "invalid_argument: " << iv.what() << endl;
		return;//exit trade()
	}
}

