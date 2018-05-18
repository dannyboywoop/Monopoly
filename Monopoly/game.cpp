#include"game.h"
#include"input.h"
#include"monopoly.h"
#include<algorithm>
#include<set>
#include<iostream>

using namespace std;
using namespace mnply;
using namespace inputmethods;

//game constructor
game::game(const vector<string>& playerNames) :diceSet(), board(static_cast<int>(playerNames.size())),startingMoney(1500),numOfPlayers(static_cast<int>(playerNames.size())),currentPlayer(0),gameOver(false){
	if (playerNames.size() == 0) throw invalid_argument("Cannot construct game object using empty vector of playerNames");//throw an exception if inTheRunning is an empty set
	try{//catches exceptions thrown in the initialize function and avoids running the game after bad initialization
		initialize(playerNames);//initialize the game
		runGame();//start the game
	}
	catch (domain_error &de){//possibly thrown by player constructor
		cerr << "domain_rror: " << de.what() << endl;
		cout << "Game Aborted!" << endl;
	}
	catch (bad_alloc &ba){//possibly thrown by dynamic allocation of memory to players
		cerr << "bad_alloc: " << ba.what() << endl;
		cout << "Game Aborted!" << endl;
	}
	catch (invalid_argument &ia){//possibly thrown by rollForFirst() and assignTokens() inside initialize()
		cerr << "invalid_argument: " << ia.what() << endl;
	}
}

//game destructor
game::~game(){}

//fills players vector, assigns each player a token and rolls to determine which player starts the game
void game::initialize(const vector<string>& playerNames){
	//iterate over playerNames vector
	for_each(playerNames.begin(), playerNames.end(), [&](string name){
		players.push_back(make_shared<player>(name, startingMoney));//create a shared_ptr to a new player with name from playerNames and startingMoney, add this player to the players vector
	});
	board.assignTokens(players);//assign each player a token on the gameBoard (this links each token on 'board' with a shared_pointer to a player from 'game', allowing easier interfacing between game and gameBoard methods)

	//create a set of ints, setOfPlayerIndices, which will store all the integers 0 up to numOfPlayers-1
	set<int> setOfPlayerIndices;
	for (int i = 0; i < numOfPlayers; i++) setOfPlayerIndices.insert(i);//fill setOfPlayerIndices
	cout << "Let's roll to see who goes first!" << endl << endl;
	currentPlayer = rollForFirst(setOfPlayerIndices);//pass setOfPlayerIndices to the rollForFirst method to establish who will start the game
	
}

//recursively defined method, taking a set of player indices as a parameter, to have players roll to see who starts the game (highest roller starts)
int game::rollForFirst(set<int> inTheRunning){
	if (inTheRunning.size() == 0) throw invalid_argument("Cannot pass an empty set to rollForFirst()!");//throw an exception if inTheRunning is an empty set
	int highestRoll{ 0 };
	set<int> highRollers;//stores the indices of players who rolled the current value for highestRoll

	//iterate over the the indices stored in the inTheRunning set
	for_each(inTheRunning.begin(), inTheRunning.end(),[&](int playerIndex){
		//ask the player correspinding to the current index to roll
		cout << players[playerIndex]->getName() << ": It's your turn to roll the dice!" << endl;
		pauseForUser();
		int roll{ diceSet.rollDice() };//roll the dice
		if (roll > highestRoll) {//if they have rolled the new highestRoll
			highestRoll = roll; //update highestRoll
			highRollers.clear(); //remove any other players currently in the highRollers set
			highRollers.insert(playerIndex); //add the new high roller to the highRollers set
		}
		else if(roll==highestRoll){//if the player matched the current highestRoll
			highRollers.insert(playerIndex);//add the player to the highRollers set
		}
	});
	if (highRollers.size() > 1){//if more than one player rolled the final highest roll
		//run the rollForFirst method on the subset of players who rolled the joint highest roll
		cout << "More than one high roller! Roll again to see who starts!" << endl << endl;
		return rollForFirst(highRollers);
	}
	else{//if only one player rolled the highest roll
		cout << players[*highRollers.begin()]->getName() << ": You rolled the highest, so you will start the game!" << endl; //output the player with the with the highest roll
		diceSet.resetDoublesRolled();//reset the dice set's doublesRolled counter
		return *highRollers.begin();//return the highest roller (terminating the recursive stack)
	}
}

//method containing the game loop
void game::runGame(){
	gameOver = false;
	while (!gameOver){//gameLoop (runs until gameOver)
		if (!players[currentPlayer]->isBankrupt()){//if the current player is not bankrupt
			if (players[currentPlayer]->inJail()) takeJailTurn();//take their turn in jail, if they are in jail
			else takeTurn(true);//else take a normal turn
		}
		if (numOfPlayers - countBankruptedPlayers() <= 1){//check there is more than one player remaining, else end the game
			gameOver = true;
		}
		currentPlayer++; //set the currentPlayer to the next player;
		if (currentPlayer == numOfPlayers) currentPlayer = 0;//if end of players vector reached, loop back to the start
	}
	endGame();//once gameLoop has finished, run endGame() method
}

