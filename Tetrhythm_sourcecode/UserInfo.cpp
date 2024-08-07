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
