#include "gameSpace.h"
#include "player.h"

using namespace std;
using namespace mnply;

//overload of operator<< to work with gameSpace objects
ostream& mnply::operator<<(ostream& os, const gameSpace& gs){
	gs.printDetails(os);//calls the virtual printDetails() method of gameSpace to allow for polymorphism
	return os;
}

//gameSpace constructor and destructor
gameSpace::gameSpace(string spaceName) :name(spaceName){}
gameSpace::~gameSpace(){}

//name accessor
string gameSpace::getName()const{
	return name;
}

//virtual function to output details of a gameSpace to os (overridden by some derived classes of gameSpace)
void gameSpace::printDetails(ostream& os)const{
	os<<name;
}

//runs when a gameSpace is landed on, displays confirmation of who and what has been landed on
void gameSpace::printLandMessage(const shared_ptr<player>& plyr)const{
	cout << plyr->getName() << ": You have landed on " << (*this) << endl; //uses the operator<< overload for gameSpace objects defined above
}

