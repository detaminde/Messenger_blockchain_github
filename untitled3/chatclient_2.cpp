#include "chatclient_2.h"
#include "ui_chatclient_2.h"


//Сделать access-token; приватный и публичный ключ; блокчейн реализовать;


//Первостепенные задачи:
//электронная подпись
//сделать приём сообщений через потоки
//проверить все функции на логичность


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
    statusClient = true;
    if(LoadDialogsFromMemory())
    {
        if(ChatClient_2::isConnected())
        {
            if(LoadDialogs())//пересмотреть функцию
                        ui->statusbar->showMessage("Успешно были загружены диалоги с сервера");
                    else
                    {
                        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить список диалогов");
                    }
        }
    }
    else
    {
    }
    //засунуть это всё в потоки
    thread_receivour = new Thread_receiviourMsg(p_TcpSocket_chatclient,
                                                &statusClient,
                                                ui->listWidget_Dialogs,
                                                &doc_forThreadRec);
    thread_receivour->run();
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
            else
            {
                return false;
            }
        }
        else
        {
            ui->statusbar->showMessage("Ошибка: " + docError.errorString());
            return false;
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
                    if(QJsonParseError::NoError == docError.error)
                    {
                        read = true;
                        docArr = QJsonValue(doc.object().value("Blocks")).toArray();
                        //сделать проверку на номер последнего блока
                        if(ChatClient_2::RequestLastBlockNumber(doc.object().value("chat_id").toInteger(),
                                docArr.last().toObject().value("BlockNum").toInt()))
                        {
                            int docArrSize = docArr.count();
                            if(docArrSize > 100)
                                docArrSize = 100;
                            ui->textBrowser->clear();
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
                            item->setBackground(Qt::white);
                        }
                        else
                        {
                            //здесь идёт запрос и последующая выгрузка последних блоков, которые хранятся у меня в буфере doc_forThreadRec
                            QJsonDocument buffer_forThreadRec = doc_forThreadRec;
                            if(ChatClient_2::RequestForNewMessages(buffer_forThreadRec,
                                            doc.object().value("chat_id").toInteger()))
                            {
                                   //загрузка в файл в директории

                            }
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
            if(ChatClient_2::RequestDialogFromServer(item->text()))
            {

            }
            else
            {
                ui->statusbar->showMessage("Ошибка: при создании файла диалога");
                break;
            }
        }
        countRequests++;
    }while((countRequests != 2) && (!read));//пересмотреть как работает на свежую голову
}
bool ChatClient_2::RequestForNewMessages(QJsonDocument docFrom, int chat_id)
{
    if(ChatClient_2::isConnected())
    {
        QJsonArray docFrom_arr = QJsonValue(docFrom.object().value("New_Messages")).toArray();
        QJsonArray docFrom_arr_arr;
        for(int i = 0; i < docFrom_arr.size(); i++)
        {
            if(chat_id == docFrom_arr.at(i).toObject().value("chat_id").toInteger())
            {
                docFrom_arr_arr = QJsonValue(docFrom_arr.at(i).toObject().value("BlockNums")).toArray();
            }
        }
        QString sendStr = "{\"request\":1101, \"type\":\"send new messages\", \"chat_id\":" + QString::number(chat_id) + ",";
        for(int i = 0; i < docFrom_arr_arr.size(); i++)
        {
            sendStr += "\"blocknum\":" + QString::number(docFrom_arr_arr.at(i).toObject().value("blocknum").toInt()) + ",";
        }
        sendStr += "}";
        p_TcpSocket_chatclient->write(sendStr.toLocal8Bit());
        if(ChatClient_2::ResponseFromServer_1101(&doc_forLoadingMessages))
        {
            //поиск в директории название такого диалога(chat_id)
            //и последующая дозапись в него
            //и отображение
            if(file.open(QFile::ReadWrite | QFile::Text))
            {
                QJsonParseError docErr_buf_FileFromMemory;
                QJsonDocument doc_buf_FileFromMemory = QJsonDocument::fromJson(QByteArray(file.readAll()), &docErr_buf_FileFromMemory);
                file.close();
                if(QJsonParseError::NoError == docErr_buf_FileFromMemory.error)
                {
                    QJsonArray docArr_buf = doc_buf_FileFromMemory.object().value("Blocks").toArray();
                    QJsonArray docArr_folLoadingMessages = doc_forLoadingMessages.object().value("NewBlocks").toArray();
                    //мне нужно добавить в первый другой
                    for(int i = 0; i < docArr_buf.size(); i++)
                    {
                        for(int j = 0; j < docArr_folLoadingMessages.size(); j++)
                        {
                            if((docArr_folLoadingMessages.at(j).toObject().value("BlockNum").toInteger() -
                                    docArr_buf.at(i).toObject().value("BlockNum").toInt()) == 1)
                            {
                                docArr_buf.insert(i, docArr_folLoadingMessages.at(j));
                            }
                        }
                    }
                    //вставка в изначальный документ
                    //QJsonDocument
                    /*
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
                    file.close();
                    return true;
                }
                    */
                }
            }

        }
        else
        {
            return false;
        }

    }
    else
        return false;
}
bool ChatClient_2::ResponseFromServer_1101(QJsonDocument* bufferDocForNewMessages)
{
    if(isConnected())
    {
        p_TcpSocket_chatclient->waitForReadyRead(2000);
        QByteArray byte_arr_1101;
        byte_arr_1101 = p_TcpSocket_chatclient->readAll();
        QJsonParseError docErr_1101;
        QJsonDocument doc_1101 = QJsonDocument::fromJson(byte_arr_1101, &docErr_1101);
        if(QJsonParseError::NoError == docErr_1101.error)
        {
            *bufferDocForNewMessages = doc_1101;
            return true;
        }
        else
        {
            ui->statusbar->showMessage("Ошибка: " + docErr_1101.errorString());
            return false;
        }
    }
    else
    {
        ui->statusbar->showMessage("Нет соединения");
        return false;
    }
}
bool ChatClient_2::RequestLastBlockNumber(int chat_id, int blocknum)
{
    if(isConnected())
    {
        QString sendStr = "\"request\":1100, \"type\":\"lastBlockNumber\", \"chat_id\":" + QString::number(chat_id) + ",\"BlockNum\":" + QString::number(blocknum) + "}";
        p_TcpSocket_chatclient->write(sendStr.toLocal8Bit());
        return ChatClient_2::ResponseFromServer_1100();
    }
    else
    {
        return false;
    }
}
bool ChatClient_2::ResponseFromServer_1100()
{
    p_TcpSocket_chatclient->waitForReadyRead();
    QByteArray buf = p_TcpSocket_chatclient->readAll();
    QJsonParseError bufdoc_err;
    QJsonDocument bufdoc = QJsonDocument::fromJson(buf, &bufdoc_err);
    if(QJsonParseError::NoError == docError.error)
    {
        if((bufdoc.object().value("response").toInteger() == 1100) && (bufdoc.object().value("status").toString() == "this is the last block"))
            return true;
        else
            return false;
    }
    else
    {
        ui->statusbar->showMessage("Ошибка 1100");
        return false;
    }
}
bool ChatClient_2::RequestDialogFromServer(QString textFromItem)//прописать ещё создание диалога
{
    QString requestToServer = "{\"request\":\"500\", \"dialog_name\":\"" + textFromItem + "\"}";

    arrBlock_chatclient.clear();
    arrBlock_chatclient.append(requestToServer.toLocal8Bit());
    p_TcpSocket_chatclient->write(arrBlock_chatclient);
    QJsonDocument docFromServer;
    //функция проверки, которая определяет данные файла
    if(ChatClient_2::ResponseFromServer_500(textFromItem))
    {
        docFromServer = QJsonDocument::fromJson(arrBlock_chatclient, &docError);

        if(QJsonParseError::NoError == docError.error)
        {
            ui->statusbar->showMessage("Диалог загружен успешно");
            QFile newFile;
            newFile.setFileName(docFromServer.object().value("chat_id").toString());
            if(newFile.open(QFile::ReadOnly))
            {
                file.close();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            ui->statusbar->showMessage("Ошибка: " + docError.errorString());
            return false;
        }
    }
    else
    {
        ui->statusbar->showMessage("Не получилось :(");
        return false;
    }
}
bool ChatClient_2::isExistOnDevice(QString path)
{
    QFileInfo check_file(path);
        //проверяет существует файл или нет:если - да, то true; если - нет, то false
        return check_file.exists() && check_file.isFile();
}
bool ChatClient_2::ResponseFromServer_500(QString dialogName)
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
            if((flagForRequest))//а зачем мне запрос на диалог, если я на него нажал, т.е. получается двойная проверкаы
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
                                    ui->statusbar->showMessage("Не получилось замайнить блок :(");//поменять на функцию замены всех невалидных блоков на валидные
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

            }
            countRequest++;
        }while((!flagForRequest) && (countRequest != 2));

    }
    else
    {
        ui->statusbar->showMessage("Введите сообщение");
    }

}
bool ChatClient_2::SendBlockToServer(Block block)//пересылка блока на сервер
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
                +"\"request\":601, \"type\":\"save message in server\"}";
        QByteArray byteArr_buf;
                byteArr_buf.append(sendStr.toLocal8Bit());
                p_TcpSocket_chatclient->write(byteArr_buf);
                return true;
    }
    else
    {
        ui->statusbar->showMessage("Отсутствует подключение к серверу");
        return false;
    }
}
bool ChatClient_2::SaveMessageToMemory(QString pathToFile, Block block)//сохранение собщения в память
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
            stringstream ss;
            ss<<block.GetNonce();
            exBlock.insert("Nonce", QString::fromStdString(ss.str()));//поменять функцию
            document = QJsonDocument::fromJson(QByteArray(file.readAll()), &docErr);
            if(QJsonParseError::NoError == docError.error)
            {
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
                    file.close();
                    return true;
                }
                else
                {
                    file.close();
                    return false;
                }
            }
            else
            {
                file.close();
                ui->statusbar->showMessage("Ошибка: " + docErr.errorString());
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

    if(QJsonParseError::NoError == docError.error)
    {
        if(lastHash == docbuf.object().value("lastHashServer").toString())
            return true;
        else
            return false;
    }
    else
    {
        ui->statusbar->showMessage("Ошибка: " + docError.errorString());
        return false;
    }
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
void ChatClient_2::on_Exit_triggered()//выход из профиля
{
    this->close();
    emit BackToAuthorizationWindow();
}
bool ChatClient_2::isConnected()//проверка на соединение
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
                    ui->listWidget_Dialogs->addItem(peopleArr.at(i).toString());
                }
            }
            else
            {
                ui->statusbar->showMessage(doc.object().value("error").toString());//напомнить Петренко об этом
            }

        }
    }
}
bool ChatClient_2::ResponseFromServer_900()//запрос на пересылку списка людей по заданному критерию
{
    p_TcpSocket_chatclient->waitForReadyRead(2000);
    arrBlock_chatclient.clear();
    arrBlock_chatclient = p_TcpSocket_chatclient->readAll();

    doc = QJsonDocument::fromJson(arrBlock_chatclient, &docError);
    if(QJsonParseError::NoError == docError.error)
    {
        if(doc.object().value("response").toInt() == 901)//уточнить ещё этот номер
        {
            return true;
        }
        else
            return false;
    }
    else
    {
        ui->statusbar->showMessage("Ошибка: " + docError.errorString());
        return false;
    }

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
                                                     "JPG(*.jpg);;PNG(*.png);;All Files(*.*)");
}


void ChatClient_2::on_lineEdit_findPeople_returnPressed()
{
    on_pushButton_FindPeople_clicked();
}







