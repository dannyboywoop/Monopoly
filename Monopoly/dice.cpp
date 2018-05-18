#include"dice.h"
#include<random>
#include<iostream>

using namespace std;
using namespace mnply;

//dice constructor and destructor
dice::dice(){}
dice::~dice(){}

//accessor method for doublesRolled
int dice::getDoublesRolled()const{
	return doublesRolled;
}

//sets doubles rolled to 0
void dice::resetDoublesRolled(){
	doublesRolled = 0;
}

//private dice method, returns a random integer uniformly distributed between 1 and 6
int dice::rollDie(){
	random_device rd; //create a new random device rd
	default_random_engine generator(rd());//generates uniformly distributed numbers using rd as a seed;
	uniform_int_distribution<int> distribution(1, 6);
	return distribution(generator);//converts the randomly generated number into the desired distribution
}

//method to roll two dice, print their results and return their total
int dice::rollDice(){
	//get two random dice rolls
	int die1{ rollDie() };
	int die2{ rollDie() };
	//print the dice values and total
	cout << "Rolled: [" << die1 << "][" << die2 << "]" << " : Total = " << die1 + die2 << endl << endl;
	if (die1 == die2){//if doubles rolled
		doublesRolled++;//increment doublesRolled
	}
	else{//otherwise reset doublesRolled to 0
		doublesRolled = 0;
	}
	return die1 + die2;//return total
}