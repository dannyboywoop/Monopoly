//header guard
#ifndef LUCKY_DIP_H
#define LUCKY_DIP_H

#include"gameSpace.h"
#include<string>
#include<functional>
#include<vector>
#include<memory>

//THIS FILE CONTAINS THE DEFINITION THE CLASS luckyDip AND IT'S 2 DERIVED CLASSES: chance AND communityChest

namespace mnply{

	//definition of abstract luckyDip class (base class for chance and communityChest)
	class luckyDip : public gameSpace{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use luckyDip's protected constructor
	protected:
		luckyDip(std::string name);//protected constructor prototype

		//card struct definition, used for chance/communityChest cards, contains a string message and a std::function wrapper to store a lambda function to act as the action of the card
		struct card{
			std::string message;
			std::function<void(const std::shared_ptr<player>&, gameBoard&)> action;//std::function wrapper, accepts any function-like object with a void return type that takes a const std::shared_ptr<player>& and a gameBoard& as parameters
			card(std::string mess, std::function<void(const std::shared_ptr<player>&, gameBoard&)> act);//constructor prototype for card
		};

		static void addCard(std::vector<card>& deck, std::string mess, std::function<void(const std::shared_ptr<player>&, gameBoard&)> act);//prototype method to create a card and add it to a vector of cards
		static void shuffleDeck(std::vector<card>& deck); //prototype method to randomly shuffle a vector of cards 
		void issueCard(std::vector<card>& deck, const std::shared_ptr<player>& plyr, gameBoard& board); //prototype method to take a card from a deck, read its message, perform its action and place it to the back of the deck

	public:
		virtual ~luckyDip();//virtual destructor prototype
	};

	//chance class definition
	class chance : public luckyDip{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use chance's private constructor
	private:
		chance();//private constructor prototype
		static std::vector<card> deck; //static member data stores the deck of chance cards shared by all chance spaces

		static std::vector<card> makeDeck();//prototype of static member function to initialise deck
	public:
		~chance();//destructor prototype
		void land(const std::shared_ptr<player>& plyr, gameBoard& board);//override of gameSpace's land() method, has a user take a card
	};

	//communityChest class definition
	class communityChest : public luckyDip{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use communityChest's private constructor
	private:
		communityChest();//private constructor prototype
		static std::vector<card> deck; //static member data stores the deck of communityChest cards shared by all communityChest spaces

		static std::vector<card> makeDeck();//prototype of static member function to initialise deck
	public:
		~communityChest();//destructor prototype
		void land(const std::shared_ptr<player>& plyr, gameBoard& board);//override of gameSpace's land() method, has a user take a card
	};

}//end of mnply namespace

#endif