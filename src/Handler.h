/*
 * Handler.h
 *
 *  Created on: Sep 2, 2017
 *      Author: elad
 */

#ifndef SRC_HANDLER_H_
#define SRC_HANDLER_H_
#include <string>
#include <iostream>
#include <vector>
#include "Record.h"
#include "UDPMSN.h"


using namespace std;

namespace networkingLab {

/**
 * Handling server answers
 */
class Handler {
protected:
	UDPMSN* msn; // p2p UDP msn for game platform
	bool iStartTheGame; // true if this client go first
public:
	Handler(){ this->msn = NULL; this->iStartTheGame = false;} // c'tor

	/**
     * In case of login answer
	 */
	virtual void onLogin(bool answer) =0;

	/**
	 * In case of registration answer
	 */
	virtual void onRegister(bool answer) =0;

	/**
	 * In case the username we enter is already logged in
	 */
	virtual void onUserNameAlreadyLoggedIn(string userName) = 0;

	/**
	 * In case received A game request
	 */
	virtual void onReciveGameRequest(string opponentUserName) =0;

	/**
	 * In case the server failed to start a game
	 */
	virtual void onFailedToStartGame() =0;

	/**
	 * In case there is no available player
	 */
	virtual void onNoAvailablePlayer() =0;

	/**
	 * In case the opponent accepted the game request
	 */
	virtual void onGameStarted(string opponentUserName) =0;

	/**
	 * In case the opponent declined the game request
	 */
	virtual void onGameRquestDeclined(string opponentUserName) =0;

	/**
	 * score = WINNER / LOSER / DRAW / CANCELED
	 * In end of a match we receive all the details
	 */
	virtual void onEndOfGame(string player1, string player2, int score, int points) =0;

	/**
	 * Vector of available players username
	 */
	virtual void onGetListOfUsers(vector<string> users) =0;

	/**
	 * Vector of Record object
	 * Record object contains userName and score record
	 */
	virtual void onGetScoreBoard(vector<Record> records) =0;

	/**
	 * When disconnected from server
	 */
	virtual void onDisconnect() =0;

	/**
	 * When server no recognize the command we sent him
	 */
	virtual void onInvalidCommand(int num) =0;

	/**
	 * return 2 integers,
	 * 1 is the status, WINNER / LOSER / DRAW / CANCELED
	 * 2 is the amount of points
	 */
	virtual int* gamePlatfrom() =0;

	/**
	 * setters and getters
	 */
	void setUDPMSN(UDPMSN* msn){
		this->msn = msn;
	}
	UDPMSN* getUDPMSN(){
		return this->msn;
	}
	void IsStartintgTheGame(bool iStartTheGame){
		this->iStartTheGame = iStartTheGame;
	}


	virtual ~Handler(){}
};

} /* namespace networkingLab */

#endif /* SRC_HANDLER_H_ */
