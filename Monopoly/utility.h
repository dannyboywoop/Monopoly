//header guard
#ifndef UTILITY_H
#define UTILITY_H

#include"realEstate.h"
#include<string>
#include<memory>

namespace mnply{

	//definition of utility class
	class utility : public realEstate{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use utilities's private constructor
	private:
		utility(groupEnum set, std::string name, int purchasePrice);//private constructor prototype
	public:
		~utility();//destructor prototype
		void chargeRent(const std::shared_ptr<player>& plyr)const;//prototype of chargeRent override
	};
	
}//end of mnply namespace

#endif