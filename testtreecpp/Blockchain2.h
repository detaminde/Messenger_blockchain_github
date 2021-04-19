#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include <cstring>
#include <fstream>
#include "sha256.h"

//�������� ���������� ���������� � ���� � �������� �������� ����
//����� �������� - ��� �������� ���� �������� ������� ����� � �����

using namespace std;


class Block
{
public:
    Block()
    {
        string buf;
        for (int i = 0; i < 10; i++)
        {
            cout << "� ���������: " << (i + 1) << " ";
            cin >> buf;
            sData = sData + buf + '\n';
            buf = "";
        }
        nNonce = -1;
        tTime = time(nullptr);
    }
    ~Block()
    {
        nNonce = NULL;
        sData.clear();
        sHash.clear();
        tTime = NULL;
        sPrevHash.clear();
    }
    Block(string str);
    string GetHash()
    {
        return sHash;
    }
    void MineBlock(uint32_t nDifficulty);//+
    void setBlockData(string const str);
    string getBlockData();
    void setBlockIndex(uint64_t num);
    string getPrevHash() { return sPrevHash; }//+
    void setPrevHash(string str) { this->sPrevHash = str; }

private:
    string sPrevHash;
    int64_t nBlockNum;
    int64_t nNonce;
    string sData;
    string sHash;
    time_t tTime;
    inline string CalculateHash() const
    {
        stringstream ss;
        ss << tTime << sData << nNonce << sPrevHash;

        return sha256(ss.str());
    }//+
};
void Block::MineBlock(uint32_t nDifficulty)
{
    string cstr = "���� �������� ������������";
    cstr.replace(cstr.begin(),
        cstr.begin() + nDifficulty, nDifficulty, '0');
    int i = 0;
    do
    {
        nNonce++;
        sHash = CalculateHash();
        i++;
    } while (sHash.substr(0, nDifficulty) != cstr.substr(0, nDifficulty));
    cout << "Block mined:" << sHash << endl << " with this count of iterations: " << i << endl;
}
string Block::getBlockData()
{
    return sData;
}
void Block::setBlockData(string const str) { this->sData = str; }
Block::Block(string str)
{
    setBlockData(str);
    this->nNonce = -1;
    tTime = time(nullptr);
}
void Block::setBlockIndex(uint64_t num) { this->nBlockNum = num; }


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
    bool givenBlockchain_whenNewBlockAdded_thenSucces(Block newBlock);
    bool givenBlockchaim_whenValidated_thenSuccess();

private:
    uint64_t BlockCount = 1;
    uint32_t nDifficulty;
    vector<Block> vChain;
    Block GetLastBlock() const;

};
Blockchain::Blockchain() {
    vChain.emplace_back(Block("Genesis Block"));
    nDifficulty = 2;
}
Blockchain::~Blockchain()
{
    for (int i = 0; i < vChain.size(); i++)
    {
        vChain[i].~Block();
    }
}

Block Blockchain::GetLastBlock() const {
    return vChain.back();
}
vector<Block> Blockchain::getChain()
{
    return vChain;
}
void Blockchain::printData()
{
    for (int i = 0; i < getChain().size(); i++)
    {
        cout << "���� �" << i << endl << "hash = " << getChain()[i].GetHash() << endl << "������: " << endl
            << getChain()[i].getBlockData() << endl << "____________" << endl;
    }
}
void Blockchain::printBlockData(uint16_t num)
{
    for (int i = 0; i < getChain().size(); i++)
    {
        if (i == num)
            cout << getChain()[i].getBlockData() << endl;
    }
}
uint64_t Blockchain::getBlockCount() { return BlockCount; }

bool Blockchain::AddBlock(Block newBlock)
{
    if (!newBlock.getBlockData().empty())
    {
        newBlock.setPrevHash(GetLastBlock().GetHash());
        newBlock.MineBlock(2);
        vChain.push_back(newBlock);
        BlockCount++;
        GetLastBlock().setBlockIndex(getBlockCount());
        return true;
    }
    else
    {
        return false;
    }
        
}

/*void Blockchain::AddBlock(Block bNew) {
    bNew.setPrevHash(GetLastBlock().GetHash());
    bNew.MineBlock(2);
    vChain.push_back(bNew);
    BlockCount++;
    bNew.setBlockIndex(getBlockCount());
}*/