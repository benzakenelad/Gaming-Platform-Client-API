/*
 * Record.h
 *
 *  Created on: Sep 2, 2017
 *      Author: elad
 */

#ifndef SRC_RECORD_H_
#define SRC_RECORD_H_
#include <string>
#include <iostream>

using namespace std;

namespace networkingLab {

class Record {
	string userName;
	int score;
public:
	Record(string userName, int score){
		this->userName = userName;
		this->score = score;
	}
	string getUserName(){
		return userName;
	}
	int getScore(){
		return score;
	}
};

} /* namespace networkingLab */

#endif /* SRC_RECORD_H_ */
