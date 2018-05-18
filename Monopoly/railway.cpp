#include"railway.h"
#include"player.h"
#include<cmath>

using namespace std;
using namespace mnply;

//railway's constructor (invokes base class constructor) and destructor
railway::railway(groupEnum set, string name, int purchasePrice) :realEstate(set, name, purchasePrice){}
railway::~railway(){}

//chargeRent override, charges the player rent based on the number of railways owned by the owner
void railway::chargeRent(const shared_ptr<player>& plyr)const{
	int stationsOwned{ owner->ownedInGroup(group) };//get number of railways owned by the owner of the calling object
	int rent{ 25 * static_cast<int>(pow(2, stationsOwned - 1)) };//rent is 25 when 1 station owned, 50 when 2 owned, 100 when 3 owned or 200 when 4 owned
	cout << owner->getName() << " owns " << stationsOwned << " station(s)!" << endl;//output the number of stations owned
	try{
		plyr->payPlayer(owner, rent);//charge the player the appropriate rent
	}
	catch (domain_error &de){
		cerr << "domain_error: " << de.what() << endl;
	}
}  