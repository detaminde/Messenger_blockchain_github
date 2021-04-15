#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include <cstring>
#include "sha256.h"

//добавить раздельное добавление в цепь и проверку прошлого хеша
//также добавить - при создании цепи создание первого блока с хешем

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
    Block(string str);
    string GetHash()
    {
        return sHash;
    }
    void MineBlock(uint32_t nDifficulty);
    void setBlockData(string const str);
    string getBlockData();
    void setBlockIndex(uint64_t num);
    void setPrevHash(string str) { this->sPrevHash = str; }

private:
    int64_t nBlockNum;
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
    bool difficultyAuthentication(Block block);
    
private:
    uint64_t BlockCount = 1;
	uint32_t nDifficulty;
	vector<Block> vChain;
	Block GetLastBlock() const;

};
Blockchain::Blockchain()
{  
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
bool Blockchain::AddBlock(Block newBlock) 
{
    if (!newBlock.getBlockData().empty())
    {
        newBlock.setPrevHash(GetLastBlock().GetHash());
        newBlock.MineBlock(2);
        if (difficultyAuthentication(newBlock))
        {
            vChain.push_back(newBlock);
            BlockCount++;
            GetLastBlock().setBlockIndex(getBlockCount());
            return true;
        }
        else
            return false;
    }
    else
    {
        return false;
    }
}
Block Blockchain::GetLastBlock() const 
{
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
uint64_t Blockchain::getBlockCount() { return BlockCount; }

bool Blockchain::difficultyAuthentication(Block block)
{
    string sDifficulty = "5.13. 24.11. 13.16. 9.13.5. 5.13. 24.11.";
    sDifficulty.replace(sDifficulty.begin(),
        sDifficulty.begin() + nDifficulty, nDifficulty, '0');
    if (block.GetHash().substr(0, nDifficulty) == sDifficulty.substr(0, nDifficulty))
        return true;
    else
        return false;
}

