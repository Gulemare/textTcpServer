#include "textclient.h"
#include "ui_textclient.h"
#include "../../TextServer/TextServer/textserver_communication_protocol.h"

TextClient::TextClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextClient),
    tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->textEdit->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->connectionState->setText("Disconnected");
    ui->serverPort->setText(QString::number(textserver::DEFAULT_PORT));
    ui->comboBox->addItems(textserver::COMMANDS_NAMES.values());
    for (auto key : textserver::COMMANDS_NAMES.keys()) {
        comboBoxID_to_commandID.push_back(key);
    }


    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(ui->connectButton,SIGNAL(clicked(bool)),this,SLOT(setConnection()));
    connect(ui->disconnectButton,SIGNAL(clicked(bool)),this,SLOT(stopConnection()));
    connect(ui->sendButton,SIGNAL(clicked(bool)),this,SLOT(send()));

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

}

TextClient::~TextClient()
{
    delete ui;
}

void TextClient::setConnection()
{
    bool addr_valid = false;
    bool port_valid = false;
    quint16 port = ui->serverPort->text().toInt(&port_valid);
    QHostAddress addr;
    addr_valid = addr.setAddress(ui->serverAddress->text());

    if (port_valid)
        ui->serverPort->setStyleSheet("background-color: white");
    else
        ui->serverPort->setStyleSheet("background-color: rgb(245,190,190)");

    if (addr_valid)
        ui->serverAddress->setStyleSheet("background-color: white");
    else
        ui->serverAddress->setStyleSheet("background-color: rgb(245,190,190)");

    static int fails_counter = 0;   // Количество неудачных попыток соединения
    if (port_valid && addr_valid) {
        ui->connectButton->setEnabled(false);
        tcpSocket->connectToHost(addr,port);
        if (!tcpSocket->waitForConnected(1000)) {
            stopConnection();
            fails_counter++;
            QString state("Connection failed");
            if (fails_counter > 1) {
                state += " (";
                state += QString::number(fails_counter);
                state += ")";
            }
            ui->connectionState->setText(state);
            return;
        } else {
            fails_counter = 0;
            ui->serverAddress->setEnabled(false);
            ui->serverPort->setEnabled(false);
            ui->connectButton->setEnabled(false);
            ui->disconnectButton->setEnabled(true);
            ui->textEdit->setEnabled(true);
            ui->sendButton->setEnabled(true);
            ui->comboBox->setEnabled(true);
            ui->connectionState->setText("Connected");
        }
    }

    return;
}

void TextClient::stopConnection()
{
    tcpSocket->abort();
    ui->serverAddress->setEnabled(true);
    ui->serverPort->setEnabled(true);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->textEdit->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->connectionState->setText("Disconnected");
    ui->incomingText->setText("");
}

void TextClient::read()
{
    in.startTransaction();
    in >> text;
    if (!in.commitTransaction())
            return;

    // Конец чтения
    ui->incomingText->setText(text);
}

void TextClient::send()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    // Формирование и отправка сообщения на сервер
    int command = comboBoxID_to_commandID.at(ui->comboBox->currentIndex());
    QString message(QString::number(command));
    message += ' ';
    message += ui->textEdit->toPlainText();
    out << message;
    tcpSocket->write(block);
}

void TextClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::information(this, tr("Client"),tr("The host closed connection"));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Client"),
            tr("The host was not found. Please check the host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Client"),
            tr("The connection was refused by the peer"));
        break;
    default:
        QMessageBox::information(this, tr("Client"),
            tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
        break;
    }
    stopConnection();
}









