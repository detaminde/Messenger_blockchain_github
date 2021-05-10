#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QWidget>
#include <QTcpSocket>
#include <QTime>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QListWidgetItem>

//#include "ChatClient/chatperson.h"

namespace Ui {
class ChatClient;
}



class ChatClient : public QMainWindow
{
    Q_OBJECT
private:
    Ui::ChatClient *ui;
    QTcpSocket* p_TcpSocket_chatclient;//сокет для соединения с сервером
    quint16 m_nNextBlockSize_chatclient;//размер буфера пакета
    QByteArray arrBlock_chatclient;//буфер для передачи пакетов
    QJsonDocument doc;//документ-буффер, считывающий данные
    QJsonParseError docError;//документ-сборщик ошибок при прочтении данных


signals:
    void AuthorizationWin();//сигнал для окна авторизации
public:
    explicit ChatClient(QTcpSocket* parentSocket, QWidget *parent = 0);
    //explicit ChatClient(const QString& strHost, int nPort, QWidget *parent = 0);

private slots:
    void slotReadyRead_ResponseFromConnect();
    void slotError(QAbstractSocket::SocketError);
    //void slotSendToServer();
    void slotConnected();
    //void slotDisconnected();
    //void slotexitProgramm();
    void on_listWidget_Dialogs_itemClicked(QListWidgetItem *item);
    void on_pushButton_send_clicked();
};

#endif // CHATCLIENT_H
