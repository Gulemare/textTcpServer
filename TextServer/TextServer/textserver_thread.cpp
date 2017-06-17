#include "textserver_thread.h"
#include "textserver_communication_protocol.h"

using namespace textserver;

ServerThread::ServerThread(quintptr _descriptor,  QObject * parent):
    QThread(parent),
    descriptor(_descriptor)
{
}

void ServerThread::run()
{
    // Начало нового потока, сокет должен создаваться здесь
    socket = new QTcpSocket;
    if (!socket->setSocketDescriptor(descriptor)) {
        emit error(socket->error());
        socket->deleteLater();
        qDebug() << "THREAD" << QThread::currentThreadId()
                 << "couldn't start";
        return;
    }

    qDebug() << "THREAD" << QThread::currentThreadId()
             << "started with socket descriptor" << descriptor;

    connect(socket,SIGNAL(readyRead()), this,
                   SLOT(read()), Qt::DirectConnection);

    connect(socket,SIGNAL(disconnected()), this,
                   SLOT(disconnect()), Qt::DirectConnection);

    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)), this,
                   SLOT(socket_error(QAbstractSocket::SocketError)), Qt::DirectConnection);

    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_4_0);

    exec();
}

void ServerThread::read()
{
    // Чтение посылки
    in.startTransaction();
    in >> text;
    if (!in.commitTransaction())
            return;

    // Конец чтения
    // Парсинг префикса, заданного клиентом
    int n = text.indexOf(' ');
    int request = text.left(n).toInt();
    text.remove(0,n+1);

    // Обработка текста и отправка обратного сообщения
    processText(request);
    write();
    qDebug() << "THREAD" << QThread::currentThreadId() << "task with request"
             << COMMANDS_NAMES.value(request) << "completed";
}

void ServerThread::disconnect()
{
    qDebug() << "THREAD" << QThread::currentThreadId() << "closing...";
    socket->deleteLater();
    exit(0);
}

void ServerThread::socket_error(QAbstractSocket::SocketError error)
{
    qDebug() << "THREAD" << QThread::currentThreadId() << "SOCKET ERROR:"
             << socket->errorString();
}

void ServerThread::processText(int cmd)
{
    switch(cmd) {
    case COMMANDS::SYMBOL_SORT:
    {
        text.replace("\n","");
        std::sort(text.begin(),text.end());
        break;
    }

    case COMMANDS::MIRROR_TURN:
    {
        int n = text.size();
        QChar dummy;
        for (int i = 0; i <= n/2; ++i) {
            dummy = text[i];
            text[i] = text[n - i];
            text[n - i] = dummy;
        }
        break;
    }

    case COMMANDS::STRINGS_SORT:
    {
        QStringList list = text.split('\n', QString::SkipEmptyParts);
        std::sort(list.begin(),list.end());
        text = list.join('\n');
        break;
    }

    case COMMANDS::SYMBOL_STATISTICS:
    {
        text.replace("\n","");
        text.replace("\t","");
        text.replace(" ","");
        std::sort(text.begin(),text.end());
        QMultiMap<int,QChar> statistics;
        QChar prev_sym = text[0];
        int n = 0;
        for (auto it = text.cbegin(); it != text.cend(); ++it) {
            QChar sym = *it;
            if (sym != prev_sym) {
                statistics.insert(n,prev_sym);
                prev_sym = sym;
                n = 0;
            }
            n++;
            if (it == text.cend() - 1) {
                statistics.insert(n,prev_sym);
            }
        }
        text = "";
        for (auto it = statistics.cbegin(); it != statistics.cend(); ++it) {
            QString new_line(QString::number(it.key()));
            new_line += " - ";
            new_line += QString(it.value());
            new_line += "\n";
            text.push_front(new_line);
        }
        break;
    }
    default:
    {
        text = "unknown command";
        break;
    }
    }
    return;
}

void ServerThread::write()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << text;
    socket->write(block);
}
