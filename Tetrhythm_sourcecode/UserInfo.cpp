#pragma once
#include "UserInfo.h"

 UserInfo& UserInfo::getInstance() {
	static UserInfo instance;
	return instance;
}

 void UserInfo::setUserID(const std::string& username) {
	this->userID = username;
}

 std::string UserInfo::getUserID() const {
	return this->userID;
}

 void UserInfo::setUserCharacter(const std::string& userCharacter)
 {
	 this->userCharacter = userCharacter;
 }

 std::string UserInfo::getUserCharacter() const
 {
	 return this->userCharacter;
 }

 void UserInfo::setLine(const int line)
 {
	 this->line = line;
 }

 int UserInfo::getLine() const
 {
	 return line;
 }

 void UserInfo::setTetris(const int tetris)
 {
	 this->tetris = tetris;
 }

 int UserInfo::getTetris() const
 {
	 return tetris;
 }

 void UserInfo::setHighScore(const int highScore)
 {
	 this->highScore = highScore;
 }

 int UserInfo::getHighScore() const
 {
	 return highScore;
 }

