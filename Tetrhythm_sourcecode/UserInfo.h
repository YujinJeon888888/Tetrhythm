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
	int line;
	int tetris;
	int highScore;
	int score;
	bool perfectClear = false;
public:
	static UserInfo& getInstance();
	void setUserID(const std::string& username);
	std::string getUserID() const;
	void setUserCharacter(const std::string& userCharacter);
	std::string getUserCharacter() const;
	void setLine(const int line);
	int getLine() const;
	void setTetris(const int tetris);
	int getTetris() const;
	void setHighScore(const int highScore);
	int getHighScore() const;
	void setScore(const int score);
	int getScore() const;
	void setPerfectClear(const bool perfectClear);
	bool getPerfectClear() const;
};