//print list of options for a player to take on their turn, rollsLeft passed as a parameter lets the method know whether the player still needs to roll the dice
void game::displayTurnOptions(bool rollsLeft)const{
	cout << players[currentPlayer]->getName() << ": It's your turn!" << endl;//print current players name
	cout << "-----------------------------------------------------------" << endl;
	cout << "1: Display the Game Board" << endl;
	cout << "2: See your Inventory" << endl;
	cout << "3: Manage your Inventory" << endl;
	cout << "4: Make a trade" << endl;
	if (rollsLeft){
		cout << "r: Roll the Dice (consecutive doubles rolled = " << diceSet.getDoublesRolled() << ")"<<endl;
	}
	else{
		cout << "e: End your turn!" << endl;
	}
	cout << "x: End the Game!" << endl;
	cout << "-----------------------------------------------------------" << endl;
}

//method provides user with a list of possible choices to make on their turn
void game::takeTurn(bool rollsRemaining){//rollsRemaining allows takeTurn to know whether the player has rolls left to take when takeTurn is initiated
	bool rollsLeft{ rollsRemaining };//stores whether the player has rolls left to make
	
	auto noMoreRolls = [&](list<char>& optionsList){//define lambda function as local variable noMoreRolls() to run when the user has no rolls left
		//remove options to roll from optionsList and add options to end the players turn
		optionsList.remove('r');
		optionsList.remove('R');
		optionsList.push_back('e');
		optionsList.push_back('E');
		diceSet.resetDoublesRolled();
	};

	auto roll = [&](){//define lambda function as local variable roll() to allow user to roll the dice and act upon the result based on the number of consecutive doubles rolled
		int move{ diceSet.rollDice() };//stores result of dice roll
		if (diceSet.getDoublesRolled() == 3){//if three doubles rolled, send the player to jail
			cout << "You rolled 3 doubles in a row! Go to Jail!" << endl;
			try{
				board.moveToJail(players[currentPlayer]);//move the players token straight to jail
				players[currentPlayer]->setJailed(true);//set player to jailed state
			}
			catch (invalid_argument &ia){
				cerr << "invalid_argument: " << ia.what() << endl;
			}
			rollsLeft=false;
			return; //exit roll()
		}
		if (diceSet.getDoublesRolled() == 0){//if the player didn't roll a double, set rollsLeft to 0
			rollsLeft = false;
		}
		try{
			board.advancePlayer(players[currentPlayer], move);//advance the current players token along 'board' by 'move' spaces
		}
		catch (invalid_argument &ia){
			cerr << "invalid_argument: " << ia.what() << endl;
		}
	};

	bool turnEnded{ players[currentPlayer]->isBankrupt() };//if player isn't bankrupt, turnEnded is false;
	if (players[currentPlayer]->isBankrupt()) turnEnded = true;
	while (!turnEnded){//repeats until player chooses to end their turn
		list<char> options{ '1', '2', '3', '4', 'r', 'R', 'x', 'X' };//list of acceptable user inputs
		if (players[currentPlayer]->inJail()) rollsLeft = false;
		if (!rollsLeft) noMoreRolls(options);//if the user has no rollsLeft run 'noMoreRolls' on 'options'
		displayTurnOptions(rollsLeft);//display player's options
		char choice = takeChoice<list, char>("Please select one of the options above: ", options);//take player choice from options
		switch (choice){
		case '1':
			board.displayBoard();//show current state of gameBoard
			break; 
		case '2':
			players[currentPlayer]->printInventory();//display players inventory
			break; 
		case '3':
			players[currentPlayer]->manageInventory();//manage players inventory
			break;
		case '4':
			players[currentPlayer]->trade(players);//allow player to trade with other players
			break;
		case 'r':
		case 'R':
			roll(); //allow player to roll the dice
			break; 
		case 'e':
		case 'E':
			turnEnded = true; //end players turn
			break; 
		case 'x':
		case 'X':
			turnEnded = true;//end players turn
			gameOver = true; //end the game
			break; 
		}
		if(players[currentPlayer]->isBankrupt()) turnEnded = true;
	}
}

