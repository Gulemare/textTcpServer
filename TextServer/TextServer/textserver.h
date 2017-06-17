#ifndef TEXTSERVER_H
#define TEXTSERVER_H

#include <QTcpServer>
#include <QNetworkInterface>
#include <QDebug>

namespace textserver {
    class Server : public QTcpServer
    {
    public:
        Server(QObject *parent = 0);

    protected:
          void incomingConnection(qintptr socketDescriptor) override;
    };
}


#endif // TEXTSERVER_H
