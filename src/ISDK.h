/*
 * ISDK.h
 *
 *  Created on: Sep 6, 2017
 *      Author: elad
 */

#ifndef SRC_ISDK_H_
#define SRC_ISDK_H
#include <string>
#include "Handler.h"

using namespace std;

namespace networkingLab {

class ISDK {
protected:
	Handler* handler;
public:
	ISDK(Handler* handler){
		this->handler = handler;
	}
	/**
	 *  Existing user can login to the system with his username and password
	 */
	virtual void login(string userName, string passWord) =0;

	/**
	 * New user can register to the system with a username and password
	 */
	virtual void registerr(string userName, string passWord) =0;

	/**
	 * A user can send a request to start a game to another user
	 */
	virtual void startGame(string userName) =0;

	/**
	 * A user can automatically start a game with a random available user
	 */
	virtual void autoStartGame() =0;

	/**
	 * A user can accept a game request
	 */
	virtual void acceptGameRequest() =0;

	/**
	 * A user can reject a game request
	 */
	virtual void declineGameRequest() =0;

	/**
	 * The game application can end the game and declare its result
	 */
	virtual void endGame(int status, int points) =0;

	/**
	 * User can see the other users which are online and available to play with him
	 */
	virtual void getUserList() =0;

	/**
	 * After the game a user can see the score board and his position in
	 * it. The score board will order the users according to their accumulating score.
	 */
	virtual void getScoreBoard() =0;

	/**
	 * User can properly disconnect from server
	 */
	virtual void disconnect() =0;

	/**
	 * Run the game platform.
	 */
	virtual void startGamePlatform() =0;


	// methods
	virtual bool isLoggedIn() =0;
	virtual bool isPlaying() =0;

	virtual ~ISDK() {
	}
};

} /* namespace networkingLab */

#endif /* SRC_ISDK_H_ */
