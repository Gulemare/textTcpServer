#ifndef TEXTSERVER_THREAD_H
#define TEXTSERVER_THREAD_H

#include <QThread>
#include <QDataStream>
#include <QTcpSocket>
#include <QDebug>
#include <algorithm>

namespace textserver {

    class ServerThread : public QThread
    {
        Q_OBJECT
    public:
        ServerThread(quintptr _descriptor, QObject * parent = 0);
        void run();

    private:
        QTcpSocket * socket;
        quintptr descriptor;
        QDataStream in;
        QString text;

        void processText(int cmd);
        void write();

    signals:
        void error(QTcpSocket::SocketError socketError);

    private slots:
        void read();
        void disconnect();
        void socket_error(QAbstractSocket::SocketError);

    };

}

#endif // TEXTSERVER_THREAD_H
