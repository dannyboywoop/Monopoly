//header guard
#ifndef PLAYER_H
#define PLAYER_H

#include<string>
#include<list>
#include<memory>
#include<vector>

namespace mnply{

	//forward declaration of realEstate and groupEnum to allow their use in the player class definition
	class realEstate;
	enum groupEnum;

	//definition of player class
	class player :public std::enable_shared_from_this<player> {//inheritance here used to allow player methods to access a shared_ptr to the calling object
	private:
		//private member data
		std::string name;
		int money;
		std::list<std::shared_ptr<realEstate>> realEstateOwned; //stores shared_ptrs to all realEstate spaces owned by the player
		int getOutOfJailFreeCards; //number of get out of jail free cards owned
		bool jailed;
		int turnsInJail; //stores the number of turns a player has spent in jail
		bool bankrupt;

		//tradeOffer struct definition, private to player class
		struct tradeOffer{
			int amountOfMoney;
			int outOfJailFreeCards;
			std::list<std::shared_ptr<realEstate>> realEstateOffered;
			void printOffer()const;//prints out member data of tradeOffer
			tradeOffer()noexcept;
		};

		//private methods prototypes
		void notEnoughFunds(const std::shared_ptr<player>& playerOwed);//runs when the player is made to pay another player or the bank more money than they have
		void declareBankruptcy(const std::shared_ptr<player>& playerOwed); //runs when player declares bankruptcy
		std::shared_ptr<realEstate> selectRealEstate()const;//allows the player to select one of their realEstate spaces, a shared_ptr to that realEstate is then returned
		void displayOfferOptions()const; //prints out user menu for choosing a trade offer
		tradeOffer chooseTradeOffer()const; //allows user to select what money, cards and realestate to add to a trade offer

	public:
		//constructor and destructor prototypes
		player(std::string playerName, int startingMoney);
		~player();

		//accessor prototypes
		std::string getName()const;
		int getMoney()const;
		int countGetOutOfJailFreeCards()const;
		bool inJail()const;
		int getTurnsInJail()const;
		bool isBankrupt()const;
		int ownedInGroup(groupEnum group)const;//returns number of realEstate spaces owned by the player from group 'group'
		void printInventory()const; //prints out all all assets owned by the player

		//mutator prototypes
		void addMoney(int amount);
		void takeMoney(int amount);
		void payPlayer(const std::shared_ptr<player>& plyr, int amount);
		void addRealEstate(std::shared_ptr<realEstate> pDetails);//adds a realEstate shared_ptr to the players realEstateOwned list
		void addGetOutOfJailFreeCard();
		void takeGetOutOfJailFreeCard();
		void setJailed(bool jailedState);
		void incrementTurnsInJail();
		void resetTurnsInJail();//sets turns in jail to 0

		//other public method prototypes
		void manageInventory()const; //menu from which player can mortgage/unmortgage real estate + buy/sell houses
		void trade(const std::vector<std::shared_ptr<player>>& players); //menu for trading with other players
	};

}//end of mnply namespace

#endif