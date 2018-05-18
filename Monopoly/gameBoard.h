//header guard
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include"gameSpace.h"
#include<vector>
#include<memory>
#include<string>

namespace mnply{

	//global constant
	const int realEstateInGroup[10]{2, 3, 3, 3, 3, 3, 3, 2, 2, 4};//stores the number of spaces of each realEstate group found on the monopolyBoard

	//gameBoard class definition
	class gameBoard{
	private:
		std::vector<std::shared_ptr<gameSpace>> monopolyBoard;//vector of gameSpace base class pointers
		struct token{
			int position;//position of token, corresponds to index on monopolyBoard
			std::shared_ptr<player> tokenOwner;//shared_ptr to the player assigned to the token
			token(int pos); //token constructor prototype
		};
		const int boardSize;//stores number of spaces on board
		std::vector<token> tokens;

		int getSpaceIndex(std::string gameSpaceName)const;//private method that takes a string name of a gameSpace and returns the index of that gameSpace on the monopolyBoard
		int getTokenIndex(const std::shared_ptr<player>& plyr)const;//private method that takes a shared_ptr to a player and returns the index of the players token in the tokens vector
	public:
		//constructor and destructor prototypes
		gameBoard(int numOfPlayers);
		~gameBoard();

		//method prototypes
		void displayBoard()const;//outputs the current state of the monopoly board
		void assignTokens(const std::vector<std::shared_ptr<player>>& playerList);//takes a vector of shared_ptrs to players and copies each element to the tokenOwner member of a token in tokens
		void advancePlayer(const std::shared_ptr<player>& plyr, int spaces); //given a pointer to a player and a number of spaces, that player's token is moved that number of spaces around the board
		void movePlayer(const std::shared_ptr<player>& plyr, std::string destinationName);//given a pointer to a player and destination name, the players token is advanced to destination on the board
		void moveToJail(const std::shared_ptr<player>& plyr);//takes a pointer to a player and moves that player directly to jail, without passing GO
	};

}//end of mnply namespace

#endif