//print list of options for a player to take on their turn whilest in jail. breakOutAttempts parameter tells the method how many previous turns the player has spent
void game::displayJailOptions()const{
	cout << players[currentPlayer]->getName() << ": It's your turn , but you're in Jail!" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "1: Display the Game Board" << endl;
	cout << "2: See your Inventory" << endl;
	cout << "3: Manage your Inventory" << endl;
	cout << "4: Make a trade" << endl;
	cout << "5: Pay " << pound << "50 to leave Jail" << endl;
	cout << "6: Use a Get out of Jail Free card (" << players[currentPlayer]->countGetOutOfJailFreeCards() << " - owned)" << endl;
	int breakOutAttempts{ players[currentPlayer]->getTurnsInJail() };
	if (breakOutAttempts<3){//if the player has spent less than 3 turns in jail, allow them to roll for doubles in an attempt to escape
		cout << "r: Roll for doubles (" << 3 - breakOutAttempts << " attempts remaining!)" << endl;
	}
	cout << "x: End the Game!" << endl;
	cout << "-----------------------------------------------------------" << endl;
}

//method provides user with a list of possible choices to make on their turn, whilest in jail
void game::takeJailTurn(){
	list<char> options{ '1', '2', '3', '4', '5', '6', 'r', 'R', 'x', 'X' }; //list of acceptable user inputs
	bool turnOver{ false }; //stores whether the players turn in jail is over

	auto releasePlayer = [&](){//define lambda function as local variable to release player from jail
		players[currentPlayer]->setJailed(false);
		players[currentPlayer]->resetTurnsInJail();
		turnOver = true;
		takeTurn(true);//take turn outside of jail with rolls left to make
	};

	auto rollForDoubles = [&](){//define lambda function as local variable to allow player to attempt to roll for doubles, in order to escape jail
		int roll{ diceSet.rollDice() };//roll dice
		players[currentPlayer]->incrementTurnsInJail();
		if (diceSet.getDoublesRolled() == 1){//if the player rolled doubles
			cout << "Congrats! You rolled doubles, you're free!" << endl;
			try{
				board.advancePlayer(players[currentPlayer], roll);//advance the player's toke by the amount rolled
				releasePlayer();//release the player from jail
			}
			catch (invalid_argument &ia){
				cerr << "invalid_argument: " << ia.what() << endl;
			}
		}
		else{//if the player didnt roll doubles
			cout << "Your breakout attempt failed! Better luck next time!" << endl;
			if (players[currentPlayer]->getTurnsInJail() < 3){//if the player has had less than three attempts at escape
				turnOver = true;//end their turn in jail
				takeTurn(false);//provide the player with normal turn options with no rolls remaining
			}
		}
	};

	while (!turnOver){//repeats until players jail turn is over
		if (players[currentPlayer]->getTurnsInJail() == 3){//if the player has already had 3 attempts at escape, remove the option to roll for doubles
			options.remove('r');
			options.remove('R');
		}
		displayJailOptions();//display players choices
		char choice = takeChoice<list, char>("Please select one of the options above: ", options);//take players choice from options
		switch (choice){
		case '1':
			board.displayBoard();//display the current state of the board
			break;
		case '2':
			players[currentPlayer]->printInventory(); //display the players inventory
			break;
		case '3':
			players[currentPlayer]->manageInventory(); //manage the players inventory
			break;
		case '4':
			players[currentPlayer]->trade(players); //allow player to trade
			break;
		case '5':
			players[currentPlayer]->takeMoney(50); //charge the player £ and release them from jail
			releasePlayer();
			break;
		case '6':
			if (players[currentPlayer]->countGetOutOfJailFreeCards() > 0){//if the player has atleast one Get Out of Jail Free card
				players[currentPlayer]->takeGetOutOfJailFreeCard();//take a Get Out of Jail free card from the player and release them from jail
				releasePlayer();
			}
			else{//otherwise output that the user has no Get Out of Jail Free Cards
				cout << "You have no Get out of Jail Free cards!" << endl;
			}
			break;
		case 'r':
		case 'R':
			rollForDoubles();//attempt to roll for doubles
			break;
		case 'x':
		case 'X':
			turnOver = true;//end turn
			gameOver = true;//end the game
			break;
		}
	}
}

//returns the number of players in the game that are currently bankrupt
int game::countBankruptedPlayers()const{
	//iterate over the players vector and count players that are bankrupt, storing the result in 'bankrupted'
	int bankrupted = static_cast<int>(count_if(players.begin(), players.end(), [&bankrupted](const shared_ptr<player>& plyr){
		return plyr->isBankrupt();
	}));
	return bankrupted;
}

//ends the game, displaying the winner (or remaining players if the game was aborted early) along with their inventories
void game::endGame(){
	cout << "Game Over!" << endl;
	if (numOfPlayers - countBankruptedPlayers() == 1){//if there is only one player left not bankrupt
		cout << "Winner:" << endl;//state there was a winner
	}
	else{//otherwise state the surviving players
		cout << "Surviving players:" << endl;
	}
	//output the inventories of any players that have not gone bankrupt
	for (int i = 0; i < numOfPlayers; i++){
		if (!players[i]->isBankrupt()) players[i]->printInventory();
	}
}




