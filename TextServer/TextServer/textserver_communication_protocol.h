#ifndef TEXTSERVER_COMMUNICATION_PROTOCOL_H
#define TEXTSERVER_COMMUNICATION_PROTOCOL_H
/********************************************************************
Общий протокол формата данных при коммуникации сервера и клиентов:
[Префикс с номером комманды][Пробел][Текст для обработки]

Названия для команд в общей таблице
********************************************************************/
#include <QString>
#include <QHash>

namespace textserver
{
    // TCP порт, используемый сервером
    const quint16 DEFAULT_PORT = 49100;

    // Список кодов комманд
    enum COMMANDS {
        SYMBOL_SORT = 0,
        MIRROR_TURN,
        STRINGS_SORT,
        SYMBOL_STATISTICS
    };

    const QHash<int, QString> COMMANDS_NAMES = {
        {SYMBOL_SORT,       "Symbol sort"},
        {MIRROR_TURN,       "Mirror turn"},
        {STRINGS_SORT,      "Strings sort"},
        {SYMBOL_STATISTICS, "Symbol statistics"}
    };

}
#endif // TEXTSERVER_COMMUNICATION_PROTOCOL_H
