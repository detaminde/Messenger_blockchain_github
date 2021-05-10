#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>

namespace Ui {
class Registration;
}

class Registration : public QMainWindow
{
    Q_OBJECT

public:
    explicit Registration(QTcpSocket *parentSocket, QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_Exit_clicked();//слот-обработчик кнопки
    void on_SignUp_clicked();
    void slotReadyRead_registration();

private:
    Ui::Registration *ui;
    QByteArray byteArr;
    QTcpSocket* p_TcpSocket_registration;

    QJsonDocument doc;
    QJsonParseError docError;

    bool Connected();
    void SendToServer();
signals:
    void openAuthorizationWin();//сигнал для окна авторизации
};

#endif // REGISTRATION_H
