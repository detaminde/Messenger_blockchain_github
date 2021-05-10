#include "mainwindow.h"
#include "myclient.h"
#include <QApplication>
#include "chatclient_2.h"
#include <QTcpSocket>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MyClient client("b8570adb8ac2.sn.mynetname.net", 1567);
    //client.show();

//    MainWindow w;
//    w.show();

//    QTcpSocket *primer = new QTcpSocket();
//    primer->connectToHost("localhost", 80);
//    ChatClient w(primer);
//    w.show();

    ChatClient_2 w;
    w.show();

    return a.exec();
}


//на какое-то будущее, когда Даниил сделает сервак под клиент ---- сделать шифрование данных своё
