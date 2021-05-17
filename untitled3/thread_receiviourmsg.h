#ifndef THREAD_RECEIVIOURMSG_H
#define THREAD_RECEIVIOURMSG_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include <QFile>
#include <QListWidget>

class Thread_receiviourMsg : QThread
{
public:
   explicit Thread_receiviourMsg(QTcpSocket* parentSocket,
                                 bool* status,
                                 QListWidget* listWidget_parent,
                                 QJsonDocument* doc);

   void run();
private:
   QTcpSocket* p_TcpSocket_thread;
   QByteArray byteArr_thread;
   QJsonDocument doc_thread;
   QJsonArray docArr_thread;
   QJsonParseError docErr_thread;
   QFile file;
   QString buffer;

   QJsonDocument* doc_buffer;
   QJsonParseError docForbuf;
   QJsonArray docArr_buffer;
   QListWidget* listWidget_thread;

   bool* flag_statusClient;

   bool ResponseFromServer_1000();
   bool isConnectedToServer();
   void SortQJsonArray_valueBlockNums(QJsonArray arr);
};

#endif // THREAD_RECEIVIOURMSG_H
