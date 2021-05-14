#include "chatclient_2.h"
#include "ui_chatclient_2.h"


//Сделать access-token; приватный и публичный ключ; блокчейн реализовать;


//Первостепенные задачи:
//чтобы мы сделали базу данных сообщений и их переадресацию пользователю
//сделать прочитку диалога с сервера и сохранненого файла
//приделать блокчейн к этому всему
//приделать отправку онлайна и добавление в друзья(запрос с сервера)


//таски на 179 строке
ChatClient_2::ChatClient_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient_2)
{
    p_TcpSocket_chatclient = new QTcpSocket();
    p_TcpSocket_chatclient->connectToHost("localhost", 80);
    ui->setupUi(this);
    if(ChatClient_2::isConnected())
        ui->statusbar->showMessage("Вы подключены");
    else
        ui->statusbar->showMessage("Соединение отсутствует");
   // ui->listWidget_Dialogs->addItem("testJson");

    if(LoadDialogsFromMemory())
    {

    }
    else
    {
        if(LoadDialogs())
            ui->statusbar->showMessage("Успешно были загружены диалоги с сервера");
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось загрузить список диалогов");
        }
    }
}
bool ChatClient_2::LoadDialogs()
{
    if(isConnected())
    {
        QString sendStr = "{\"request\":901, \"type\":\"load dialogs list\"}";
        arrBlock_chatclient.clear();
        arrBlock_chatclient.append(sendStr.toLocal8Bit());
        p_TcpSocket_chatclient->write(arrBlock_chatclient);

        if(ChatClient_2::ResponseFromServer_901())
        {
            docArr = QJsonValue(doc.object().value("Dialogs_list")).toArray();
            for(int i = 0; i < docArr.size(); i++)
            {
                ui->listWidget_Dialogs->addItem(docArr.at(i).toString());
            }
            return true;
        }
        else
        {
            ui->statusbar->showMessage("Ошибка при загрузке списка диалогов");
            return false;
        }
    }
    else
    {
        ui->statusbar->showMessage("Отсутствует соединение");
        return false;
    }
}
bool ChatClient_2::ResponseFromServer_901()//мб здесь ошибка в возвращаемой переменной
{
    if(p_TcpSocket_chatclient->isOpen())
    {
        p_TcpSocket_chatclient->waitForReadyRead(2000);
        arrBlock_chatclient.clear();
        arrBlock_chatclient = p_TcpSocket_chatclient->readAll();
        doc = QJsonDocument::fromJson(arrBlock_chatclient, &docError);
        if(QJsonParseError::NoError == docError.error)
        {
            if((doc.object().value("response").toInt() == 901) && (doc.object().value("status").toString() == "succesful"))
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }

}
ChatClient_2::ChatClient_2(QTcpSocket* parentSocket, QWidget* parent): QMainWindow(parent), ui(new Ui::ChatClient_2)
{
    p_TcpSocket_chatclient = parentSocket;
    ui->setupUi(this);
    if(ChatClient_2::isConnected())
        ui->statusbar->showMessage("Вы подключены");
    else
        ui->statusbar->showMessage("Соединение отсутствует");
    ui->listWidget_Dialogs->addItem("testJson");
}
bool ChatClient_2::LoadDialogsFromMemory()//убрать .json из имен файлов
{
    QString path = "C:\\Users\\User\\Documents\\QTfiles\\Test3\\untitled3\\json";

    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkpath(path);
        return false;
    }
    else
    {
        QDir myPath(path);
        myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList myList = myPath.entryList();
        ui->listWidget_Dialogs->addItems(myList);
        return true;
    }
}
void ChatClient_2::on_listWidget_Dialogs_itemClicked(QListWidgetItem *item)//добавить отображение сообщений в label
{   //item - это chat_id и блокчейн одновременно
    //сделать так: один item - один файл json
    //добавить сначала проверку на наличие файла в памяти ПК, если его нет, то запрос на сервер для скачки


    //Присобачить блокчейн после того, как доделаю network
    globpath = "C:\\Users\\User\\Documents\\QTfiles\\Test3\\untitled3\\json\\" + item->text();
    buffer = item->text();
    int countRequests = 0;
    bool read = false;
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
                        read = true;
                        docArr = QJsonValue(doc.object().value("Blocks")).toArray();
                        int docArrSize = docArr.count();
                        if(docArrSize > 100)
                            docArrSize = 100;
                        for(int i = 2; i < docArrSize; i++)//читает первые 100 сообщений
                            //добавить стилизацию текста сообщений
                        {

                           QString bufferForTextBrowser = docArr.at(i).toObject().value("TimeCreation").toString()
                                   + ": "
                                   + docArr.at(i).toObject().value("creatorNickname").toString()
                                   + ": "
                                   + docArr.at(i).toObject().value("Data").toString();//добавить условие отображение справа:
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
            doc = ChatClient_2::RequestDialogFromServer(item->text());
        }
        countRequests++;
    }while((countRequests != 2) && (!read));//пересмотреть как работает на свежую голову
}
QJsonDocument ChatClient_2::RequestDialogFromServer(QString textFromItem)
{
    QString requestToServer = "{\"request\":\"send dialog\", \"dialog_name\":\"" + textFromItem + "\"}";

    arrBlock_chatclient.clear();
    arrBlock_chatclient.append(requestToServer.toLocal8Bit());
    p_TcpSocket_chatclient->write(arrBlock_chatclient);
    QJsonDocument docFromServer;
    //функция проверки, которая определяет данные файла
    if(ChatClient_2::ResponseFromServer_300(textFromItem))
    {
        docFromServer = QJsonDocument::fromJson(arrBlock_chatclient, &docError);
        ui->statusbar->showMessage("Диалог загружен успешно");
        if(QJsonParseError::NoError == docError.error)
            return docFromServer;
        else
        {
            ui->statusbar->showMessage("Ошибка: " + docError.errorString());
            return false;
        }
    }
    else
    {
        ui->statusbar->showMessage("Не получилось :(");
        return docFromServer;
    }
}
bool ChatClient_2::isExistOnDevice(QString path)
{
    QFileInfo check_file(path);
        //проверяет существует файл или нет:если - да, то true; если - нет, то false
        return check_file.exists() && check_file.isFile();
}
bool ChatClient_2::ResponseFromServer_300(QString dialogName)
{
    p_TcpSocket_chatclient->waitForReadyRead(3500);
    arrBlock_chatclient.clear();
    arrBlock_chatclient = p_TcpSocket_chatclient->readAll();

    QString newFileName = "C:\\Users\\User\\Documents\\QTfiles\\Test3\\untitled3\\json" + dialogName + ".json";
    QFile newDialogFile(newFileName);
    if(newDialogFile.open(QIODevice::WriteOnly))
    {
        newDialogFile.write(arrBlock_chatclient);
        newDialogFile.close();
        return true;
    }
    else
        return false;
}
//завтра доделать функцию сохранения/отправки полностью + (done)
//ещё сделать функцию добавления вложенного файла(сделано) +
    //{сохранение в сжатом кеше, отправка на сервер}
