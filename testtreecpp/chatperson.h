#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>

//написать токен
//написать функционал для юзера

using namespace std;
class ChatPerson
{
protected:
	string nickname;
	string aboutYourself;
	string firstName;
	string secondName;
public:
	ChatPerson(string nickname, string abyours
		, string frsname, string secondname);
	~ChatPerson() {};
	ChatPerson(string nickname);
	//GETTERS
	string getNickname() { return nickname; }
	string getAboutYourself() {return aboutYourself;}
	string getFirstName() { return firstName; }
	string SecondName() { return secondName; }
	//SETTERS
	void setNickname(string newNickname) { this->nickname = newNickname; }
	void setAboutYourself(string newAboutYourself) { this->aboutYourself = newAboutYourself; }
	void setFirstName(string newFirstName) { this->firstName = newFirstName; }
	void setSecondName(string newSecondName) { this->secondName = newSecondName; }

	virtual void printInfo();
};

ChatPerson::ChatPerson(string nickname, string abyours
	, string frsname, string secondname)
{
	this->nickname = nickname;
	this->aboutYourself = abyours;
	this->firstName = frsname;
	this->secondName = secondname;
}
ChatPerson::ChatPerson(string nickname)
{
	this->nickname = nickname;
}
void ChatPerson::printInfo()
{
	printf("Никнейм: %10s, Доп.информация: %10s, Имя: %10s, Фамилия: %10s",
		nickname, aboutYourself, firstName, secondName);
}

class User : ChatPerson
{
protected:
	uint64_t id;
public:
	User(string nickname, string abyours
		, string frsname, string secondname, uint64_t id);
	User(string nickname, uint64_t id);
	~User() {}

	uint64_t getId() { return id; }
	void setId(uint64_t newId) { this->id = newId; }

	void printInfo() override;
};

User::User(string nickname, string abyours
	, string frsname, string secondname, uint64_t id) :
	ChatPerson(nickname, abyours, frsname, secondname)
{
	this->nickname = nickname;
	this->aboutYourself = abyours;
	this->firstName = frsname;
	this->secondName = secondname;
	this->id = id;
}
User::User(string nickname, uint64_t id): ChatPerson(nickname)
{
	this->nickname = nickname;
	this->id = id;
}
void User::printInfo()
{
	ChatPerson::printInfo();
	cout << endl << "Id: " << id << endl;
}