#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include <cstring>
#include <fstream>
#include "sha256.h"

//запилить сериализацию

using namespace std;


class Block
{
public:
	string sPrevHash;
    Block()
    {
        string buf;
        for (int i = 0; i < 10; i++)
        {
            cout << "№ сообщения: " << (i+1) << " ";
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
    string GetHash()
    {
        return sHash;
    }
    void MineBlock(uint32_t nDifficulty);
    void setBlockData(string const str);
    string getBlockData();
    
private:
	int64_t nNonce;
	string sData;
	string sHash;
	time_t tTime;
    inline string CalculateHash() const
    {
        stringstream ss;
        ss  << tTime << sData << nNonce << sPrevHash;

        return sha256(ss.str());
    }
};
void Block::MineBlock(uint32_t nDifficulty)
{
    string cstr = "Пути господня неисповедимы";
    cstr.replace(cstr.begin(), 
        cstr.begin()+nDifficulty, nDifficulty, '0');
    int i = 0;
    do
    {
        nNonce++;
        sHash = CalculateHash();
        i++;
    } while (sHash.substr(0, nDifficulty) != cstr.substr(0, nDifficulty));
    cout << "Block mined:" << sHash <<endl<<" with this count of iterations: "<<i << endl;
}
string Block::getBlockData()
{
    return sData;
}
void Block::setBlockData(string const str){this->sData = str;}


class Blockchain
{
public:
    Blockchain();
    ~Blockchain();
	void AddBlock(Block bNew);
    void printData();
    void printBlockData(uint16_t num);
    vector<Block> getChain();
    
private:
	uint32_t nDifficulty;
	vector<Block> vChain;
	Block GetLastBlock() const;

};
Blockchain::Blockchain() {
    const string genstr = "Genesis Block";
    vChain.emplace_back(Block());
    vChain[0].setBlockData(genstr);
    nDifficulty = 2;
}
Blockchain::~Blockchain()
{
    for (int i = 0; i < vChain.size(); i++)
    {
        vChain[i].~Block();
    }
}
void Blockchain::AddBlock(Block bNew) {
    bNew.sPrevHash = GetLastBlock().GetHash();
    bNew.MineBlock(2);
    vChain.push_back(bNew);
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
        cout << "Блок №" << i << endl << "hash = " << getChain()[i].GetHash() << endl << "Данные: " << endl
        << getChain()[i].getBlockData() << endl << "____________" << endl;
    }
}
void Blockchain::printBlockData(uint16_t num)
{
    for (int i = 0; i < getChain().size(); i++)
    {
        if(i == num)
            cout<<getChain()[i].getBlockData()<<endl;
    }
}
