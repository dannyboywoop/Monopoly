//header guard
#ifndef MONOPOLY_H
#define MONOPOLY_H

#include<Windows.h>

namespace mnply{

	//global constants
	const HANDLE hConsole{ GetStdHandle(STD_OUTPUT_HANDLE) };//handle to standard output console, allows for colour of output text to be changed
	const char pound{ '\x9C' };//£ symbol, used for widely when outputting amounts of money

}//end of mnply namespace

#endif