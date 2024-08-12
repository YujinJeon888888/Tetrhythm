#pragma once
#include <string>

class UserInfo {
private:
	UserInfo() = default;  // 생성자를 private으로 설정
	~UserInfo() = default; // 소멸자도 private으로 설정

	// 복사 생성자와 대입 연산자를 삭제하여 복제 방지
	UserInfo(const UserInfo&) = delete;
	UserInfo& operator=(const UserInfo&) = delete;

	std::string userID;
	std::string userCharacter;
public:
	static UserInfo& getInstance();
	void setUserID(const std::string& username);
	std::string getUserID() const;
	void setUserCharacter(const std::string& username);
	std::string getUserCharacter() const;

};