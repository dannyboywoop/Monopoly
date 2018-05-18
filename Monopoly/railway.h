//header guard
#ifndef RAILWAY_H
#define RAILWAY_H

#include"realEstate.h"
#include<string>
#include<memory>

namespace mnply{
	
	//definition of railway class
	class railway : public realEstate{
		friend class gameBoard;//declares gameBoard as friend class to allow it to use railway's private constructor
	private:
		railway(groupEnum set, std::string name, int purchasePrice);//private constructor prototype
	public:
		~railway();//destructor prototype
		void chargeRent(const std::shared_ptr<player>& plyr)const;//prototype of chargeRent override
	};

}//end of mnply namespace

#endif