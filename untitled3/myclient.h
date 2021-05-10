#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>



class QTextEdit;
class QLineEdit;

class MyClient : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit* m_ptxtinfo;
    QLineEdit* m_ptxtinput;
    quint16 m_nNextBlockSize;
    QByteArray arrBlock;


signals:
    void AuthorizationWin();//сигнал для окна авторизации
public:
    MyClient(const QString& strHost, int nPort, QWidget* pwgt = 0);
private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotDisconnected();
    void slotexitProgramm();
};

#endif // MYCLIENT_H
