//header guard
#ifndef REAL_ESTATE_H
#define REAL_ESTATE_H

#include"gameSpace.h"
#include<string>
#include<memory>
#include<windows.h>

namespace mnply{
	
	//global constant and enum definition
	enum groupEnum{ brown, lightBlue, pink, orange, red, yellow, green, darkBlue, utilities, stations };//enum to store all possible realEstate groups
	const WORD groupColour[10]{6, 11, 13, 12, 4, 14, 2, 9, 7, 8};//stores the font colour values for each of the realEstate groups

	//definition of abstract realEstate class (base class for utility, railway and property)
	class realEstate : public gameSpace, public std::enable_shared_from_this<realEstate>{//inheritance from enable_shared_from_this allows methods to return a shared_ptr to the calling object
		friend class gameBoard; //declares gameBoard as friend class to allow it to use realEstate's protected constructor
	protected:
		int price;//price to purchase the realEstate
		std::shared_ptr<player> owner; //shared pointer to the player that currently owns the property, nullptr if unowned
		groupEnum group;
		int mortgagePrice;
		bool mortgaged;
		const double mortgageInterest; //multiplier applied to mortgagePrice when paying to unmortgage realEstate

		//protected method prototypes
		realEstate(groupEnum set, std::string name, int purchasePrice);//constructor is protected
		void mortgage();
		void unMortgage();

	public:
		//prototype for virtual destructor
		virtual ~realEstate();
	
		//public method prototypes
		groupEnum getGroup()const;//group accessor
		virtual bool hasHouses()const;//virtual method to return whether realEstate has houses on it
		virtual void printDetails(std::ostream& os)const;//virtual override of groupSpace's printDetails

		void land(const std::shared_ptr<player>& plyr, gameBoard& board);//override of groupSpace's land method
		void offerPurchase(const std::shared_ptr<player>& plyr);//offers plyr the option to purchase the calling realEstate object
		void changeOwner(std::shared_ptr<player> newOwner); //makes a copy of the newOwner shared_ptr<player> and sets it to be the realEstates owner

		virtual void chargeRent(const std::shared_ptr<player>& plyr)const = NULL;//pure virtual function, will be overriden by derived classes to charge players rent
		virtual void manage();//virtual method, provides options to mortgage/unmortgage the realEstate
	};

}//end of mnply namespace

#endif