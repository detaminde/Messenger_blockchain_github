#include "chatclient.h"
#include "ui_chatclient.h"
#include <QWidget>

//Сделать access-token; приватный и публичный ключ; блокчейн реализовать;


//Первостепенные задачи:
//Теперь нужно будет сделать сохранение диалогов в памяти компьютера
//и их последующее отображение в listWidget_Chat,
//также сказать Даниилу, чтобы мы сделали базу данных сообщений и их переадресацию пользователю

ChatClient::ChatClient(QTcpSocket *parentSocket, QWidget *parent)
     : QMainWindow(parent), m_nNextBlockSize_chatclient(0)
 {
    p_TcpSocket_chatclient = parentSocket;
    ui->setupUi(this);
    //сделать сохранение на компе диалогов в формате Json и последующее их открытие в окне listWidget_Chat
    ui->listWidget_Dialogs->addItem("Person 1");
    ui->listWidget_Dialogs->addItem("Person 2");
    ui->listWidget_Dialogs->addItem("Person 3");
    ui->listWidget_Dialogs->addItem("Person 4");
    ui->listWidget_Dialogs->addItem("Person 5");
    ui->listWidget_Dialogs->addItem("Person 6");
    ui->listWidget_Dialogs->addItem("Person 7");







    //сделать проверку на залогинность

 }

void ChatClient::slotConnected()
{
    ui->statusbar->showMessage("Соединение успешно");


}
void ChatClient::slotReadyRead_ResponseFromConnect()
{
    ui->statusbar->showMessage("ну да...");
    //сделать отключение этого слота
    //disconnect(p_TcpSocket_chatclient, SIGNAL(readyRead()), SLOT(slotReadyRead_ResponseFromConnect()), );
}
void ChatClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError ?
        "The host was not found." :
        err == QAbstractSocket::RemoteHostClosedError ?
        "The remote host is closed." :
        err == QAbstractSocket::ConnectionRefusedError ?
        "The connection was refused." :
        QString(p_TcpSocket_chatclient->errorString()));

        ui->statusbar->showMessage(strError);
}


void ChatClient::on_listWidget_Dialogs_itemClicked(QListWidgetItem *item)
{
    //сделать класс Dialog
    //сделать выгрузку из файла в поле ChatBrowser через цикл
    QString YourText = item->text();
    ui->ChatBrowser->append(YourText);
}

void ChatClient::on_pushButton_send_clicked()
{
    //сделать сохранение нового сообщения в файл
    QString YourText = ui->lineEdit_message->text();
    ui->ChatBrowser->append(YourText);
    ui->lineEdit_message->setText("");
}

