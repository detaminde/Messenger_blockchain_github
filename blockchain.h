#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>

#include <cstring>
#include "sha256.h"


//�������� ���������� ���������� � ���� � �������� �������� ����
//����� �������� - ��� �������� ���� �������� ������� ����� � �����

//���������� ��� ���

using namespace std;


class Block
{
public:
	string sPrevHash;
    Block();
    Block(string str);

    void MineBlock(uint32_t nDifficulty);
    void setBlockData(string const str);
    void setBlockIndex(uint64_t num);
    void setPrevHash(string str);
    void setCreatorNickName(string new_creatorNickName);
    string getTime();
    string getBlockData();
    string GetHash();
    string GetCreatorNickName();
    uint64_t GetNonce();
    string GetPrevHash();
    uint64_t GetBlockNum();
private:
    string creatorNickName;
    uint64_t nBlockNum;
    uint64_t nNonce;
	string sData;
	string sHash;
    time_t tTime;
    inline string CalculateHash() const
    {
        stringstream ss;
        ss  << tTime << sData << nNonce << sPrevHash << creatorNickName;

        return sha256(ss.str());
    }
};

class Blockchain
{
public:
    Blockchain();
    ~Blockchain();
	bool AddBlock(Block bNew);
    void printData();
    void printBlockData(uint16_t num);
    vector<Block> getChain();
    uint64_t getBlockCount();
    bool difficultyAuthentication(Block block);
    uint32_t getDifficulty();
private:
    uint64_t BlockCount = 2;
	uint32_t nDifficulty;
	vector<Block> vChain;
	Block GetLastBlock() const;
};


