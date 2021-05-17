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
#include <QDir>
#include <QMessageBox>
#include <QThread>

#include "blockchain.h"
#include "thread_receiviourmsg.h"

//сделать систему приватный-публичный ключ
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
    QString buffer;
    QFile file;//работа с файлом

    Thread_receiviourMsg* thread_receivour;
    QJsonDocument doc_forThreadRec;

    QJsonDocument doc_forLoadingMessages;
    QString dirPath = "C:\\Users\\User\\Documents\\QTfiles\\Test3\\untitled3\\json";

    bool statusClient;

    QFile attachedFile;
    QString path_attachedFile;
public:
    explicit ChatClient_2(QWidget *parent = nullptr);
    explicit ChatClient_2(QTcpSocket* parentSocket, QWidget* parent = nullptr);
signals:
    void BackToAuthorizationWindow();
private slots:
    void on_listWidget_Dialogs_itemClicked(QListWidgetItem *item);

    void on_Send_clicked();

    void slotError(QAbstractSocket::SocketError);

    void on_Exit_triggered();

    void on_pushButton_FindPeople_clicked();

    void on_lineEdit_findPeople_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_lineEdit_findPeople_returnPressed();

private:
    Ui::ChatClient_2 *ui;
    bool isConnected();
    bool isExistOnDevice(QString filename);
    bool ResponseFromServer_500(QString dialogName);
    bool LoadDialogsFromMemory();
    bool LastHashFromServer(QString lastHash);
    bool RequestDialogFromServer(QString textFromItem);
    bool SaveMessageToMemory(QString pathToFile, Block block);
    bool SendBlockToServer(Block block);
    bool ResponseFromServer_900();
    bool LoadDialogs();
    bool ResponseFromServer_901();
    bool isEmpty_attachedFilePath();
    bool RequestLastBlockNumber(int chat_id, int blocknum);
    bool ResponseFromServer_1100();
    bool RequestForNewMessages(QJsonDocument docFrom, int chat_id);
    bool ResponseFromServer_1101(QJsonDocument* doc);
};


#endif // CHATCLIENT_2_H
