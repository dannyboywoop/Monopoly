#include "input.h"
#include<iostream>
#include<string>

using namespace std;

void inputmethods::pauseForUser(){
	cout << "{Hit Return to Continue}";
	string inputLine;
	getline(cin,inputLine);
}
