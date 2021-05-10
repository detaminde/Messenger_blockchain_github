#include "chatclient_2.h"
#include "ui_chatclient_2.h"


//Сделать access-token; приватный и публичный ключ; блокчейн реализовать;


//Первостепенные задачи:
//чтобы мы сделали базу данных сообщений и их переадресацию пользователю
//сделать прочитку диалога с сервера и сохранненого файла
ChatClient_2::ChatClient_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient_2)
{
    p_TcpSocket_chatclient = new QTcpSocket();
    p_TcpSocket_chatclient->connectToHost("localhost", 80);
    ui->setupUi(this);
    if(isConnected())
        ui->statusbar->showMessage("Вы подключены");
    else
        ui->statusbar->showMessage("Соединение отсутствует");

}

void ChatClient_2::on_listWidget_Dialogs_itemClicked(QListWidgetItem *item)//добавить отображение сообщений в label
{   //item - это chat_id
    //добавить сами диалоги
    //пока что он отображает один диалог
    //сделать так: один item - один файл json
    //добавить сначала проверку на наличие файла в памяти ПК, если его нет, то запрос на сервер для скачки


    //Присобачить блокчейн после того, как доделаю network
    globpath = "C:\\Users\\User\\Documents\\QTfiles\\Test3\\untitled3\\json\\" + item->text() + ".json";
    int countRequests = 0;
    do
    {
        if(isExistOnDevice(globpath))
        {
            file.setFileName(globpath);
            //поле chat_id хранится на сервере, как и весь файл
            ui->statusbar->showMessage(item->text());
            if(file.open(QIODevice::ReadOnly | QFile::Text))
            {
                doc = QJsonDocument::fromJson(QByteArray(file.readAll()), &docError);
                file.close();
                if(!doc.isEmpty())
                {
                    if(QJsonParseError::NoError == docError.error)//добавить условие пустоты файла внутрь
                    {
                        docArr = QJsonValue(doc.object().value("chatData")).toArray();
                        int docArrSize = docArr.count();
                        if(docArrSize > 100)
                            docArrSize = 100;
                        for(int i = 0; i < docArrSize; i++)//читает первые 100 сообщений
                            //добавить стилизацию текста сообщений
                        {

                           QString bufferForTextBrowser = docArr.at(i).toObject().value("time").toString()
                                   + ": "
                                   + docArr.at(i).toObject().value("username").toString()
                                   + ": "
                                   + docArr.at(i).toObject().value("message").toString();//добавить условие отображение справа:
                                                    //если отправитель - ты, то сообщение справа(скомбинирвать вместе с label)
                          ui->textBrowser->append(bufferForTextBrowser);
                        }
                    }
                    else
                    {
                        ui->statusbar->showMessage("Ошибка при прочтении файла: " + docError.errorString());
                    }
                }
                else
                {
                        ui->textBrowser->append("Здесь пусто");
                }
            }
            else
            {
                ui->statusbar->showMessage("Cannot open file");
            }
        }
        else//если нет, то запрос на сервер для закачки
        {
            QString requestToServer = "{\"request\":\"send dialog\", \"dialog_name\":\"" + item->text() + "\"}";

            arrBlock_chatclient.clear();
            arrBlock_chatclient.append(requestToServer.toLocal8Bit());
            p_TcpSocket_chatclient->write(arrBlock_chatclient);

            //функция проверки, которая определяет данные файла
            if(ChatClient_2::ResponseFromServer_300(item->text()))
                ui->statusbar->showMessage("Диалог загружен успешно");
            else
                ui->statusbar->showMessage("Не получилось :(");
        }
        countRequests++;
    }while(countRequests != 2);
}
bool ChatClient_2::isExistOnDevice(QString path)
{
    QFileInfo check_file(path);
        //проверяет существует файл или нет:если - да, то true; если - нет, то false
        return check_file.exists() && check_file.isFile();
}
bool ChatClient_2::ResponseFromServer_300(QString dialogName)
{
    p_TcpSocket_chatclient->waitForReadyRead(3000);
    arrBlock_chatclient.clear();
    arrBlock_chatclient = p_TcpSocket_chatclient->readAll();

    QString newFileName = "C:\\Users\\User\\Documents\\QTfiles\\Test3\\untitled3\\json" + dialogName + ".json";
    QFile newDialogFile(newFileName);
    if(newDialogFile.open(QIODevice::WriteOnly))
    {
        newDialogFile.write(arrBlock_chatclient);
        return true;
    }
    else
        return false;
}

void ChatClient_2::on_Send_clicked()//прописать отправку на сервер и сохранение этого же сообщения в памяти ПК
{

    QString YourText = ui->lineEdit->text();
    ui->textBrowser->append(YourText);
    ui->lineEdit->setText("");

}
void ChatClient_2::slotError(QAbstractSocket::SocketError err)
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
void ChatClient_2::on_Exit_triggered()
{
    this->close();
    emit BackToAuthorizationWindow();
}
bool ChatClient_2::isConnected()
{
    if(p_TcpSocket_chatclient->state() == QTcpSocket::ConnectedState)
        return true;
    else
        return false;
}
