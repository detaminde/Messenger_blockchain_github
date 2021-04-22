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
	string id;
public:
	User(string nickname, string abyours
		, string frsname, string secondname, string id);
	User(string nickname, string id);
	~User() {}

	
	//setters
	void setId(string newId) { this->id = newId; }
	//getters
	string getAboutYourself() { return aboutYourself; }
	string getNickName() { return nickname; }
	string getFirstName() { return firstName; }
	string getSecondName() { return secondName; }
	string getId() { return id; }
	void printInfo() override;
};

User::User(string nickname, string abyours
	, string frsname, string secondname, string id) :
	ChatPerson(nickname, abyours, frsname, secondname)
{
	this->nickname = nickname;
	this->aboutYourself = abyours;
	this->firstName = frsname;
	this->secondName = secondname;
	this->id = id;
}
User::User(string nickname, string id): ChatPerson(nickname)
{
	this->nickname = nickname;
	this->id = id;
}
void User::printInfo()
{
	ChatPerson::printInfo();
	cout << endl << "Id: " << id << endl;
}
string WriteMessage(User* user);

static string WriteMessage(User* user)
{
	stringstream ss;
	cout << "Введите сообщение: " << endl;
	string buffer = "";
	string buffer1 = "";
	ss << user->getNickName() << '/' << user->getId() << '/';
	buffer1 += ss.str();
	cin >> buffer;
	buffer1 += buffer;
	buffer1 += '//';
	return buffer1;
}