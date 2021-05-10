#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pTcpSocket_signIn = new QTcpSocket(this);
    ui->statusbar->showMessage("Соединение с сервером...");
    m_pTcpSocket_signIn->connectToHost("b8570adb8ac2.sn.mynetname.net", 1567);
    connect(m_pTcpSocket_signIn, SIGNAL(connected()), SLOT(slotConnected()));

    connect(m_pTcpSocket_signIn, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError)));

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_Sign_up_clicked()
{

    p_Registration_window = new Registration(m_pTcpSocket_signIn);//инициализация окна регистрации
    connect(p_Registration_window, &Registration::openAuthorizationWin, this, &MainWindow::show);//подключение к слоту запуска главного окна по кнопке окна регистрации
    p_Registration_window->show();
    this->close();
}

void MainWindow::on_Sign_in_clicked()
{
    if(m_pTcpSocket_signIn->isOpen())
    {
        Data.clear();
        QString sendStr = "{\"type\":\"loging user login\",\"login\":\"" + ui->LoginLineEdit->text() + "\"}";

        Data.append(sendStr.toLocal8Bit());
        m_pTcpSocket_signIn->write(Data);


        if(MainWindow::ResponseFromServer_100())
        {
            Data.clear();
            sendStr = "{\"type\":\"loging user password\",\"check_password_data\":\"" + QString::number(buffer) + "\"}";

            Data.append(sendStr.toLocal8Bit());
            m_pTcpSocket_signIn->write(Data);

            if(MainWindow::ResponseFromServer_202())
            {
                Data.clear();
                p_chat_window = new ChatClient_2();
                connect(p_chat_window, &ChatClient_2::BackToAuthorizationWindow, this, &MainWindow::show);
                //connect(p_Chat_window, &ChatClient::AuthorizationWin, this, &MainWindow::show);
                p_chat_window->show();

                this->close();
            }
            else
            {
                Data.clear();
                ui->statusbar->showMessage("Неправильно введён логин/пароль");
            }
        }

    }
    else
        ui->statusbar->showMessage("Нет соединения");
    //{"login":"$username$", "password":"$password$"}


}

/*
    p_Chat_window = new MyClient("b8570adb8ac2.sn.mynetname.net", 1567);
    connect(p_Chat_window, &MyClient::AuthorizationWin, this, &MainWindow::show);
    p_Chat_window->show();
    this->close();
*/

void MainWindow::on_Exit_clicked()
{
    delete p_Registration_window;
    delete p_chat_window;
    delete m_pTcpSocket_signIn;
    delete ui;
    this->close();
    //присылать пакет при выходе
}
void MainWindow::slotConnected()//сделать это как булевую
{
    ui->statusbar->showMessage("Сервер работает");
}

bool MainWindow::ResponseFromServer_202()
{
    m_pTcpSocket_signIn->waitForReadyRead(1000);

    Data = m_pTcpSocket_signIn->readAll();
    doc = QJsonDocument::fromJson(Data, &docError);
    Data.clear();
    if(QJsonParseError::NoError == docError.error)
    {
        if((doc.object().value("response").toInt() == 202) &&
                (doc.object().value("status").toString() == "login successfully"))
        {
            return true;
        }
        else
        {
            ui->statusbar->showMessage(doc.object().value("status").toString());
            return false;
        }
    }
    else
    {
        ui->statusbar->showMessage(docError.errorString());
        return false;
    }

}
bool MainWindow::ResponseFromServer_100()
{
    m_pTcpSocket_signIn->waitForReadyRead(1000);

    Data = m_pTcpSocket_signIn->readAll();

    doc = QJsonDocument::fromJson(Data, &docError);
    Data.clear();
    if(QJsonParseError::NoError == docError.error)
    {
        if((doc.object().value("response").toInt() == 100))
        {
            //buffer = doc.object().value("check_password_data").toInt();
            buffer = ui->PassLineEdit->text().size();//поставить сюда норм функцию хеширования
            return true;
        }
        else
        {
            ui->statusbar->showMessage(doc.object().value("status").toString());
            return false;
        }
    }
    else
    {
        ui->statusbar->showMessage(docError.errorString());
        return false;
    }
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError ?
        "The host was not found." :
        err == QAbstractSocket::RemoteHostClosedError ?
        "The remote host is closed." :
        err == QAbstractSocket::ConnectionRefusedError ?
        "The connection was refused." :
        QString(m_pTcpSocket_signIn->errorString()));

        ui->statusbar->showMessage(strError);
}




















