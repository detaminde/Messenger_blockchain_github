#include "thread_receiviourmsg.h"

Thread_receiviourMsg::Thread_receiviourMsg(QTcpSocket *parentSocket, bool* status,
                                           QListWidget* listwidget,
                                           QJsonDocument* docBuffer_parent)
{
   p_TcpSocket_thread = parentSocket;
   flag_statusClient = status;
   listWidget_thread = listwidget;
   doc_buffer = docBuffer_parent;

}
void Thread_receiviourMsg::run()
{
    while(*flag_statusClient)
    {
        QString sendStr = "{\"request\":1000, \"type\":\"new message for client\"}";
        byteArr_thread.clear();
        byteArr_thread.append(sendStr.toLocal8Bit());
        p_TcpSocket_thread->write(byteArr_thread);
        if(Thread_receiviourMsg::ResponseFromServer_1000())
        {
            docArr_thread = QJsonValue(doc_thread.object().value("New_messages")).toArray();

            for(int i = 0; i < listWidget_thread->count(); i++)
            {
                for(int j = 0; j < docArr_thread.size(); j++)
                {
                    if(listWidget_thread->item(i)->text() == docArr_thread.at(j).toObject().value("chat_id").toString())
                    {
                        listWidget_thread->item(i)->setBackground(Qt::blue);
                    }
                }
            }
            //проверка на наличие уже новых сообщений
            if(doc_buffer->isEmpty())
            {
                *doc_buffer = doc_thread;
            }
            else
            {
                QJsonObject object {doc_buffer->object()};


                docArr_buffer = QJsonValue(doc_buffer->object().value("New_messages")).toArray();
                for(int i = 0; i < docArr_buffer.size(); i++)
                {
                    for(int j = 0; j < docArr_thread.size(); j++)
                    {
                        if(docArr_buffer.at(i).toObject().value("chat_id").toString() == docArr_thread.at(j).toObject().value("chat_id").toString())
                        {
                            QJsonArray docArr_buffer_newMessages = QJsonValue(docArr_buffer.at(i).toObject().value("BlockNums")).toArray();
                            QJsonArray docArr_thread_newMessages = QJsonValue(docArr_thread.at(j).toObject().value("BlockNums")).toArray();

                            for(int k = 0; k < docArr_buffer_newMessages.size(); k++)
                            {
                                for(int l = 0; l < docArr_thread_newMessages.size(); l++)
                                {
                                    if(docArr_buffer_newMessages.at(k).toObject().value("blocknum").toInt() ==
                                            docArr_thread_newMessages.at(l).toObject().value("blocknum").toInt())
                                    {
                                        docArr_buffer_newMessages.append(docArr_thread_newMessages.at(l));
                                    }
                                }
                            }
                            //сделать сортировку по номеру блока
                            Thread_receiviourMsg::SortQJsonArray_valueBlockNums(docArr_buffer_newMessages);
                            //здесь должно быть добавление обновленного массива в первичный массив
                            docArr_buffer.at(i).toObject().value("BlockNums") = docArr_buffer_newMessages;

                        }
                    }
                }
                //здесь сделать добавление массива в QJsonDocument
                object["New_messages"] = docArr_buffer;
                doc_buffer->setObject(object);

                        /*QJsonObject object2 {document.object()};

                        QJsonArray block = object2["Blocks"].toArray();
                        block.append(exBlock);
                        object2["Blocks"] = block;

                        document.setObject(object2);*/
            }

        }
        wait(50);
    }
}
void Thread_receiviourMsg::SortQJsonArray_valueBlockNums(QJsonArray arr)//сортировка Шелла
{
    int d = arr.size();
    QJsonValue buf;
    int j;
    d /= 2;
    while(d > 0)
    {
        for(int i = 0; i < arr.size() - d; i++)
        {
            j = i;
            while((j >= 0 )
   && (arr.at(j).toObject().value("blocknum").toInt() > arr.at(j+d).toObject().value("blocknum").toInt()))
            {
                buf = arr.at(j);
                arr.at(j) = arr.at(j+d);
                arr.at(j+d) = buf;
                j--;
            }
        }
        d /= 2;
    }
}
bool Thread_receiviourMsg::ResponseFromServer_1000()
{
    if(isConnectedToServer())
    {
        p_TcpSocket_thread->waitForReadyRead(1000);
        byteArr_thread.clear();
        byteArr_thread = p_TcpSocket_thread->readAll();
        doc_thread = QJsonDocument::fromJson(byteArr_thread, &docErr_thread);
        if(QJsonParseError::NoError == docErr_thread.error)
        {
            if((doc_thread.object().value("response").toInteger() == 1000)
                    && (doc_thread.object().value("status").toString() == "ready"))
            {
                return true;
            }
            else
                return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool Thread_receiviourMsg::isConnectedToServer()
{
    if(p_TcpSocket_thread->state() == QTcpSocket::ConnectedState)
        return true;
    else
        return false;
}

















