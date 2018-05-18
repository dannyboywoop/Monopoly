//header guard
#ifndef INPUT_H
#define INPUT_H

#include<string>
#include<iostream>

namespace inputmethods{

	//template function to repeated ask user for input of type T using 'inputRequest', until valid input of type T is entered. operator<< must be defined for type T
	template <class T> T takeInput(std::string inputRequest){
		T input;
		bool validInput{ false };
		while (!validInput){//runs atleast once, repeats until an input of the valid type is entered
			std::cout << inputRequest;//request input
			std::cin >> input;//attempts to take input
			if (!std::cin.fail() && (std::cin.peek() == '\n')){//check that the attempt to take input succeeded and there are no further characters in the input stream
				validInput = true;//if so input was of valid type T
			}
			else{//triggered if the stream doesn't begin with the correct type, or the correct type was entered but there were extra characters in the stream
				std::cout << std::endl << "Error: Invalid input type!" << std::endl;
				std::cin.clear();//resets fail flag if it was triggered
				while (std::cin.peek() != '\n'){//clears characters from the stream until the end of line character is reached
					std::cin.ignore();
				}
			}
		}
		std::cin.ignore();//ignores end of line character remaining in stream
		std::cout << std::endl;
		return input;//returns input of valid type T
	}

	//template function that request user input (using inputRequest) of type T within a given range: <, <= and >= must be defined for type T
	template <class T> T takeInRange(std::string inputRequest, T lowerBound, T upperBound){
		if (upperBound < lowerBound) throw std::invalid_argument("Upper bound < lower bound!");//if upperBound is lower than lowerBound, throw an exception
		T value;//stores inputted value
		bool valueAccepted{ false };
		while (!valueAccepted){//repeats until an input is taken within the acceptable range
			value = takeInput<T>(inputRequest);//take input of type T, using inputRequest
			if (value >= lowerBound && value <= upperBound) {//if in the acceptable range
				valueAccepted = true;//value is accepted
			}
			else{//otherwise, output a message that the value was outside the acceptable range and repeat
				std::cout << "Error: Input outside of acceptable range!" << std::endl;
			}
		}
		return value;//once a valid input has been take, return it
	}

	//template takes input of type T and compares it to a sequencial container<T> of acceptable input options, repeats until an acceptable option is chosen
	//container must be a template with two template parameters: type T and allocator A. When instantiating this template, A does not need to be specified due to type deduction
	template <template<class, class> class C, class T, class A> T takeChoice(std::string inputRequest, const C<T, A>& optionsContainer){
		T input;//stores user input
		bool isOption{ false };//bool, stores whether the inputted value is one of the acceptable options
		while (!isOption){//repeats until one of the options in optionsContainer is entered by the user
			input = takeInput<T>(inputRequest);//request input of type T from the user using inputRequest
			//count the number of occurrences of 'input' in the optionsContainer. If equal to zero, input is not a valid option
			if (count(optionsContainer.begin(), optionsContainer.end(), input) == 0){
				std::cout << "Error: Not a valid option!" << std::endl;
			}
			else{//if input was one of the valid options
				isOption = true;
			}
		}
		return input;
	}

	//prototype for function pauses the program until a new line is entered by the user
	void pauseForUser();

}//end of inputmethods namespace

#endif