#include "textserver.h"
#include "textserver_communication_protocol.h"
#include "textserver_thread.h"

using namespace textserver;

Server::Server(QObject * parent)
{
    if (!listen(QHostAddress::Any, DEFAULT_PORT)) {
        qDebug() << "Couldn't start server:" << errorString();
    }
    else {
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

        // Поиск первого нелокального IP
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        qDebug() << "Network address:" << ipAddress << serverPort();
        qDebug() << "Local address:" << QHostAddress(QHostAddress::LocalHost).toString();
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // Создание нового потока для очередного соединения
    ServerThread * new_thread = new ServerThread(socketDescriptor, this);
    connect(new_thread,SIGNAL(finished()),new_thread,SLOT(deleteLater()));
    new_thread->start();
}
