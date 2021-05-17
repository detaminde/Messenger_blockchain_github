#include "blockchain.h"

using namespace std;

Block::Block()
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
uint64_t Block::GetBlockNum()
{
    return nBlockNum;
}
uint64_t Block::GetNonce()
{
    return nNonce;
}
string Block::GetPrevHash()
{
    return sPrevHash;
}
string Block::GetHash()
{
    return sHash;
}
string Block::getTime()
{
    tm* ltm = localtime(&tTime);
    stringstream date;
    date << ltm->tm_hour << ":" << ltm->tm_min;
    string returnstr = date.str();

    return returnstr;
}
void Block::setCreatorNickName(string newCreatorNickName)
{
    this->creatorNickName = newCreatorNickName;
}
void Block::setPrevHash(string str)  { this->sPrevHash = str; }

void Block::MineBlock(uint32_t nDifficulty)
{
    string cstr = "Пути господня неисповедимы";
    cstr.replace(cstr.begin(),
        cstr.begin()+nDifficulty, nDifficulty, '0');
    //int i = 0;
    do
    {
        nNonce++;
        sHash = CalculateHash();
        //i++;
    } while (sHash.substr(0, nDifficulty) != cstr.substr(0, nDifficulty));
    //cout << "Block mined:" << sHash <<endl<<" with this count of iterations: "<<i << endl;
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
string Block::GetCreatorNickName() {return this->creatorNickName; }





uint32_t Blockchain::getDifficulty() { return nDifficulty; }
Blockchain::Blockchain()
{
    vChain.emplace_back(Block("Genesis Block"));
    vChain.emplace_back(Block("The 1st block"));
    nDifficulty = 2;
}
Blockchain::~Blockchain()
{
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




