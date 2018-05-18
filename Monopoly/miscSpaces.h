//header guard
#ifndef SPECIAL_SPACES_H
#define SPECIAL_SPACES_H

#include"gameSpace.h"
#include<string>
#include<memory>

//THIS FILE CONTAINS THE DEFINITIONS FOR MULTIPLE SMALL CLASSES: GO, jail, goToJail, freeParking and tax

namespace mnply{
	
	//GO class definition
	class GO : public gameSpace{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use GO's protected constructor
	private:
		static const int goReward;//amount given to player upon passing go
		GO();//private constructor prototype
	public:
		~GO();//destructor prototype
		static void passGo(const std::shared_ptr<player>&plyr);//static function to reward player for passing go
		void land(const std::shared_ptr<player>& plyr, gameBoard& board); //override of gameSpace's land method
	};

	//jail class definition
	class jail : public gameSpace{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use jail's private constructor
	private:
		jail();//private constructor prototype
	public:
		~jail();//destructor prototype
		void land(const std::shared_ptr<player>& plyr, gameBoard& board); //override of gameSpace's land method
	};

	//goToJail class definition
	class goToJail : public gameSpace{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use goToJail's private constructor
	private:
		goToJail();//private constructor prototype
	public:
		~goToJail();//destructor prototype
		void land(const std::shared_ptr<player>& plyr, gameBoard& board); //override of gameSpace's land method
	};

	//freeParking class definition
	class freeParking : public gameSpace{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use freeParking's private constructor
	private:
		freeParking();//private constructor prototype
	public:
		~freeParking();//destructor prototype
		void land(const std::shared_ptr<player>& plyr, gameBoard& board); //override of gameSpace's land method
	};

	//tax class definition
	class tax : public gameSpace{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use tax's private constructor
	private:
		int charge;
		tax(std::string name, int amount);//private constructor prototype
	public:
		~tax();//destructor prototype
		void land(const std::shared_ptr<player>& plyr, gameBoard& board); //override of gameSpace's land method
	};

}//end of mnply namespace

#endif