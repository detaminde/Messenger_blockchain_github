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

//�������� ������������(��������� �)
//�������� ���������� ��� ���-�������
//�������� ����������� ����� ���������� � ����� � ��� ���� ���������� ����� ������� ���-�������
//�������� �������� �������
//�������� ������� � �.�.

//����������� �� �������� �������� ����� ��� ������������
//�������� ������������ ��� ������ � ��������� �

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

    cout << "�������� ����: " << endl;
    Blockchain bChain = Blockchain();
    Block b1 = Block("���� ������");
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
        cout << "���� �������: "<<endl<<
            "1. �������� ���� � ����"<<endl<<
            "2. �������� ���� � ������"<<endl<<
            "3. ������� ������ �� ������"<<endl<<
            "4. ������ �� ����"<<endl;
        cin >> num;
        switch(num)
        {
        case 1:
            cout << "���������� ����� � ����..." << endl;
            pbChain = &bChain;
            if(pbChain->AddBlock(b1))
                cout << "�������" << endl;
            break;
        case 2:
            cout << "���������� ���� � ������..." << endl;
            pbChain = &bChain;
            set_tree(tree->get_The_Tree(), string_func("9239fa"), (Blockchain*)pbChain);
            cout << "�������" << endl;
            break;
        case 3:
            cout << "������� ������..." << endl;
            print_tree(tree->get_The_Tree());
            break;
        case 4:
            cout << "�����..." << endl;
            flag = false;
            break;
        default:
            cout << "������������ �������, ������� ��� ���" << endl;
            break;
        }
        system("pause");
    }*/

    //delete tree;
    cout << "�������" << endl;
    system("pause");
    return 0;
}

//������������ ���������
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
//�������������� ���������(� �����)

//������������ ������

void transform_Tree_ForSerialize(Tree& tree,
    serialize::serTree& sertree)//�������� ����� ������� ���������� ����� �����
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
            //������� ������� ����� ������
            
                // ���-���������� - tree_node
            
            //������� ��������� ������ ������, ����� �������� �� ������
            sertree.mutable_treenode()->mutable_value()
                ->set_allocated_blockchain(&bufSerBlock);
            break;
        case USER_ELEM:
            //���� ������ ����������� � ����������
            sertree.set_value_type(serialize::USER_ELEM);
            break;
        }
        break;
    case STRING_ELEM:
        sertree.set_key_type(serialize::STRING_ELEM);
        //���� ����������� � ����������� �����
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

