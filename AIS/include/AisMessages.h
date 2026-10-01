#pragma once
#include <QList>
#include <QPair>
#include <QString>
#include <QVariantMap>
#include <functional>
#include "AisBits.h"

// Описание сообщений, у которых нет собственных структур и виджетов: список полей формы
// и функция упаковки. Значения полей лежат в QVariantMap по ключу поля.
// Служебный ключ "_interval" - период отправки в секундах (0 - только по кнопке).
namespace AIS_Messages {

struct FieldDef {
    enum Kind {
        UInt,   // целое без знака, min..max
        Int,    // целое со знаком, min..max
        Flag,   // флажок
        Combo,  // выбор из items (id, название)
        Text,   // 6-битный текст, max - число символов
        Hex,    // двоичные данные в hex, max - число байт
        Lon,    // долгота, градусы
        Lat     // широта, градусы
    };
    QString key;
    QString label;
    Kind kind = UInt;
    qint64 min = 0;
    qint64 max = 0;
    qint64 def = 0;
    QList<QPair<int, QString>> items;
};

struct MessageDef {
    int type = 0;
    QString title;
    QList<FieldDef> fields;
    // MMSI отправителя и repeat не входят в поля формы
    std::function<AisBits(const QVariantMap &values, unsigned int mmsi)> pack;
};

/// @brief Типы 6, 7, 8, 10, 12, 13, 15, 16, 17, 20, 22, 23, 25, 26
const QList<MessageDef> &defs();
const MessageDef *find(int type);

/// @brief Предложения AIVDM для сообщения type с заданными значениями полей
QStringList encode(const MessageDef &def, const QVariantMap &values, unsigned int mmsi);

}
