#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <locale>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdexcept>
#include <chrono>

#include "tree.h"
#include "blockchain.h"
#include "Packet.pb.h"
#include "chatperson.h"


using namespace std;
using namespace testtree;

//запилить сериализацию(проверить её)
//обновить функционал для хеш-таблицы
//запилить авторизацию через шифрование в байты и ещё одно шифрование через простую хеш-функцию
//запилить цифровую подпись
//запилить билдеры и т.д.

//разобраться со временем создания блока для сериализации
//запилить сериализацию для дерева и проверить её

void transform_Blockchain_ForSerialize(Blockchain& blockchain, 
    serialize::serBlockchain& serblockchain);
void saveBlockchain(const char* fname, serialize::serBlockchain& serblockchain);


void transform_Tree_ForSerialize(Tree& tree,
    serialize::serTree& sertree);
void saveTree(const char* fname, const serialize::serTree& sertree);


void loadBlockchain(const char* fname, serialize::serBlock& deserblockchain);
void loadTree(const char* fname, serialize::serTree& desertree);

int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    setlocale(0, "");

    cout << "Создание цепи: " << endl;
    Blockchain bChain = Blockchain();
    Block b1 = Block("Всем привет");
    //Blockchain* pbChain;
    //The_Tree* tree = new The_Tree(BLOCK_ELEM); 

    serialize::serBlockchain SerBlockchain = serialize::serBlockchain();
    transform_Blockchain_ForSerialize(bChain, SerBlockchain);
    saveBlockchain("blockchain.dat", SerBlockchain);

    /*int num = 0;
    bool flag = true;
    while (flag)
    {
        system("cls");
        cout << "Ввод команды: "<<endl<<
            "1. Добавить блок в цепь"<<endl<<
            "2. Добавить цепь в дерево"<<endl<<
            "3. Вывести данные из дерева"<<endl<<
            "4. Выходи из меню"<<endl;
        cin >> num;
        switch(num)
        {
        case 1:
            cout << "Добавление блока в цепь..." << endl;
            pbChain = &bChain;
            if(pbChain->AddBlock(b1))
                cout << "Успешно" << endl;
            break;
        case 2:
            cout << "Добавление цепи в дерево..." << endl;
            pbChain = &bChain;
            set_tree(tree->get_The_Tree(), string_func("9239fa"), (Blockchain*)pbChain);
            cout << "Успешно" << endl;
            break;
        case 3:
            cout << "Выводим данные..." << endl;
            print_tree(tree->get_The_Tree());
            break;
        case 4:
            cout << "Выход..." << endl;
            flag = false;
            break;
        default:
            cout << "Некорректная команда, введите ещё раз" << endl;
            break;
        }
        system("pause");
    }*/

    //delete tree;
    cout << "Успешно" << endl;
    system("pause");
    return 0;
}

//сериализация блокчейна
void transform_Blockchain_ForSerialize(Blockchain& blockchain,
    serialize::serBlockchain& serblockchain)
{
    serblockchain.set_blockcount(blockchain.getBlockCount());
    serblockchain.set_ndifficulty(blockchain.getDifficulty());

    serialize::serBlock* bufSerBlock = new serialize::serBlock();
    for (int i = 0; i < blockchain.getChain().size(); i++)
    {
        serblockchain.add_block()->set_prevhash(blockchain.getChain()[i].sPrevHash);
        serblockchain.add_block()->set_sdata(blockchain.getChain()[i].getBlockData());
    }
}
void saveBlockchain(const char* fname,
    const serialize::serBlockchain& serblockchain)
{
    fstream out(fname, ios::out | ios::trunc | ios::binary);
    if (!serblockchain.SerializeToOstream(&out))
        throw runtime_error("Blockchain saving has failed.");
}
//десериализация блокчейна(а зачем)

//сериализация дерева

void transform_Tree_ForSerialize(Tree& tree,
    serialize::serTree& sertree)//добавить везде условия содержания всего этого
{
    serialize::serBlockchain bufSerBlock = serialize::serBlockchain();
    serialize::serUser bufSerUser = serialize::serUser();
    switch (tree.type.key)
    {
    case DECIMAL_ELEM:
        sertree.set_key_type(serialize::DECIMAL_ELEM);
        sertree.mutable_treenode()->mutable_key()->set_decimal(tree.node->data.key.decimal);
        switch (tree.type.value)
        {
        case BLOCK_ELEM:
            sertree.set_value_type(serialize::BLOCK_ELEM);
            transform_Blockchain_ForSerialize((*tree.node->data.value.block), bufSerBlock);
            //сделать перебор всего дерева
            
                // код-добавление - tree_node
            
            //сначала добавлять список дерева, потом значение уж только
            sertree.mutable_treenode()->mutable_value()
                ->set_allocated_blockchain(&bufSerBlock);
            break;
        case USER_ELEM:
            //сюда просто скопировать и переделать
            sertree.set_value_type(serialize::USER_ELEM);
            break;
        }
        break;
    case STRING_ELEM:
        sertree.set_key_type(serialize::STRING_ELEM);
        //сюда скопировать с предыдущего блока
        sertree.mutable_treenode()->mutable_key()->set_str(tree.node->data.key.string);
        switch (tree.type.value)
        {
        case BLOCK_ELEM:
            sertree.set_value_type(serialize::BLOCK_ELEM);
            break;
        case USER_ELEM:
            sertree.set_value_type(serialize::USER_ELEM);
            break;
        }
        break;
    }
}
void saveTree(const char* fname, const serialize::serTree& sertree)
{
    fstream out(fname, ios::out | ios::trunc | ios::binary);
    if (!sertree.SerializeToOstream(&out))
        throw runtime_error("Tree saving has failed.");
}

