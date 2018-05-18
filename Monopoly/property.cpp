#include"property.h"
#include"gameBoard.h"
#include"player.h"
#include"monopoly.h"
#include"input.h"
#include<sstream>
#include<windows.h>

using namespace std;
using namespace mnply;
using namespace inputmethods;

//property's constructor (invokes base class constructor) and destructor
property::property(groupEnum set, string name, int purchasePrice, vector<int> rent) : realEstate(set, name, purchasePrice), rentValues(rent), housePrice(50 * (1 + set / 2)){}
property::~property(){}

//accessor for houses
bool property::hasHouses()const{
	return(houses > 0);
}

//override of printDetails method, outputs member data of property in formatted manner
void property::printDetails(ostream& os)const{
	SetConsoleTextAttribute(hConsole, groupColour[group]);//sets console font colour to correspond with the property's group
	os << name << " [" << ((owner == nullptr) ? "Unowned" : (owner->getName() + "'s"));//output the properties name and owner ("Unowned" if no owner)
	if (mortgaged){//if the property is mortgaged, output this
		os << " - mortgaged";
	}
	else if (houses > 0){//if there are any houses built on the property
		if (houses == 5) os << " - Hotel";//output that there is a hotel if there are 5 houses
		else os << " - " << houses << " House(s)";//otherwise output the number of houses
	}
	os << ']';
	SetConsoleTextAttribute(hConsole, 15);//set the console font colour back to white
}

//prints a list of user choices for the manage menu
void property::displayManageOptions()const{
	stringstream mortgageRequest;
	if (mortgaged) mortgageRequest << "Unmortgage this property for " << pound << mortgagePrice*mortgageInterest;//if the property is already mortgaged, it will ask the user if they want to unmortgage
	else mortgageRequest << "Mortgage this property for " << pound << mortgagePrice;//if the property is not mortgage, it will ask the user if they want to mortgage
	cout << "Manage: " << *this << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "1: " << mortgageRequest.str() << endl;
	cout << "2: Buy a house for " << pound << housePrice << endl;
	cout << "3: Sell a house for " << pound << housePrice / 2 << endl;
	cout << "e: Exit property manager" << endl;
	cout << "-----------------------------------------------------------" << endl;
}

//menu to allow player to mortgage/unmortgage the property and buy/sell houses on it
void property::manage(){
	vector<char> options{ '1', '2', '3', 'e', 'E' };//vector of acceptable user inputs
	bool finishedManaging{ false };
	while (!finishedManaging){//repeats until user decides they are finished managing the property
		cout << owner->getName() << ": You have " << pound << owner->getMoney() << endl;//print current balance
		displayManageOptions();//display choices
		char choice = takeChoice<vector, char>("Please select one of the options above: ", options);//take user input for choice from options
		switch (choice){
		case '1':
			if (houses > 0){//dont allow user to mortgage a property with houses built on it
				cout << "Error: You must sell the houses on a property before it can be mortgaged!" << endl;
			}
			else{
				if (mortgaged) unMortgage();//if the property is currently mortgaged, unMortgage it
				else mortgage();//if the property is not currently mortgaged, mortgage it
			}
			break;
		case '2':
			buyHouse();//attempt to buy a house on the property
			break;
		case '3':
			sellHouse();//attempt to sell a house on the property
			break;
		case 'e':
		case 'E':
			finishedManaging = true;//exit manage()
			break;
		}
	}
}

//charges the owner of the property housePrice and adds a house to the property
void property::buyHouse(){
	if (owner->ownedInGroup(group) == realEstateInGroup[group] && !mortgaged && houses < 5) {//if the the owner of this property owns all properties in this group and there are not already 5 houses built on this property
		if (owner->getMoney() < housePrice){//check the owner has enough money to buy the house
			cout << "Error: You cannot afford to buy a house!" << endl << endl;
		}
		else{//if they have enough money
			owner->takeMoney(housePrice);//charge the owner for a house
			houses++;//add a house to the property
		}
	}
	else{//if cant build a house on this property, output reason(s) why
		if (owner->ownedInGroup(group) < realEstateInGroup[group]) cout << "Error: You must own all of the properties in the colour group in order to purchase houses!" << endl;
		if (mortgaged) cout << "Error: Cannot buy houses on a mortgaged property!" << endl;
		if (houses == 5) cout << "Error: There is already a hotel on this property!" << endl << endl;
	}
}

//removes a house from the property and pays the owner half of housePrice
void property::sellHouse(){
	if (houses > 0){//if there is atleast one house on the property
		try{
			owner->addMoney(housePrice / 2);//pay the player for the house (half of housePrice)
			houses--;//remove a house from the property
		}
		catch (domain_error &de){
			cerr << "domain_error: " << de.what() << endl;
		}
	}
	else{//if there are no houses to sell, output this to the player
		cout << "Error: There are no houses to sell on this property!" << endl << endl;
	}
}

//override of chargeRent method, charges rent based on the number of houses built on the property
void property::chargeRent(const shared_ptr<player>& plyr)const{
	try{
		plyr->payPlayer(owner, rentValues[houses]);//charges the player the appropriate amount of rent
	}
	catch (domain_error &de){
		cerr << "domain_error: " << de.what() << endl;
	}
}