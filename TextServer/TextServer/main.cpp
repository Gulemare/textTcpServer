#include <QCoreApplication>
#include "textserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    textserver::Server server;

    return a.exec();
}
