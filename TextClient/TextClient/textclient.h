#ifndef TEXTCLIENT_H
#define TEXTCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>
#include <QMessageBox>
#include <QHostAddress>

namespace Ui {
class TextClient;
}

class TextClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextClient(QWidget *parent = 0);
    ~TextClient();

private:
    Ui::TextClient *ui;
    QTcpSocket * tcpSocket;
    QDataStream in;
    QString text;
    QVector<int> comboBoxID_to_commandID; // Связь между элементами комбо-бокса и кодами комманд серверу

private slots:
    void setConnection();
    void stopConnection();
    void read();
    void send();
    void displayError(QAbstractSocket::SocketError socketError);

};

#endif // TEXTCLIENT_H
