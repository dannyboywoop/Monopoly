//Monopoly - by Daniel Holmes
/*
This is my attempt at a fully featured monopoly console application, using object oriented programming. The game allows 2-8 players to: roll dice, move around a game board, buy real estate, buy houses/hotels, mortgage properties, pay rent, trade, go to jail and more.
The game relies on run-time polymorphism to treat each space on the gameboard appropriately. Other advanced features implemented in this program include: static member data, templates, smart pointers and lambda functions.
*/

#include"monopoly.h"
#include"game.h"
#include"input.h"
#include<iostream>
#include<sstream>
#include<vector>
#include<string>

using namespace std;
using namespace mnply;
using namespace inputmethods;


//prints the game title to the screen in large characters
void displayTitle(){
	cout << "=============================================================================" << endl;
	cout << "=============================================================================" << endl;
	cout << " /$$      /$$                                                   /$$          " << endl;
	cout << "| $$$    /$$$                                                  | $$          " << endl;
	cout << "| $$$$  /$$$$  /$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ | $$ /$$   /$$" << endl;
	cout << "| $$ $$/$$ $$ /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$| $$| $$  | $$" << endl;
	cout << "| $$  $$$| $$| $$  \\ $$| $$  \\ $$| $$  \\ $$| $$  \\ $$| $$  \\ $$| $$| $$  | $$" << endl;
	cout << "| $$\\  $ | $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$| $$  | $$" << endl;
	cout << "| $$ \\/  | $$|  $$$$$$/| $$  | $$|  $$$$$$/| $$$$$$$/|  $$$$$$/| $$|  $$$$$$$" << endl;
	cout << "|__/     |__/ \\______/ |__/  |__/ \\______/ | $$____/  \\______/ |__/ \\____  $$" << endl;
	cout << "                                           | $$                     /$$  | $$" << endl;
	cout << "                                           | $$                    |  $$$$$$/" << endl;
	cout << "                                           |__/                     \\______/ " << endl;
	cout << "=============================================================================" << endl;
	cout << "=============================================================================" << endl << endl;
}

//takes an integer number of players and request a unique name for each of them from the user
vector<string> requestPlayerNames(int numOfPlayers){
	vector<string> names;//stores the names
	for (int i = 0; i < numOfPlayers; i++){//repeats until all names have been entered
		stringstream inputRequest;
		inputRequest << "Please enter the name of player " << i + 1 << " (one word only): ";
		string name;
		int playersWithSameName{ -1 };//stores the number of elements in the names vector equal to the inputted name
		while (playersWithSameName != 0){//repeats until the inputted name is not already in the vector of names
			name = takeInput<string>(inputRequest.str());//takes string input for name from user
			playersWithSameName = count(names.begin(), names.end(), name);
			if (playersWithSameName > 0) cout << "Error: Each player must have a unique name!" << endl;
		}
		names.push_back(name);//if name is unique, add to names vector
	}
	return names;//return vector of names
}

//main program
int main(){
	SetConsoleTextAttribute(hConsole, 15);//set font colour to white in the console
	SetConsoleTitle(TEXT("Monopoly 1.0 - by Daniel Holmes"));//set console title line to the string listed
	
	displayTitle();

	try{
		int numberOfPlayers{ takeInRange<int>("Please enter the number of players (2-8): ", 2, 8) }; //request the number of players from the user
	
		vector<string> nameList{ requestPlayerNames(numberOfPlayers) }; //assign each player a unique name, inputted by the user

		cout << "Great! Launching the Game!" << endl;

		game newGame(nameList);//attempt to create a new game with the players listed in nameList
	}
	catch (bad_alloc &ba){//may be thrown by game constructor, which invokes dynamic memory allocation
		cerr << "bad_alloc: " << ba.what() << endl;
		cout << "Game could not be launched!" << endl;
		return 1;
	}
	catch (invalid_argument &iv){//may be thrown by takeInRange<int> or game constructor
		cerr << "invalid_argument: " << iv.what() << endl;
		cout << "Game could not be launched!" << endl;
		return 1;
	}
	
	return 0;
}