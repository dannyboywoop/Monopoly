#include"utility.h"
#include"gameBoard.h"
#include"player.h"
#include"dice.h"
#include"input.h"
#include<iostream>

using namespace std;
using namespace mnply;
using namespace inputmethods;

//utilty's constructor (invokes base class constructor) and destructor
utility::utility(groupEnum set, string name, int purchasePrice) :realEstate(set, name, purchasePrice){}
utility::~utility(){}

//chargeRent override, has the player roll the dice and charges them the amount rolled*multiplier, where multiplier depends on the number of utilities owned
void utility::chargeRent(const shared_ptr<player>& plyr)const{
	int multiplier{ 4 };//default multiplier
	if (owner->ownedInGroup(group) == realEstateInGroup[group]){//if all utilities owned
		multiplier = 10;//multiplier increased to 10
	}
	dice rentDice;//create a new pair dice
	cout << "You must roll the dice and pay the amount shown x" << multiplier << endl;
	pauseForUser();//wait for user to hit enter
	int rent{ rentDice.rollDice()*multiplier };//roll dice and set total rolled * multiplier to be the rent due
	try{
		plyr->payPlayer(owner, rent);//charge the player the rent due
	}
	catch (domain_error &de){
		cerr << "domain_error: " << de.what() << endl;
	}
}