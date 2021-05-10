#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItem>

#include "chatclient_2.h"
#include "registration.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Sign_up_clicked();
    void on_Sign_in_clicked();
    void on_Exit_clicked();

    void slotError(QAbstractSocket::SocketError);
    /*void slotSendToServer();*/
    void slotConnected();
private:

    Ui::MainWindow *ui;
    ChatClient_2 *p_chat_window;
    Registration* p_Registration_window;
    QTcpSocket* m_pTcpSocket_signIn;
    QByteArray Data;
    int buffer;

    QJsonParseError docError;
    QJsonDocument doc;

    bool ResponseFromServer_100();
    bool ResponseFromServer_202();
};
#endif // MAINWINDOW_H
