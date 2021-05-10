#include "registration.h"
#include "ui_registration.h"

using namespace std;
Registration::Registration( QTcpSocket *parentSocket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Registration)
{
    p_TcpSocket_registration = parentSocket;
    ui->setupUi(this);
    Registration::Connected();
}

Registration::~Registration()
{
    delete ui;
}



void Registration::on_Exit_clicked()
{
    this->close();//закрытие текущего окна
    emit openAuthorizationWin();//Вызов сигнала на открытие главного окна
    delete this;
}

void Registration::on_SignUp_clicked()
{

   /* connect(p_TcpSocket_registration, SIGNAL(connected()), SLOT(slotConnected()));

    connect(p_TcpSocket_registration, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(slotError(QAbstractSocket::SocketError)));*/

    //сделать проверку на русский язык в этих двух полях
    if(Registration::Connected())
    {
        if((ui->Login_edit->text() == "") || (ui->Pass_edit->text() == ""))
            ui->statusbar->showMessage("Некорректно введен логин/пароль");
        else
            Registration::SendToServer();
    }

}
bool Registration::Connected()
{
    bool connected = (p_TcpSocket_registration->state() == QTcpSocket::ConnectedState);
    if(connected)
    {
        ui->statusbar->showMessage("Соединение установлено");
        return true;
    }
    else
    {
        ui->statusbar->showMessage("Соединение отсутствует");
        return false;
    }
}

void Registration::SendToServer()
{
    if(p_TcpSocket_registration->isOpen())
    {
        QString sendStr ="{\"type\": \"registration new user\", \"data\":{\"login\":\"" + ui->Login_edit->text()+ "\", \"password\":\""
              + ui->Pass_edit->text() + "\", \"about_yourself\":\""+ui->AbYourSlf_edit->text()
              +  "\", \"first_name\":\"" + ui->FName_edit->text() + "\", \"second_name\":\"" + ui->SName_edit->text() + "\"}}";

        //формат: {\"regLogin\":\"$text$\", \"regPass\":\"$text$\", \"regAbYrs\":\"$text$\",\"regFName\":\"$text$\", \"regSName\":\"$text$\"}

        byteArr.append(sendStr.toLocal8Bit());
        p_TcpSocket_registration->write(byteArr);
        byteArr.clear();
        connect(p_TcpSocket_registration, SIGNAL(readyRead()), SLOT(slotReadyRead_registration));
    }
    else
        ui->statusbar->showMessage("Произошла ошибка");
}
void Registration::slotReadyRead_registration()
{
    p_TcpSocket_registration->waitForReadyRead(2000);

    byteArr = p_TcpSocket_registration->readAll();

    doc = QJsonDocument::fromJson(byteArr, &docError);

    if(QJsonParseError::NoError == docError.error)
    {
        if((doc.object().value("response").toInt() == 201) && (doc.object().value("status").toString() == "created"))
        {
            ui->statusbar->showMessage("Регистрация успешна!");
            byteArr.clear();
            this->close();//закрытие текущего окна
            disconnect(p_TcpSocket_registration, SIGNAL(readyRead()), p_TcpSocket_registration, SLOT(slotReadyRead_registration()));
            emit openAuthorizationWin();//Вызов сигнала на открытие главного окна

            //переделать открытие авторизации
            //поставить чистку byteArr
        }
        else
        {
            ui->statusbar->showMessage("Ошибка регистрации: " + doc.object().value("status").toString());
        }
    }
    else
    {
        ui->statusbar->showMessage(docError.errorString());
    }
}


















