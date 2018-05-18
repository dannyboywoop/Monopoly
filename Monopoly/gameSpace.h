//header guard
#ifndef GAME_SPACE_H
#define GAME_SPACE_H

#include<string>
#include<memory>
#include<iostream>

namespace mnply{

	//forward declaration of player class to allow gameSpace::land() and gameSpace::printLandMessage() to take pointers to players as parameters
	class player;
	class gameBoard;
	
	//definition abstract base class: gameSpace
	class gameSpace{
		friend class gameBoard;//gameBoard declared as friend, allows it access to gameSpace's protected constructor
		friend std::ostream& operator<<(std::ostream& os, const gameSpace& gs); //operator<< overload for gameSpaces declared as friend
	protected:
		std::string name;
		gameSpace(std::string spaceName); //constructor protected so that only the gameBoard class and derived classes can create gameSpace objects
	public:
		//virtual destuctor prototype
		virtual ~gameSpace();

		//method prototypes
		std::string getName()const;//name accessor prototype
		void printLandMessage(const std::shared_ptr<player>& plyr)const;//prints a message telling the player passed to it what gameSpace they have landed on
		virtual void printDetails(std::ostream& os)const;//virtual function that takes a reference to an output stream and prints details of the calling object to it
		virtual void land(const std::shared_ptr<player>& plyr, gameBoard& board) = NULL; //pure virtual function to run when a gameSpace is landed on
	};

}//end of mnply namespace

#endif