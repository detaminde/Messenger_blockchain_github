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

void transform_User_ForSerialize(User& user, serialize::serUser seruser);
/*void transform_TreeNode_ForSerialize(Tree& tree, serialize::serTreeNode sertreenode);
void _transform_TreeNode_ForSerailize(Tree& tree, serialize::serTreeNode& sertreenode);
void transform_Tree_ForSerialize(Tree& tree,
    serialize::serTree& sertree);*/
void saveTree(const char* fname, const serialize::serTree& sertree);


void loadBlockchain(const char* fname, serialize::serBlock& deserblockchain);
void loadTree(const char* fname, serialize::serTree& desertree);
string transform_tree_forserialize_2(tree_node* treenode, vtype_tree_t tvalue);

int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    setlocale(0, "");

    cout << "�������� ����: " << endl;
    Blockchain bChain = Blockchain();
    User* user1 = new User("dedminde", "1");
    Block b1 = Block("���� ������");
    //Blockchain* pbChain;
    The_Tree* tree = new The_Tree(BLOCK_ELEM); 
    string newmessage = WriteMessage(user1);
    Blockchain* pbChain = &bChain;
    b1.setBlockData(newmessage);
    pbChain->AddBlock(b1);
    set_tree(tree->get_The_Tree(), string_func("929fa"), (Blockchain*)pbChain);
    set_tree(tree->get_The_Tree(), string_func("92fa"), (Blockchain*)pbChain);
    set_tree(tree->get_The_Tree(), string_func("9239fa"), (Blockchain*)pbChain);
   // serialize::serBlockchain* SerBlockchain = new serialize::serBlockchain();
    
    //transform_Blockchain_ForSerialize(bChain, (*SerBlockchain));
    //saveBlockchain("blockchain.dat", (*SerBlockchain));

    print_tree(tree->get_The_Tree());

    cout << "�����.." << endl;
    string serstr = transform_tree_forserialize_2(tree->get_The_Tree()->node, 
        tree->get_The_Tree()->type.value);

    cout << serstr;
    //serialize::serTree* sertree = new_sertree(serialize::STRING_ELEM, serialize::BLOCK_ELEM);
    //set_sertree(sertree, serstring_func("0asdd"), (serialize::serBlockchain*)SerBlockchain);
    //cout<<"0asd0as0"<<endl;
   // saveTree("tree.dat", (*sertree));
    //����������� �������� ��������� ������� ��� ��������
    //�� ����� ����������� ��������� ��� �����
    //cout << "�����))" << endl;
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

    delete tree;
    cout << "�������" << endl;
    system("pause");
    return 0;
}

//������������ ���������
/*void transform_Blockchain_ForSerialize(Blockchain& blockchain,
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
     serialize::serBlockchain& serblockchain)
{
    fstream out(fname, ios::out | ios::trunc | ios::binary);
    if (!serblockchain.SerializeToOstream(&out))
        throw runtime_error("Blockchain saving has failed");
}
*/
//�������������� ���������(� �����)

//������������ ������
/*void transform_User_ForSerialize(User& user, serialize::serUser seruser)
{
    seruser.set_aboutyourself(user.getAboutYourself());
    seruser.set_firstname(user.getFirstName());
    seruser.set_nickname(user.getNickName());
    seruser.set_secondname(user.getSecondName());
    seruser.set_userid(user.getId());
}*/

void saveTree(const char* fname, 
   const serialize::serTree& sertree)
{
    fstream out(fname, ios::out | ios::trunc | ios::binary);
    if (!sertree.SerializeToOstream(&out))
        throw runtime_error("Tree saving has failed");
}

string transform_Blockchain_ForSerialize(Blockchain* blockchain)
{
    stringstream ss;
    ss << "start//";
    for (int i = 0; i < blockchain->getChain().size(); i++)
    {
        ss <<"BLOCKHASH:"<< blockchain->getChain()[i].GetHash() + '/'
            <<"PREVHASH:"<< blockchain->getChain()[i].sPrevHash + '/'
            <<"DATA:" << blockchain->getChain()[i].getBlockData() + '/'
            << "end//\n";
    }
    string bufstr = ss.str();
    return bufstr;
}
string transform_User_ForSerialize(User* user)
{
    stringstream ss;
    ss << "start//"
        <<"USER_ID:"<< user->getId() + '/'
        <<"USER_NICKNAME:"<< user->getNickName() + '/'
        <<"USER_FIRSTNAME:"<< user->getFirstName() + '/'
        <<"USER_SECONDNAME:" <<user->getSecondName() + '/'
        << "USER_ABOUTSELF:" <<user->getAboutYourself() + '/'
        << "end//\n";
    string bufstr = ss.str();
    return bufstr;
}

string transform_tree_forserialize_2(tree_node* treenode, vtype_tree_t tvalue)
{
    string bufserstring = "START///";
    string leftserialized = "START///";
    string rightserialized = "START///";
    if (treenode == NULL)
    {
        return "start//x/end\n";
    }
    else
    {
        switch (tvalue)
        {
        case BLOCK_ELEM:
            leftserialized = transform_tree_forserialize_2(treenode->left, tvalue);
            rightserialized = transform_tree_forserialize_2(treenode->right, tvalue);
            bufserstring = transform_Blockchain_ForSerialize(treenode->data.value.block);
            return bufserstring + leftserialized + rightserialized;
            break;
        case USER_ELEM:
            leftserialized = transform_tree_forserialize_2(treenode->left, tvalue);
            rightserialized = transform_tree_forserialize_2(treenode->right, tvalue);
            bufserstring = transform_User_ForSerialize(treenode->data.value.user);
            return bufserstring + leftserialized + rightserialized;
            break;
        default:
            runtime_error("Error\n");
            break;
        }
    }
}

/*void _transform_TreeNode_ForSerailize(tree_node& treenode,
    serialize::serTreeNode& sertreenode, 
    vtype_tree_t tkey, vtype_tree_t tvalue)
{
    serialize::serBlockchain bufserblockchain = serialize::serBlockchain();
    Blockchain* pbufblockchain = treenode.data.value.block;
    transform_Blockchain_ForSerialize((*pbufblockchain), bufserblockchain);

    serialize::serUser bufserUser = serialize::serUser();
    User* bufuser = treenode.data.value.user;
    transform_User_ForSerialize((*bufuser), bufserUser);

    
  
}
*/
/*void transform_TreeNode_ForSerialize(Tree& tree, serialize::serTreeNode sertreenode)
{
    serialize::serTreeNode bufsertree = serialize::serTreeNode();
    switch (tree.type.key)
    {
    case DECIMAL_ELEM:
        sertreenode.set_allocated_parent(NULL);//������ ����� ���������
        while (tree.node->left != NULL && tree.node->right != NULL)
        {
            _transform_TreeNode_ForSerailize(tree, bufsertree);
            sertreenode.set_allocated_right(&bufsertree);
            
            
        }
        break;
    case STRING_ELEM:
        break;
    }
}*/


/*void transform_Tree_ForSerialize(Tree& tree,
    serialize::serTree& sertree)//�������� ����� ������� ���������� ����� �����
{
    serialize::serBlockchain bufSerBlock = serialize::serBlockchain();
    serialize::serUser bufSerUser = serialize::serUser();
    switch (tree.type.key)
    {
    case DECIMAL_ELEM:
        sertree.set_key_type(serialize::DECIMAL_ELEM);
        //sertree.mutable_treenode()->mutable_key()->set_decimal(tree.node->data.key.decimal);
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
       // sertree.mutable_treenode()->mutable_key()->set_str(tree.node->data.key.string);
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
}*/


