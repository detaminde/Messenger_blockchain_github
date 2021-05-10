#ifndef CHATCLIENT_2_H
#define CHATCLIENT_2_H

#include <QMainWindow>
#include <QWidget>
#include <QTcpSocket>
#include <QTime>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

#include "blockchain.h"

namespace Ui {
class ChatClient_2;
}

class ChatClient_2 : public QMainWindow
{
    Q_OBJECT
private:
    QTcpSocket* p_TcpSocket_chatclient;//сокет для соединения с сервером
    quint16 m_nNextBlockSize_chatclient;//размер буфера пакета
    QByteArray arrBlock_chatclient;//буфер для передачи пакетов
    QJsonDocument doc;//документ-буффер, считывающий данные
    QJsonParseError docError;//документ-сборщик ошибок при прочтении данных
    QJsonArray docArr;//переменная для хранения массивов
    QString globpath;//хранение полного пути, который пользователь выберет
    QFile file;//работа с файлом
public:
    explicit ChatClient_2(QWidget *parent = nullptr);
signals:
    void BackToAuthorizationWindow();
private slots:
    void on_listWidget_Dialogs_itemClicked(QListWidgetItem *item);

    void on_Send_clicked();

    void slotError(QAbstractSocket::SocketError);

    void on_Exit_triggered();

private:
    Ui::ChatClient_2 *ui;
    bool isConnected();
    bool isExistOnDevice(QString filename);
    bool ResponseFromServer_300(QString dialogName);
};

#endif // CHATCLIENT_2_H
