//header guard
#ifndef PROPERTY_H
#define PROPERTY_H

#include"realEstate.h"
#include<string>
#include<memory>
#include<vector>
#include<iostream>

namespace mnply{

	//property class definition
	class property : public realEstate{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use property's private constructor
	private:
		std::vector<int> rentValues;//stores the different values for rent depending on the number of houses on the property
		int houses;//stores the number of houses built on the property
		int housePrice;//stores the price to the player to build a house on a property
		property(groupEnum set, std::string name, int purchasePrice, std::vector<int> rent);//private constructor prototype

		void displayManageOptions()const;//outputs list of player choices when managing property
		void manage();//menu to allow player to mortgage/unmortgage the property and buy/sell houses on it
		void buyHouse();//charges the owner of the property housePrice and adds a house to the property
		void sellHouse();//removes a house from the property and pays the owner half of housePrice

	public:
		~property();//destructor

		bool hasHouses()const;//house accessor prototype
		void printDetails(std::ostream& os)const;//override of printDetails, outputs member data of property in formatted manner

		void chargeRent(const std::shared_ptr<player>& plyr)const;//prototype of chargeRent override, charges rent based on the number of houses built on the property
	};

}//end of mnply namespace

#endif