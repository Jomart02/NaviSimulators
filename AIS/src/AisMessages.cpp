#include "AisMessages.h"
#include <QObject>
#include <QRegularExpression>

// Раскладка полей - ITU-R M.1371 (см. https://gpsd.gitlab.io/gpsd/AIVDM.html).

namespace AIS_Messages {
namespace {

using V = QVariantMap;
using F = FieldDef;

// ---- описание полей формы ----

F fUInt(const QString &key, const QString &label, qint64 max, qint64 def = 0)
{
    F f;
    f.key = key;
    f.label = label;
    f.kind = F::UInt;
    f.max = max;
    f.def = def;
    return f;
}

F fFlag(const QString &key, const QString &label)
{
    F f = fUInt(key, label, 1);
    f.kind = F::Flag;
    return f;
}

F fCombo(const QString &key, const QString &label, const QList<QPair<int, QString>> &items)
{
    F f = fUInt(key, label, 0);
    f.kind = F::Combo;
    f.items = items;
    return f;
}

F fText(const QString &key, const QString &label, int maxChars)
{
    F f = fUInt(key, label, maxChars);
    f.kind = F::Text;
    return f;
}

F fHex(const QString &key, const QString &label, int maxBytes)
{
    F f = fUInt(key, label, maxBytes);
    f.kind = F::Hex;
    return f;
}

F fLon(const QString &key, const QString &label)
{
    F f = fUInt(key, label, 0);
    f.kind = F::Lon;
    return f;
}

F fLat(const QString &key, const QString &label)
{
    F f = fUInt(key, label, 0);
    f.kind = F::Lat;
    return f;
}

F fMmsi(const QString &key, const QString &label)
{
    return fUInt(key, label, 999999999);
}

QString N(const char *base, int i)
{
    return QString("%1%2").arg(base).arg(i);
}

// ---- чтение значений ----

quint32 U(const V &v, const QString &key)
{
    return v.value(key).toUInt();
}

QByteArray hexBytes(const V &v, const QString &key, int maxBytes)
{
    QString s = v.value(key).toString();
    s.remove(QRegularExpression("[^0-9A-Fa-f]"));
    if (s.size() % 2) s += '0';
    return QByteArray::fromHex(s.toLatin1()).left(maxBytes);
}

AisBits head(int type, unsigned int mmsi)
{
    AisBits w;
    w.u(type, 6).u(0, 2).u(mmsi, 30);
    return w;
}

// Заголовок 6, 8, 25, 26 и т.п. с идентификатором приложения: 10 бит DAC + 6 бит FID
AisBits &appId(AisBits &w, const V &v)
{
    return w.u(U(v, "dac"), 10).u(U(v, "fid"), 6);
}

const QList<QPair<int, QString>> kTxRx = {
    {0, QObject::tr("Tx A/B, Rx A/B")},
    {1, QObject::tr("Tx A, Rx A/B")},
    {2, QObject::tr("Tx B, Rx A/B")},
    {3, QObject::tr("Зарезервировано")},
};

// ---- упаковка ----

// Тип 7 (подтверждение двоичного) и 13 (подтверждение безопасности): до 4 адресатов
AisBits packAck(int type, const V &v, unsigned int mmsi)
{
    AisBits w = head(type, mmsi);
    w.spare(2);
    for (int i = 1; i <= 4; ++i) {
        if (i > 1 && U(v, N("mmsi", i)) == 0) break; // нулевой MMSI - адресат не задан
        w.u(U(v, N("mmsi", i)), 30).u(U(v, N("seq", i)), 2);
    }
    return w;
}

AisBits pack6(const V &v, unsigned int mmsi)
{
    AisBits w = head(6, mmsi);
    w.u(U(v, "seqno"), 2).u(U(v, "dest"), 30).flag(U(v, "retransmit")).spare(1);
    appId(w, v).bytes(hexBytes(v, "data", 115)); // 920 бит
    return w;
}

AisBits pack8(const V &v, unsigned int mmsi)
{
    AisBits w = head(8, mmsi);
    w.spare(2);
    appId(w, v).bytes(hexBytes(v, "data", 119)); // 952 бита
    return w;
}

AisBits pack10(const V &v, unsigned int mmsi)
{
    AisBits w = head(10, mmsi);
    w.spare(2).u(U(v, "dest"), 30).spare(2);
    return w;
}

AisBits pack12(const V &v, unsigned int mmsi)
{
    AisBits w = head(12, mmsi);
    w.u(U(v, "seqno"), 2).u(U(v, "dest"), 30).flag(U(v, "retransmit")).spare(1)
        .textVar(v.value("text").toString(), 156);
    return w;
}

// Тип 15: 88 бит (один запрос), 110 (два запроса одной станции), 160 (две станции)
AisBits pack15(const V &v, unsigned int mmsi)
{
    AisBits w = head(15, mmsi);
    w.spare(2).u(U(v, "mmsi1"), 30).u(U(v, "type1_1"), 6).u(U(v, "offset1_1"), 12);
    const bool second = U(v, "mmsi2") != 0;
    if (second || U(v, "type1_2") != 0)
        w.spare(2).u(U(v, "type1_2"), 6).u(U(v, "offset1_2"), 12).spare(2);
    if (second)
        w.u(U(v, "mmsi2"), 30).u(U(v, "type2_1"), 6).u(U(v, "offset2_1"), 12).spare(2);
    return w;
}

// Тип 16: 96 бит (одна станция + 4 бита) или 144 (две станции)
AisBits pack16(const V &v, unsigned int mmsi)
{
    AisBits w = head(16, mmsi);
    w.spare(2).u(U(v, "mmsiA"), 30).u(U(v, "offsetA"), 12).u(U(v, "incrementA"), 10);
    if (U(v, "mmsiB") != 0)
        w.u(U(v, "mmsiB"), 30).u(U(v, "offsetB"), 12).u(U(v, "incrementB"), 10);
    else
        w.spare(4);
    return w;
}

// Тип 17: координаты в 1/10 минуты, данные до 736 бит
AisBits pack17(const V &v, unsigned int mmsi)
{
    AisBits w = head(17, mmsi);
    w.spare(2).lon(v.value("lon").toDouble(), true).lat(v.value("lat").toDouble(), true).spare(5)
        .bytes(hexBytes(v, "data", 92));
    return w;
}

// Тип 20: от 1 до 4 резервирований слотов, длина 72-160 бит
AisBits pack20(const V &v, unsigned int mmsi)
{
    AisBits w = head(20, mmsi);
    w.spare(2);
    for (int i = 1; i <= 4; ++i) {
        if (i > 1 && U(v, N("slots", i)) == 0) break;
        w.u(U(v, N("offset", i)), 12).u(U(v, N("slots", i)), 4).u(U(v, N("timeout", i)), 3)
            .u(U(v, N("incr", i)), 11);
    }
    return w.padToByte();
}

// Тип 22 - 168 бит; при addressed вместо области два MMSI
AisBits pack22(const V &v, unsigned int mmsi)
{
    AisBits w = head(22, mmsi);
    const bool addressed = U(v, "addressed") != 0;
    w.spare(2).u(U(v, "channelA"), 12).u(U(v, "channelB"), 12).u(U(v, "txrx"), 4).flag(U(v, "power"));
    if (addressed)
        w.u(U(v, "dest1"), 30).spare(5).u(U(v, "dest2"), 30).spare(5);
    else
        w.lon(v.value("neLon").toDouble(), true).lat(v.value("neLat").toDouble(), true)
            .lon(v.value("swLon").toDouble(), true).lat(v.value("swLat").toDouble(), true);
    w.flag(addressed).flag(U(v, "bandA")).flag(U(v, "bandB")).u(U(v, "zone"), 3).spare(23);
    return w;
}

// Тип 23 - 160 бит
AisBits pack23(const V &v, unsigned int mmsi)
{
    AisBits w = head(23, mmsi);
    w.spare(2)
        .lon(v.value("neLon").toDouble(), true).lat(v.value("neLat").toDouble(), true)
        .lon(v.value("swLon").toDouble(), true).lat(v.value("swLat").toDouble(), true)
        .u(U(v, "stationType"), 4).u(U(v, "shipType"), 8).spare(22)
        .u(U(v, "txrx"), 2).u(U(v, "interval"), 4).u(U(v, "quiet"), 4).spare(6);
    return w;
}

// Заголовок 25 и 26: флаги адресации и структурированности, MMSI адресата, идентификатор приложения
AisBits binaryHeader(int type, const V &v, unsigned int mmsi)
{
    AisBits w = head(type, mmsi);
    const bool addressed = U(v, "addressed") != 0;
    const bool structured = U(v, "structured") != 0;
    w.flag(addressed).flag(structured);
    if (addressed) w.u(U(v, "dest"), 30);
    if (structured) appId(w, v);
    return w;
}

// Тип 25: один слот, не более 168 бит
AisBits pack25(const V &v, unsigned int mmsi)
{
    AisBits w = binaryHeader(25, v, mmsi);
    return w.bytes(hexBytes(v, "data", qMax(0, (168 - w.size()) / 8)));
}

// Тип 26: несколько слотов, в конце 20 бит radio status
AisBits pack26(const V &v, unsigned int mmsi)
{
    AisBits w = binaryHeader(26, v, mmsi);
    w.bytes(hexBytes(v, "data", 120)).padToByte();
    return w.spare(20);
}

QList<MessageDef> build()
{
    const auto addressedHeader = [](QList<F> &f) {
        f << fFlag("addressed", QObject::tr("Адресное (иначе широковещательное)"))
          << fFlag("structured", QObject::tr("Структурированное (есть идентификатор приложения)"))
          << fMmsi("dest", QObject::tr("MMSI адресата"))
          << fUInt("dac", QObject::tr("DAC"), 1023)
          << fUInt("fid", QObject::tr("FID"), 63);
    };
    const auto area = [](QList<F> &f) {
        f << fLon("neLon", QObject::tr("СВ долгота")) << fLat("neLat", QObject::tr("СВ широта"))
          << fLon("swLon", QObject::tr("ЮЗ долгота")) << fLat("swLat", QObject::tr("ЮЗ широта"));
    };
    const auto ackFields = [] {
        QList<F> f;
        for (int i = 1; i <= 4; ++i)
            f << fMmsi(N("mmsi", i), QObject::tr("MMSI %1 (0 - нет)").arg(i))
              << fUInt(N("seq", i), QObject::tr("Номер сообщения %1").arg(i), 3);
        return f;
    };

    QList<MessageDef> out;
    auto add = [&out](int type, const QString &title, const QList<F> &fields,
                      std::function<AisBits(const V &, unsigned int)> pack) {
        out.append({type, title, fields, pack});
    };

    add(6, QObject::tr("Двоичное адресное сообщение"),
        {fMmsi("dest", QObject::tr("MMSI адресата")), fUInt("seqno", QObject::tr("Порядковый номер"), 3),
         fFlag("retransmit", QObject::tr("Повторная передача")), fUInt("dac", QObject::tr("DAC"), 1023),
         fUInt("fid", QObject::tr("FID"), 63), fHex("data", QObject::tr("Данные (hex, до 115 байт)"), 115)},
        pack6);

    add(7, QObject::tr("Подтверждение двоичного сообщения"), ackFields(),
        [](const V &v, unsigned int m) { return packAck(7, v, m); });

    add(8, QObject::tr("Двоичное широковещательное сообщение"),
        {fUInt("dac", QObject::tr("DAC"), 1023), fUInt("fid", QObject::tr("FID"), 63),
         fHex("data", QObject::tr("Данные (hex, до 119 байт)"), 119)},
        pack8);

    add(10, QObject::tr("Запрос UTC и даты"), {fMmsi("dest", QObject::tr("MMSI станции"))}, pack10);

    add(12, QObject::tr("Адресное сообщение о безопасности"),
        {fMmsi("dest", QObject::tr("MMSI адресата")), fUInt("seqno", QObject::tr("Порядковый номер"), 3),
         fFlag("retransmit", QObject::tr("Повторная передача")), fText("text", QObject::tr("Текст"), 156)},
        pack12);

    add(13, QObject::tr("Подтверждение сообщения о безопасности"), ackFields(),
        [](const V &v, unsigned int m) { return packAck(13, v, m); });

    add(15, QObject::tr("Опрос"),
        {fMmsi("mmsi1", QObject::tr("Станция 1: MMSI")), fUInt("type1_1", QObject::tr("Станция 1: тип сообщения 1"), 63),
         fUInt("offset1_1", QObject::tr("Станция 1: смещение слота 1"), 4095),
         fUInt("type1_2", QObject::tr("Станция 1: тип сообщения 2 (0 - нет)"), 63),
         fUInt("offset1_2", QObject::tr("Станция 1: смещение слота 2"), 4095),
         fMmsi("mmsi2", QObject::tr("Станция 2: MMSI (0 - нет)")),
         fUInt("type2_1", QObject::tr("Станция 2: тип сообщения"), 63),
         fUInt("offset2_1", QObject::tr("Станция 2: смещение слота"), 4095)},
        pack15);

    add(16, QObject::tr("Команда режима назначения"),
        {fMmsi("mmsiA", QObject::tr("Станция A: MMSI")), fUInt("offsetA", QObject::tr("Станция A: смещение"), 4095),
         fUInt("incrementA", QObject::tr("Станция A: шаг"), 1023),
         fMmsi("mmsiB", QObject::tr("Станция B: MMSI (0 - нет)")),
         fUInt("offsetB", QObject::tr("Станция B: смещение"), 4095),
         fUInt("incrementB", QObject::tr("Станция B: шаг"), 1023)},
        pack16);

    {
        QList<F> f;
        f << fLon("lon", QObject::tr("Долгота")) << fLat("lat", QObject::tr("Широта"))
          << fHex("data", QObject::tr("Поправки DGNSS (hex, до 92 байт)"), 92);
        add(17, QObject::tr("Широковещательные поправки DGNSS"), f, pack17);
    }

    {
        QList<F> f;
        for (int i = 1; i <= 4; ++i)
            f << fUInt(N("offset", i), QObject::tr("Резерв %1: смещение").arg(i), 4095)
              << fUInt(N("slots", i), QObject::tr("Резерв %1: число слотов (0 - нет)").arg(i), 15)
              << fUInt(N("timeout", i), QObject::tr("Резерв %1: тайм-аут").arg(i), 7)
              << fUInt(N("incr", i), QObject::tr("Резерв %1: шаг").arg(i), 2047);
        f[1].def = 1; // по умолчанию одно резервирование
        add(20, QObject::tr("Управление каналом передачи данных"), f, pack20);
    }

    {
        QList<F> f;
        f << fUInt("channelA", QObject::tr("Канал A"), 4095, 2087) << fUInt("channelB", QObject::tr("Канал B"), 4095, 2088)
          << fCombo("txrx", QObject::tr("Режим Tx/Rx"), kTxRx) << fFlag("power", QObject::tr("Высокая мощность"));
        area(f);
        f << fFlag("addressed", QObject::tr("Адресное (вместо области - два MMSI)"))
          << fMmsi("dest1", QObject::tr("MMSI 1")) << fMmsi("dest2", QObject::tr("MMSI 2"))
          << fFlag("bandA", QObject::tr("Канал A: 12.5 кГц")) << fFlag("bandB", QObject::tr("Канал B: 12.5 кГц"))
          << fUInt("zone", QObject::tr("Размер переходной зоны"), 7);
        add(22, QObject::tr("Управление каналами"), f, pack22);
    }

    {
        QList<F> f;
        area(f);
        f << fCombo("stationType", QObject::tr("Тип станций"),
                    {{0, QObject::tr("Все подвижные станции")},
                     {2, QObject::tr("Все станции класса B")},
                     {3, QObject::tr("Воздушные станции SAR")},
                     {4, QObject::tr("Средства навигационного оборудования")},
                     {5, QObject::tr("Судовые станции класса B (IEC 62287)")},
                     {6, QObject::tr("Внутренние водные пути")}})
          << fUInt("shipType", QObject::tr("Тип судна"), 255)
          << fCombo("txrx", QObject::tr("Режим Tx/Rx"), kTxRx)
          << fCombo("interval", QObject::tr("Интервал отчётов"),
                    {{0, QObject::tr("По автономному режиму")},
                     {1, QObject::tr("10 минут")},
                     {2, QObject::tr("6 минут")},
                     {3, QObject::tr("3 минуты")},
                     {4, QObject::tr("1 минута")},
                     {5, QObject::tr("30 секунд")},
                     {6, QObject::tr("15 секунд")},
                     {7, QObject::tr("10 секунд")},
                     {8, QObject::tr("5 секунд")},
                     {9, QObject::tr("Следующий более короткий")},
                     {10, QObject::tr("Следующий более длинный")}})
          << fUInt("quiet", QObject::tr("Время молчания, мин"), 15);
        add(23, QObject::tr("Команда группового назначения"), f, pack23);
    }

    {
        QList<F> f;
        addressedHeader(f);
        f << fHex("data", QObject::tr("Данные (hex, до 16 байт)"), 16);
        add(25, QObject::tr("Двоичное сообщение в одном слоте"), f, pack25);
    }

    {
        QList<F> f;
        addressedHeader(f);
        f << fHex("data", QObject::tr("Данные (hex, до 120 байт)"), 120);
        add(26, QObject::tr("Двоичное сообщение в нескольких слотах"), f, pack26);
    }

    return out;
}

}

const QList<MessageDef> &defs()
{
    static const QList<MessageDef> list = build();
    return list;
}

const MessageDef *find(int type)
{
    for (const MessageDef &d : defs())
        if (d.type == type) return &d;
    return nullptr;
}

QStringList encode(const MessageDef &def, const QVariantMap &values, unsigned int mmsi)
{
    return AIS_NMEA_Builder::frame(def.pack(values, mmsi));
}

}
