//header guard
#ifndef GAME_H
#define GAME_H

#include"player.h"
#include"gameBoard.h"
#include"dice.h"
#include<memory>
#include<vector>
#include<string>
#include<set>

namespace mnply{

	//definition of game class
	class game{
	private:
		dice diceSet;
		gameBoard board;
		const int startingMoney; //stores money each player starts with
		std::vector<std::shared_ptr<player>> players; //stores shared_ptrs to all of the players in the game
		int numOfPlayers;
		int currentPlayer;//stores index in players vector of player whos turn it is
		bool gameOver;//bool tracks whether the game has finished or not
	public:
		//constructor and destructor prototypes
		game(const std::vector<std::string>& playerNames);
		~game();

		//method prototypes
		void initialize(const std::vector<std::string>& playerNames);//fills players vector, assigns each player a token and rolls to determine which player starts the game
		int rollForFirst(std::set<int> inTheRunning);//recursively defined method to determine highest roller, who will take the first turn
		void runGame();//runs the game loop
		void displayTurnOptions(bool rollsLeft)const;//displays user choices for each turn
		void takeTurn(bool trnOver);//takes users turn choices
		void displayJailOptions()const;//displays user choices for each turn spent in jail
		void takeJailTurn();//takes users turn choices whilest in jail
		int countBankruptedPlayers()const;//returns the number of players that are currently bankrupt
		void endGame(); //ends the game, displaying the winner (or remaining players if the game was aborted early) along with their inventories
	};

}//end of mnply nameSpace

#endif