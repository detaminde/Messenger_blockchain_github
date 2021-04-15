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
//запилить сериализацию(учесть конвертирование)
//запилить авторизацию через шифрование в байты и ещё одно шифрование через простую хеш-функцию

//void saveMessage(const char*)

int main(void)
{

    setlocale(0, "");
    
    Full_Message mes;
    mes.set_data("всем привет");
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

    cout << "Создание цепи: " << endl;
    Blockchain bChain = Blockchain();
    Block b1 = Block("Всем привет");
    Blockchain* pbChain;
    The_Tree* tree = new The_Tree(); 

    int num = 0;
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
    }

    delete tree;
    return 0;
}

