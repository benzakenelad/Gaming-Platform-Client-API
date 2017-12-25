/*
 * gameAPI.cpp
 *
 *  Created on: Sep 2, 2017
 *      Author: elad
 */

#include "gameAPI.h"
#define BUFFER 512
#include "Record.h"

namespace networkingLab {

gameAPI::gameAPI(Handler* handler) : ISDK(handler) {

	if (handler == NULL)
		perror("NULL HANDLER EXCEPTION \n");

	serverConnectionSocket = new TCPSocket();
	serverConnectionSocket->connectt(SERVER_IP, PORT);
	this->handler->setUDPMSN(new UDPMSN(serverConnectionSocket->getPort()));
	stopReciver = false;
	loggedIn = false;
	playing = false;
	this->start();
}

void gameAPI::run() {
	string player1;
	string player2;
	string IP;
	int port;
	int n = 0, score = 0, points = 0;

	while (!stopReciver) {

		vector<string> strArr;
		vector<Record> recArr;

		int cmd = readCommandFromServer();

		switch (cmd) {
		case LOGIN_FAILED:
			loggedIn = false;
			userName = "";
			handler->onLogin(false);
			break;
		case LOGIN_SUCCEED:
			this->userName = readDataFromServer();
			loggedIn = true;
			handler->onLogin(true);
			break;
		case REGISTER_FAILED:
			handler->onRegister(false);
			break;
		case REGISTER_SUCCEED:
			handler->onRegister(true);
			break;
		case FAILED_TO_START_GAME:
			handler->onFailedToStartGame();
			break;
		case NO_AVAILABLE_PLAYER:
			handler->onNoAvailablePlayer();
			break;
		case GAME_REQUEST:
			requesterUserName = readDataFromServer();
			handler->onReciveGameRequest(requesterUserName);
			break;
		case GAME_STARTED:
			opponentUserName = readDataFromServer();
			handler->onGameStarted(opponentUserName);
			IP = readDataFromServer();
			port = readCommandFromServer();
			cmd = readCommandFromServer();
			if (cmd == 1)
				handler->IsStartintgTheGame(true);
			else
				handler->IsStartintgTheGame(false);
			requesterUserName = "";
			handler->getUDPMSN()->connectt(IP, port); // set the opponent p2p data
			playing = true;

			break;
		case DECLINE_GAME_REQUEST:
			requesterUserName = readDataFromServer();
			handler->onGameRquestDeclined(requesterUserName);
			requesterUserName = "";
			break;
		case LIST_OF_USERS:
			n = readCommandFromServer();
			for (int i = 0; i < n; i++)
				strArr.push_back(readDataFromServer());
			handler->onGetListOfUsers(strArr);
			break;
		case SCORE_BOARD:
			n = readCommandFromServer();
			for (int i = 0; i < n; i++) {
				string userName = readDataFromServer();
				int score = readCommandFromServer();
				Record record(userName, score);
				recArr.push_back(record);
			}
			handler->onGetScoreBoard(recArr);
			break;
		case ALLREADY_LOGGED_IN:
			player1 = readDataFromServer();
			handler->onUserNameAlreadyLoggedIn(player1);
			break;
		case END_GAME:
			player1 = readDataFromServer();
			player2 = readDataFromServer();
			score = readCommandFromServer();
			points = readCommandFromServer();
			handler->onEndOfGame(player1, player2, score, points);
			playing = false;
			opponentUserName = "";
			requesterUserName = "";
			break;
		case DISCONNECT:
			handler->onDisconnect();
			usleep(300000);
			serverConnectionSocket->closee();
			stopReciver = true;
			continue;
			break;
		default:
			handler->onInvalidCommand(cmd);
			break;
		}
	}
}

void gameAPI::login(string userName, string passWord) {
	if (loggedIn == true) {
		cout << "You are already logged in" << endl;
		return;
	}
	int userLen = userName.length();
	int passWordLen = passWord.length();
	if (userLen <= 0 || passWordLen <= 0) {
		cout << "Illegal userName" << endl;
		return;
	}
	for (int i = 0; i < userLen; i++)
		if (charValidation(userName[i]) == false) {
			cout << "Illegal userName" << endl;
			return;
		}
	for (int i = 0; i < passWordLen; i++)
		if (charValidation(passWord[i]) == false) {
			cout << "Illegal passWord" << endl;
			return;
		}
	sendCommandToServer(LOGIN);
	sendDataToServer(userName);
	sendDataToServer(passWord);
}
void gameAPI::registerr(string userName, string passWord) {
	if (loggedIn == true) {
		cout
				<< "You are already logged in, can not perform registration while logged in"
				<< endl;
		return;
	}
	int userLen = userName.length();
	int passWordLen = passWord.length();
	if (userLen <= 0 || passWordLen <= 0) {
		cout << "Illegal userName" << endl;
		return;
	}
	for (int i = 0; i < userLen; i++)
		if (charValidation(userName[i]) == false) {
			cout << "Illegal userName" << endl;
			return;
		}
	for (int i = 0; i < passWordLen; i++)
		if (charValidation(passWord[i]) == false) {
			cout << "Illegal passWord" << endl;
			return;
		}
	sendCommandToServer(REGISTER);
	sendDataToServer(userName);
	sendDataToServer(passWord);
}

void gameAPI::startGame(string userName) {
	if (userName == this->userName) {
		cout << "You can not start a game with yourself" << endl;
		return;
	}
	if (loggedIn == true) {
		sendCommandToServer(START_GAME);
		sendDataToServer(this->userName);
		sendDataToServer(userName);
	} else
		cout << "You are not logged in, can not send game request" << endl;

}

void gameAPI::autoStartGame() {
	if (loggedIn == true) {
		sendCommandToServer(AUTO_START_GAME);
		sendDataToServer(this->userName);
	} else
		cout << "You are not logged in, can not send game request" << endl;
}

void gameAPI::acceptGameRequest() {
	if (requesterUserName == "") {
		cout << "There is no game request" << endl;
		return;
	}
	if (loggedIn == false) {
		cout << "You are not logged in, can not accept a game request" << endl;
		return;
	}
	if (playing == true) {
		cout << "You are already playing a match, can not accept another game"
				<< endl;
		return;
	}

	sendCommandToServer(ACCEPT_GAME_REQUEST);
	sendDataToServer(userName);
	sendDataToServer(requesterUserName);
}

void gameAPI::declineGameRequest() {
	if (requesterUserName == "") {
		cout << "There is no game request" << endl;
		return;
	}
	if (loggedIn == false) {
		cout << "You are not logged in, can not decline a game request" << endl;
		return;
	}
	if (playing == true) {
		cout << "You are already playing a match, can not decline another game"
				<< endl;
		return;
	}

	sendCommandToServer(DECLINE_GAME_REQUEST);
	sendDataToServer(userName);
	sendDataToServer(requesterUserName);
	requesterUserName = "";
}

void gameAPI::endGame(int status, int points) {
	if (playing == false || opponentUserName == "") {
		cout << "You are not playing a match, can not end the game" << endl;
		return;
	}
	if (loggedIn == false) {
		cout << "You are not logged in, can not end the game" << endl;
		return;
	}
	sendCommandToServer(END_GAME);
	sendDataToServer(userName);
	sendDataToServer(opponentUserName);
	sendCommandToServer(status);
	sendCommandToServer(points);
}

void gameAPI::getUserList() {
	if (loggedIn == true) {
		sendCommandToServer(LIST_OF_USERS);
	} else
		cout << "You are not logged in, can not present users list" << endl;
}

void gameAPI::getScoreBoard() {
	if (loggedIn == true) {
		sendCommandToServer(SCORE_BOARD);
	} else
		cout << "You are not logged in, can not present score board" << endl;
}

void gameAPI::startGamePlatform() {
	int* result = handler->gamePlatfrom();
	if (result != NULL)
		this->endGame(result[0], result[1]);
	playing = false;
}

void gameAPI::disconnect() {
	if (playing == true) {
		cout << "can not disconnect from server while playing a match" << endl;
		return;
	}
	sendCommandToServer(DISCONNECT);
	if (loggedIn)
		sendDataToServer(userName);
}

void gameAPI::sendCommandToServer(int cmd) {
	int command = htonl(cmd);
	serverConnectionSocket->writee((char*) &command, sizeof(int));
}

void gameAPI::sendDataToServer(string msg) {
	int len = htonl(msg.length());
	serverConnectionSocket->writee((char*) &len, sizeof(int));
	serverConnectionSocket->writee(msg.c_str(), msg.length() * sizeof(char));
}

int gameAPI::readCommandFromServer() {
	int command = 0;
	serverConnectionSocket->readd((char*) &command, sizeof(int));
	return ntohl(command);
}

string gameAPI::readDataFromServer() {
	int dataSize = 0;
	serverConnectionSocket->readd((char*) &dataSize, sizeof(int));
	dataSize = ntohl(dataSize);
	char buffer[BUFFER];

	int rd = serverConnectionSocket->readd(buffer, dataSize);
	buffer[rd] = '\0';
	string msg = buffer;
	return msg;
}

bool gameAPI::charValidation(char ch) {
	if (ch == ' ' || ch == '\n')
		return false;
	else
		return true;
}

bool gameAPI::isLoggedIn() {
	return loggedIn;
}

bool gameAPI::isPlaying() {
	return playing;
}

gameAPI::~gameAPI() {
}

} /* namespace networkingLab */