//сделать поиск людей(сделано)
    //{запрос на сервер простенький}
//сделать приём файла(изображение и т.д.) с сервера(прописать в блоке)

//затем можно будет переходить к фронтэнду
//посоветоваться с Петреко на счёт ещё одной переменной в классе Block
void ChatClient_2::on_Send_clicked()//прописать отправку на сервер и сохранение этого же сообщения в памяти ПК
{   //пробная версия - потом переделаем под асинхронное шифрование
    //до взятия из строки нужно открыть последний блок в массиве json файла и взять его хеш
    if(!ui->lineEdit->text().isEmpty())
    {
        QString YourText = ui->lineEdit->text();
        bool flagForRequest = false;
        int countRequest = 0;
        do
        {
            if((isExistOnDevice(globpath)) || (flagForRequest))
            {
                file.setFileName(globpath);
                //поле chat_id хранится на сервере, как и весь файл
                if(file.open(QIODevice::ReadWrite | QFile::Text))
                {
                    doc = QJsonDocument::fromJson(QByteArray(file.readAll()), &docError);
                    file.close();
                    if(!doc.isEmpty())
                    {
                        if(QJsonParseError::NoError == docError.error)
                        {
                            if(ChatClient_2::isConnected())
                            {
                                //добавить условие, если есть наличие прикрепленного файла
                                if(!path_attachedFile.isEmpty())
                                {
                                    //здесь добавление в байтах в блок отдельной переменной
                                }
                                docArr = QJsonValue(doc.object().value("Blocks")).toArray();
                                int numlastblock = docArr.size();
                                QString sendHash = "\"request\":600, \"lastHash\":\""
            + docArr.at(numlastblock).toObject().value("Hash").toString() + "\", \"chat_id\":\"" + QString::number(doc.object().value("chat_id").toInt())
            + "\"}";
                                arrBlock_chatclient.clear();
                                arrBlock_chatclient.append(sendHash.toLocal8Bit());
                                p_TcpSocket_chatclient->write(arrBlock_chatclient);
                                if(LastHashFromServer(docArr.at(numlastblock).toObject().value("Hash").toString()))
                                    //если true, то майним блок, сохраняем его и отсылаем на сервер
                                {
                                    //связаться с Петренко для того, чтобы реализовать сохранение в файл

                                    //создать файл в котором хранится никнейм текущего юзера
                                    Block newBlockForJson(YourText.toStdString());
                                    newBlockForJson.setPrevHash(docArr.at(numlastblock).toObject().value("Hash").toString().toStdString());
                                    newBlockForJson.setCreatorNickName("sys");
                                    newBlockForJson.MineBlock(3);

                                    if(ChatClient_2::SaveMessageToMemory(globpath, newBlockForJson))
                                    {
                                            ui->statusbar->showMessage("Блок был замайнен и сохранён");
                                            //отправить блок на сервак
                                            ChatClient_2::SendBlockToServer(newBlockForJson);
                                            path_attachedFile.clear();
                                            break;
                                    }
                                    else
                                    {
                                        ui->statusbar->showMessage("Ошибка при сохранении блока");
                                    }

                                }
                                else
                                {
                                    ui->statusbar->showMessage("Не получилось замайнить блок :(");
                                }
                            }
                            else
                            {
                                ui->statusbar->showMessage("Подключение отсутствует");
                            }
                        }
                        else
                        {
                            ui->statusbar->showMessage("Ошибка при прочтении файла: " + docError.errorString());
                        }
                        ui->textBrowser->append(YourText);
                        ui->lineEdit->setText("");
                    }
                    else
                    {
                        //если файл пустой, то нунжо будет его заполнить и отправить на сервер
                    }
                }
                else
                {
                    ui->textBrowser->append("Невозможно отобразить диалог.");
                }
            }
            else
            {
                //запрос файла с сервера
                doc = ChatClient_2::RequestDialogFromServer(buffer);
                flagForRequest = true;//пересмотреть на свежую голову как работает этот цикл
            }
            countRequest++;
        }while((!flagForRequest) && (countRequest != 2));

    }
    else
    {
        ui->statusbar->showMessage("Введите сообщение");
    }

}
bool ChatClient_2::SendBlockToServer(Block block)
{
    if(ChatClient_2::isConnected())
    {
        QString sendStr = "{\"Data\":\"" + QString::fromStdString(block.getBlockData().c_str())
                +"\",\"Hash\":\"" + QString::fromStdString(block.GetHash().c_str())
                +"\",\"Nonce\":\"" + QString::number(block.GetNonce())
                +"\",\"PrevHash\":\"" + QString::fromStdString(block.GetPrevHash().c_str())
                +"\",\"TimeCreation\":\"" + QString::fromStdString(block.getTime().c_str())
                +"\",\"creatorNickName\":\"" + QString::fromStdString(block.GetCreatorNickName().c_str())
                +"\","
                +"\"request\":600, \"type\":\"save message in server\"}";
        QByteArray byteArr_buf;
                byteArr_buf.append(sendStr.toLocal8Bit());
                p_TcpSocket_chatclient->write(byteArr_buf);
    }
    else
    {
        ui->statusbar->showMessage("Отсутствует подключение к серверу");
    }
}
bool ChatClient_2::SaveMessageToMemory(QString pathToFile, Block block)
{

    QFile file;
    file.setFileName(pathToFile);

    if(file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QJsonDocument document;
            QJsonParseError docErr;

            //проставить под значения block
            QJsonObject exBlock;
            exBlock.insert("creatorNickname", QString::fromStdString(block.GetCreatorNickName().c_str()));
            exBlock.insert("Data", QString::fromStdString(block.getBlockData().c_str()));
            exBlock.insert("Hash", QString::fromStdString(block.GetHash().c_str()));
            exBlock.insert("Time_Creation", QString::fromStdString(block.getTime().c_str()));
            exBlock.insert("PrevHash", QString::fromStdString(block.GetPrevHash().c_str()));
            exBlock.insert("Nonce", QString::number(block.GetNonce()));//поменять функцию
            document = QJsonDocument::fromJson(QByteArray(file.readAll()), &docErr);
            file.close();
            QJsonObject object2 {document.object()};

            QJsonArray block = object2["Blocks"].toArray();
            block.append(exBlock);
            object2["Blocks"] = block;

            document.setObject(object2);
            qDebug() << qPrintable(document.toJson(QJsonDocument::Indented));

            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                file.write(document.toJson());
                return true;
            }
            else
            {
                return false;
            }
        }
    else
        return false;
}
bool ChatClient_2::LastHashFromServer(QString lastHash)
{
    p_TcpSocket_chatclient->waitForReadyRead(2000);
    arrBlock_chatclient.clear();
    arrBlock_chatclient = p_TcpSocket_chatclient->readAll();

    QJsonDocument docbuf = QJsonDocument::fromJson(arrBlock_chatclient, &docError);

    if(lastHash == docbuf.object().value("lastHashServer").toString())
        return true;
    else
        return false;
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

void ChatClient_2::on_pushButton_FindPeople_clicked()
{
    if(ChatClient_2::isConnected())
    {
        if(!ui->lineEdit_findPeople->text().isEmpty())
        {
            QString sendStr;
            sendStr = "{\"request\":900, \"type\":\"get online people\", \"nickname\":\"}" + ui->lineEdit_findPeople->text() + "\"}";
            if(ChatClient_2::ResponseFromServer_900())
            {
                //вывод вместо диалогов
                ui->listWidget_Dialogs->clear();
                QJsonArray peopleArr = QJsonValue(doc.object().value("peopleBySentNickname")).toArray();
                for(int i = 0; i < peopleArr.size(); i++)
                {
                    ui->listWidget_Dialogs->addItem(peopleArr.at(i).toString()); // сделать перевод QStringList и последующее добавление
                }
            }
            else
            {
                ui->statusbar->showMessage(doc.object().value("error").toString());//напомнить Петренко об этом
            }

        }
    }
}
bool ChatClient_2::ResponseFromServer_900()
{
    p_TcpSocket_chatclient->waitForReadyRead(2000);
    arrBlock_chatclient.clear();
    arrBlock_chatclient = p_TcpSocket_chatclient->readAll();

    doc = QJsonDocument::fromJson(arrBlock_chatclient, &docError);
    if(doc.object().value("response").toInt() == 901)//уточнить ещё этот номер
    {
        return true;
    }
    else
        return false;

}
void ChatClient_2::on_lineEdit_finePeople_returnPressed()
{
    on_pushButton_FindPeople_clicked();
}

void ChatClient_2::on_lineEdit_findPeople_textChanged(const QString &someText)
{
    if(someText == "")
    {
        if(LoadDialogsFromMemory())
        {
            ui->statusbar->showMessage("Диалоги после поиска людей были загружены");
        }
        else
            QMessageBox::critical(this, "Ошибка", "Не удалось загрузить диалоги после поиска людей");
    }
}

void ChatClient_2::on_pushButton_clicked()
{
    path_attachedFile = QFileDialog::getOpenFileName(this,
                                                     QString::fromUtf8("Выбрать файл"),
                                                     QDir::currentPath(),
                                                     "JPG(*.jpg);;All Files(*.*)");
}







