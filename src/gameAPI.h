/*
 * gameAPI.h
 *
 *  Created on: Sep 2, 2017
 *      Author: elad
 */

#ifndef SRC_GAMEAPI_H_
#define SRC_GAMEAPI_H_
#include "TCPSocket.h"
#include <string>
#include "Handler.h"
#include "Protocol.h"
#include "MThread.h"
#include "ISDK.h"

using namespace std;

namespace networkingLab {

/**
 * client side SDK
 */
class gameAPI: public MThread, public ISDK{
	TCPSocket* serverConnectionSocket; // TCP based connection to server;
	string userName; // client userName
	string opponentUserName; // opponent userName
	string requesterUserName; // the opponent that sent game request userName
	volatile bool stopReciver;
	bool loggedIn; // logged in to server
	bool playing; // is playing a match

	// communication methods
	void sendCommandToServer(int cmd);
	void sendDataToServer(string msg);
	int readCommandFromServer();
	string readDataFromServer();

	bool charValidation(char ch);
	void run(); // MThread

public:
	// c'tor
	gameAPI(Handler* handler);

	/**
	 *  Existing user can login to the system with his username and password
	 */
	void login(string userName, string passWord);

	/**
	 * New user can register to the system with a username and password
	 */
	void registerr(string userName, string passWord);

	/**
	 * A user can send a request to start a game to another user
	 */
	void startGame(string userName);

	/**
	 * A user can automatically start a game with a random available user
	 */
	void autoStartGame();

	/**
	 * A user can accept a game request
	 */
	void acceptGameRequest();

	/**
	 * A user can reject a game request
	 */
	void declineGameRequest();

	/**
	 * The game application can end the game and declare its result
	 */
	void endGame(int status, int points);

	/**
	 * User can see the other users which are online and available to play with him
	 */
	void getUserList();

	/**
	 * After the game a user can see the score board and his position in
	 * it. The score board will order the users according to their accumulating score.
	 */
	void getScoreBoard();

	/**
	 * User can properly disconnect from server
	 */
	void disconnect();

	/**
	 * Run the game platform.
	 */
	void startGamePlatform();


	// methods
	bool isLoggedIn();
	bool isPlaying();

	virtual ~gameAPI();
};

} /* namespace networkingLab */

#endif /* SRC_GAMEAPI_H_ */
