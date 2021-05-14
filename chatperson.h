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
    ChatPerson()
    {
        this->aboutYourself = "no info";
        this->firstName = "no info";
        this->nickname = "no info";
        this->secondName = "no info";
    }
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


class User : ChatPerson
{
protected:
    string id;
public:
    User(string nickname, string abyours
        , string frsname, string secondname, string id);
    User(string nickname, string id);
    User();
    User(User* newuser)
    {
        this->aboutYourself = newuser->getAboutYourself();
        this->firstName = newuser->getFirstName();
        this->id = newuser->getId();
        this->nickname = newuser->getNickName();
        this->secondName = newuser->getSecondName();
    }

    ~User() {}


    //setters
    void setId(string newId) { this->id = newId; }
    void setAbYourself(string newabyrslf) { this->aboutYourself = newabyrslf; }
    void setFrsname(string newfrsname) { this->firstName = newfrsname; }
    void setSecondname(string newsecondname) { this->secondName = newsecondname; }
    void setNickname(string newnickname) { this->nickname = newnickname; }
    //getters
    string getAboutYourself() { return aboutYourself; }
    string getNickName() { return nickname; }
    string getFirstName() { return firstName; }
    string getSecondName() { return secondName; }
    string getId() { return id; }
};

User::User()
{
    this->aboutYourself = "no info";
    this->firstName = "no info";
    this->id = "no info";
    this->nickname = "no info";
    this->secondName = "no info";
}

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
    this->id = id;
}
string WriteMessage(User* user);

