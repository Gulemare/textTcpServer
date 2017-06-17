#include <QApplication>
#include "textclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextClient w;
    w.show();
    return a.exec();
}
