//header guard
#ifndef DICE_H
#define DICE_H

namespace mnply{

	//definition of dice class
	class dice{
	private:
		int doublesRolled;//stores number of consecutive doubles rolled
		int rollDie();//private method to generate a random integer uniformly distributed between 1 and 6
	public:
		//constructor and destructor prototypes
		dice()noexcept;
		~dice();

		//method prototypes
		int getDoublesRolled()const; //returns doublesRolled
		void resetDoublesRolled(); //sets doublesRolled to 0
		int rollDice(); //method to roll two dice, print their results and return their total
	};

}//end of mnply namespace

#endif