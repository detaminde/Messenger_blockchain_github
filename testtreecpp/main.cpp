#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <locale>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "blockchain.h"
#include "Packet.pb.h"



using namespace std;
using namespace testtree::serialize;
//�������� ������������(������ ���������������)
//�������� ����������� ����� ���������� � ����� � ��� ���� ���������� ����� ������� ���-�������

//void saveMessage(const char*)

int main(void)
{

    setlocale(0, "");
    
    Full_Message mes;
    mes.set_data("���� ������");
    mes.set_authorid(321);
    mes.set_chatid(1);
    FirstPerson frst;
    frst.set_id(123);
    SecondPerson scnd;
    scnd.set_id(321);
    Chat chat;
    chat.set_allocated_frsprs_chat(&frst);
    chat.set_allocated_scndprs_chat(&scnd);
    chat.mutable_mes()->Add()->set_authorid(123);

    cout << "�������� ����: " << endl;
    Blockchain bChain = Blockchain();
    Block b1 = Block("���� ������");
    Blockchain* pbChain;
    The_Tree* tree = new The_Tree(); 

    int num = 0;
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
    }

    delete tree;
    return 0;
}

