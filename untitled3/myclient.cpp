#include "myclient.h"


MyClient::MyClient(const QString& strHost,
                   int nPort,
                   QWidget* pwgt)
    : QWidget(pwgt), m_nNextBlockSize(0)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError)));

    m_ptxtinfo = new QTextEdit;
    m_ptxtinput = new QLineEdit;

    connect(m_ptxtinput, SIGNAL(returnPressed()),
                this, SLOT(slotSendToServer()));
        m_ptxtinfo->setReadOnly(true);

        QPushButton* pcmd = new QPushButton("&Send");
        connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));

        QPushButton* pExitButton = new QPushButton("&Exit");
        connect(pExitButton, SIGNAL(clicked()), SLOT(slotexitProgramm()));
        //Layout setup
        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
        pvbxLayout->addWidget(m_ptxtinfo);
        pvbxLayout->addWidget(m_ptxtinput);
        pvbxLayout->addWidget(pcmd);
        pvbxLayout->addWidget(pExitButton);
        setLayout(pvbxLayout);
}
void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize)
        {
            break;
        }

        QString str;
        in >> str;

        m_ptxtinfo->append(str);
        m_nNextBlockSize = 0;
    }
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError ?
        "The host was not found." :
        err == QAbstractSocket::RemoteHostClosedError ?
        "The remote host is closed." :
        err == QAbstractSocket::ConnectionRefusedError ?
        "The connection was refused." :
        QString(m_pTcpSocket->errorString()));

        m_ptxtinfo->append(strError);
}

void MyClient::slotSendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << m_ptxtinput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    m_ptxtinput->setText("");
}

void MyClient::slotConnected()
{
    m_ptxtinfo->append("Received the connected() signal");
}
void MyClient::slotDisconnected()
{
    m_ptxtinfo->append("Disconnected");
}

void MyClient::slotexitProgramm()
{
    this->close();//закрытие текущего окна
    emit AuthorizationWin();//Вызов сигнала на открытие главного окна
    delete this;
}
























































