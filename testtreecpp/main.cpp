#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <locale>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "blockchain.h"
using namespace std;

int main(void)
{
    setlocale(0, "");
    cout << "Создание цепи: " << endl;
    Blockchain bChain = Blockchain();
    cout << "Формируем 1-ый блок: " << endl;
    bChain.AddBlock(Block());
    
    

    system("cls");

    

   The_Tree* tree = new The_Tree();
   Blockchain* pbChain = &bChain;
   set_tree(tree->get_The_Tree(), string_func("0s0ds0a"), (Blockchain*)pbChain);

    

    print_tree(tree->get_The_Tree());

    delete tree;
    system("pause");
    return 0;
}

