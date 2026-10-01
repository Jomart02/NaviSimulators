#include <QtTest>
#include "AisMessages.h"
#include "AisStructures.h"

using namespace AIS_Data_Type;
using namespace AIS_NMEA_Builder;

// pack() защищён: открываем его для проверки полей
template<class D>
struct Open : D {
    using D::pack;
};

// Типовой пример типа 1 из https://gpsd.gitlab.io/gpsd/AIVDM.html
static const QString kSamplePayload = "177KQJ5000G?tO`K>RA1wUbN0TKH";
static const QString kSampleSentence = "!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C\r\n";

static QString payloadOf(const QString &sentence)
{
    return sentence.split(',').at(5);
}

class AisCodecTest : public QObject
{
    Q_OBJECT
private slots:
    void armoringRoundTrip();
    void sentenceMatchesGpsdSample();
    void type1MatchesGpsdSample();
    void sixBitText();
    void signedCoordinates();
    void rateOfTurn();
    void positionReportTypes();
    void type18SpeedIsTenths();
    void type9SpeedIsWholeKnots();
    void type5SplitsIntoTwoSentences();
    void type4And11();
    void type14Text();
    void type24Pair();
    void type24AuxiliaryCraft();
    void type27();
    void type21ExtensionPaddedToByte();
    void everyDescribedMessagePacksWithDefaults();
    void type6Binary();
    void acknowledgements();
    void type8Binary();
    void type10Inquiry();
    void type12AddressedText();
    void type15Interrogation();
    void type16Assignment();
    void type17Dgnss();
    void type20DataLinkManagement();
    void type22ChannelManagement();
    void type23GroupAssignment();
    void type25And26();
    void longBinaryMessageSplits();
};

// Сообщение type из реестра AIS_Messages с заданными значениями полей
static AisBits pack(int type, const QVariantMap &values, unsigned int mmsi = 366123456)
{
    const AIS_Messages::MessageDef *def = AIS_Messages::find(type);
    return def ? def->pack(values, mmsi) : AisBits();
}

void AisCodecTest::armoringRoundTrip()
{
    for (quint32 v = 0; v < 64; ++v) {
        AisBits b;
        b.u(v, 6);
        QCOMPARE(AisBits::fromPayload(b.payload()).getU(0, 6), v);
    }
    QCOMPARE(AisBits().u(39, 6).payload(), QString("W"));
    QCOMPARE(AisBits().u(40, 6).payload(), QString("`")); // граница, на которой раньше получался символ 'X'
    QCOMPARE(AisBits().u(63, 6).payload(), QString("w"));
}

void AisCodecTest::sentenceMatchesGpsdSample()
{
    const QStringList s = frame(AisBits::fromPayload(kSamplePayload), 'B');
    QCOMPARE(s, QStringList{kSampleSentence});
}

void AisCodecTest::type1MatchesGpsdSample()
{
    ClassA123 p;
    p.MMSI = 477553000;
    p.navigation = 5;
    p.lon = -122.34583333333333;
    p.lat = 47.58283333333333;
    p.COG = 51.0;
    p.HDG = 181;
    p.time = 15;
    Type123Decoder dec;
    dec.setParamets(p);
    const AisBits mine = AisBits::fromPayload(payloadOf(dec.getString().at(0)));
    const AisBits sample = AisBits::fromPayload(kSamplePayload);
    // всё, кроме 19 бит radio status, совпадает с реальным сообщением
    for (int pos = 0; pos < 149; ++pos)
        QVERIFY2(mine.getU(pos, 1) == sample.getU(pos, 1), qPrintable(QString("bit %1").arg(pos)));
}

void AisCodecTest::sixBitText()
{
    QCOMPARE(AisBits().text("SHIP 1-A", 8).getText(0, 8), QString("SHIP 1-A"));
    QCOMPARE(AisBits().text("ship 12", 7).getText(0, 7), QString("SHIP 12")); // верхний регистр
    QCOMPARE(AisBits().text("A", 5).size(), 30);                              // добивка '@'
    QCOMPARE(AisBits().text("A~", 2).getText(0, 2), QString("A?"));           // неподдерживаемый символ
    QCOMPARE(AisBits().text("ABCDEFGH", 3).getText(0, 3), QString("ABC"));    // обрезка до длины поля
}

void AisCodecTest::signedCoordinates()
{
    QCOMPARE(AisBits().lon(-122.5).getI(0, 28), -73500000);
    QCOMPARE(AisBits().lat(-33.25).getI(0, 27), -19950000);
    QCOMPARE(AisBits().lon(999).getI(0, 28), 108600000); // вне диапазона -> 181 градус, недоступно
    QCOMPARE(AisBits().lat(999).getI(0, 27), 54600000);  // -> 91 градус
    QCOMPARE(AisBits().lon(-122.5, true).getI(0, 18), -73500);
}

void AisCodecTest::rateOfTurn()
{
    QCOMPARE(AisBits().rot(0).getI(0, 8), 0);
    QCOMPARE(AisBits().rot(708).getI(0, 8), 126);
    QCOMPARE(AisBits().rot(-708).getI(0, 8), -126);
    QCOMPARE(AisBits().rot(100).getI(0, 8), 47); // 4.733 * sqrt(100)
}

void AisCodecTest::positionReportTypes()
{
    for (int type : {1, 2, 3}) {
        ClassA123 p;
        p.messageType = type;
        p.PositionAccuracy = 1;
        p.RAIM = 1;
        Open<Type123Decoder> dec;
        dec.setParamets(p);
        const AisBits b = dec.pack().at(0);
        QCOMPARE(b.size(), 168);
        QCOMPARE(b.getU(0, 6), quint32(type));
        QCOMPARE(b.getU(60, 1), quint32(1));  // точность
        QCOMPARE(b.getU(148, 1), quint32(1)); // RAIM
    }
}

void AisCodecTest::type18SpeedIsTenths()
{
    ClassB18 p;
    p.SOG = 12;
    Open<Type18Decoder> dec;
    dec.setParamets(p);
    const AisBits b = dec.pack().at(0);
    QCOMPARE(b.size(), 168);
    QCOMPARE(b.getU(46, 10), quint32(120));
}

void AisCodecTest::type9SpeedIsWholeKnots()
{
    SAR p;
    p.SOG = 150;
    Open<Type9Decoder> dec;
    dec.setParamets(p);
    const AisBits b = dec.pack().at(0);
    QCOMPARE(b.size(), 168);
    QCOMPARE(b.getU(50, 10), quint32(150));
}

void AisCodecTest::type5SplitsIntoTwoSentences()
{
    ClassA5 p;
    p.MMSI = 123456789;
    p.VesselName = "TEST SHIP 1";
    p.CallSign = "UA1234";
    p.Destination = "ST PETERSBURG";
    Type5Decoder dec;
    dec.setParamets(p);
    const QStringList s = dec.getString();
    QCOMPARE(s.size(), 2);
    for (const QString &line : s)
        QVERIFY2(line.size() <= 82, qPrintable(line));
    const QStringList f1 = s.at(0).split(',');
    const QStringList f2 = s.at(1).split(',');
    QCOMPARE(f1.at(1), QString("2"));
    QCOMPARE(f1.at(2), QString("1"));
    QCOMPARE(f2.at(2), QString("2"));
    QVERIFY(!f1.at(3).isEmpty());   // sequential id
    QCOMPARE(f1.at(3), f2.at(3));
    QVERIFY(f1.at(6).startsWith("0")); // fill bits только у последнего фрагмента
    QVERIFY(f2.at(6).startsWith("2")); // 424 бита = 70 символов + 4 бита

    const AisBits all = AisBits::fromPayload(payloadOf(s.at(0)) + payloadOf(s.at(1)), 2);
    QCOMPARE(all.size(), 424);
    QCOMPARE(all.getU(0, 6), quint32(5));
    QCOMPARE(all.getText(112, 20), QString("TEST SHIP 1"));
    QCOMPARE(all.getText(70, 7), QString("UA1234"));
    QCOMPARE(all.getText(302, 20), QString("ST PETERSBURG"));
}

void AisCodecTest::type4And11()
{
    BaseStation4 p;
    p.MMSI = 3669702;
    p.useSystemTime = false;
    p.utc = QDateTime(QDate(2026, 10, 1), QTime(12, 34, 56), Qt::UTC);
    p.lat = 37.7;
    p.lon = -122.4;
    p.PositionAccuracy = 1;
    p.PositionType = 1;
    p.RAIM = 1;
    Open<Type4Decoder> dec;

    for (bool response : {false, true}) {
        p.response = response;
        dec.setParamets(p);
        const AisBits b = dec.pack().at(0);
        QCOMPARE(b.size(), 168);
        QCOMPARE(b.getU(0, 6), quint32(response ? 11 : 4));
        QCOMPARE(b.getU(8, 30), quint32(3669702));
        QCOMPARE(b.getU(38, 14), quint32(2026));
        QCOMPARE(b.getU(52, 4), quint32(10));
        QCOMPARE(b.getU(56, 5), quint32(1));
        QCOMPARE(b.getU(61, 5), quint32(12));
        QCOMPARE(b.getU(66, 6), quint32(34));
        QCOMPARE(b.getU(72, 6), quint32(56));
        QCOMPARE(b.getU(78, 1), quint32(1));
        QCOMPARE(b.getI(79, 28), qRound(-122.4 * 600000));
        QCOMPARE(b.getI(107, 27), qRound(37.7 * 600000));
        QCOMPARE(b.getU(134, 4), quint32(1));
        QCOMPARE(b.getU(148, 1), quint32(1));
    }
}

void AisCodecTest::type14Text()
{
    Safety14 p;
    p.MMSI = 3669702;
    p.text = "Test message 123";
    Open<Type14Decoder> dec;
    dec.setParamets(p);
    const AisBits b = dec.pack().at(0);
    QCOMPARE(b.size(), 40 + 6 * 16);
    QCOMPARE(b.getU(0, 6), quint32(14));
    QCOMPARE(b.getText(40, 16), QString("TEST MESSAGE 123"));

    p.text = QString(161, 'X'); // максимум: 3 предложения
    Type14Decoder full;
    full.setParamets(p);
    const QStringList s = full.getString();
    QCOMPARE(s.size(), 3);
    for (const QString &line : s)
        QVERIFY2(line.size() <= 82, qPrintable(line));
}

void AisCodecTest::type24Pair()
{
    ClassB24 p;
    p.MMSI = 338123456;
    p.VesselName = "Yacht 7";
    p.ShipType = 37;
    p.VendorId = "ABC";
    p.Model = 5;
    p.Serial = 123456;
    p.CallSign = "WDC1234";
    p.DimensionBow = 10;
    p.DimensionStern = 5;
    p.DimensionPort = 2;
    p.DimensionStarboard = 3;
    Open<Type24Decoder> dec;
    dec.setParamets(p);
    const auto parts = dec.pack();
    QCOMPARE(int(parts.size()), 2);

    const AisBits &a = parts[0];
    QCOMPARE(a.size(), 168);
    QCOMPARE(a.getU(0, 6), quint32(24));
    QCOMPARE(a.getU(38, 2), quint32(0));
    QCOMPARE(a.getText(40, 20), QString("YACHT 7"));

    const AisBits &b = parts[1];
    QCOMPARE(b.size(), 168);
    QCOMPARE(b.getU(8, 30), quint32(338123456));
    QCOMPARE(b.getU(38, 2), quint32(1));
    QCOMPARE(b.getU(40, 8), quint32(37));
    QCOMPARE(b.getText(48, 3), QString("ABC"));
    QCOMPARE(b.getU(66, 4), quint32(5));
    QCOMPARE(b.getU(70, 20), quint32(123456));
    QCOMPARE(b.getText(90, 7), QString("WDC1234"));
    QCOMPARE(b.getU(132, 9), quint32(10));
    QCOMPARE(b.getU(141, 9), quint32(5));
    QCOMPARE(b.getU(150, 6), quint32(2));
    QCOMPARE(b.getU(156, 6), quint32(3));

    Type24Decoder framed;
    framed.setParamets(p);
    QCOMPARE(framed.getString().size(), 2); // по одному предложению на часть
}

void AisCodecTest::type24AuxiliaryCraft()
{
    ClassB24 p;
    p.MMSI = 982345678; // 98XXXYYYY
    p.MothershipMMSI = 338123456;
    p.DimensionBow = 10;
    Open<Type24Decoder> dec;
    dec.setParamets(p);
    const AisBits b = dec.pack().at(1);
    QCOMPARE(b.getU(132, 30), quint32(338123456));
}

void AisCodecTest::type27()
{
    LongRange27 p;
    p.MMSI = 477553000;
    p.PositionAccuracy = 1;
    p.RAIM = 1;
    p.navigation = 5;
    p.lon = -122.34583333333333;
    p.lat = 47.58283333333333;
    p.SOG = 12;
    p.COG = 51;
    Open<Type27Decoder> dec;
    dec.setParamets(p);
    const AisBits b = dec.pack().at(0);
    QCOMPARE(b.size(), 96);
    QCOMPARE(b.getU(0, 6), quint32(27));
    QCOMPARE(b.getU(38, 1), quint32(1));
    QCOMPARE(b.getU(39, 1), quint32(1));
    QCOMPARE(b.getU(40, 4), quint32(5));
    QCOMPARE(b.getI(44, 18), qRound(-122.34583333333333 * 600));
    QCOMPARE(b.getI(62, 17), qRound(47.58283333333333 * 600));
    QCOMPARE(b.getU(79, 6), quint32(12));
    QCOMPARE(b.getU(85, 9), quint32(51));

    p.SOG = 200;
    p.COG = 400;
    dec.setParamets(p);
    const AisBits na = dec.pack().at(0);
    QCOMPARE(na.getU(79, 6), quint32(63));  // недоступно
    QCOMPARE(na.getU(85, 9), quint32(511)); // недоступно
}

void AisCodecTest::type21ExtensionPaddedToByte()
{
    ClassAton21 p;
    p.MMSI = 992345678;
    p.nameAton = "BUOY 1";
    Open<Type21Decoder> dec;
    dec.setParamets(p);
    QCOMPARE(dec.pack().at(0).size(), 272);

    p.extensionAton = "EXTENSION";
    dec.setParamets(p);
    const AisBits b = dec.pack().at(0);
    QCOMPARE(b.size() % 8, 0);
    QCOMPARE(b.getText(272, 9), QString("EXTENSION"));
}

void AisCodecTest::everyDescribedMessagePacksWithDefaults()
{
    QCOMPARE(AIS_Messages::defs().size(), 14);
    for (const auto &def : AIS_Messages::defs()) {
        const AisBits b = def.pack(QVariantMap(), 366123456);
        QVERIFY2(b.size() >= 40, qPrintable(QString("тип %1").arg(def.type))); // без данных 25 - 40 бит
        QCOMPARE(b.getU(0, 6), quint32(def.type));
        QCOMPARE(b.getU(8, 30), quint32(366123456));
        for (const QString &s : AIS_Messages::encode(def, QVariantMap(), 366123456))
            QVERIFY2(s.size() <= 82, qPrintable(s));
    }
}

void AisCodecTest::type6Binary()
{
    const AisBits b = pack(6, {{"dest", 123456789}, {"seqno", 2}, {"retransmit", true}, {"dac", 200}, {"fid", 10},
                               {"data", "DE AD BE EF"}});
    QCOMPARE(b.size(), 120);
    QCOMPARE(b.getU(38, 2), quint32(2));
    QCOMPARE(b.getU(40, 30), quint32(123456789));
    QCOMPARE(b.getU(70, 1), quint32(1));
    QCOMPARE(b.getU(72, 10), quint32(200));
    QCOMPARE(b.getU(82, 6), quint32(10));
    QCOMPARE(b.getU(88, 32), 0xDEADBEEFu);
}

void AisCodecTest::acknowledgements()
{
    for (int type : {7, 13}) {
        const AisBits one = pack(type, {{"mmsi1", 111111111}, {"seq1", 1}});
        QCOMPARE(one.size(), 72);
        QCOMPARE(one.getU(0, 6), quint32(type));

        const AisBits two = pack(type, {{"mmsi1", 111111111}, {"seq1", 1}, {"mmsi2", 222222222}, {"seq2", 2}});
        QCOMPARE(two.size(), 104);
        QCOMPARE(two.getU(40, 30), quint32(111111111));
        QCOMPARE(two.getU(70, 2), quint32(1));
        QCOMPARE(two.getU(72, 30), quint32(222222222));
        QCOMPARE(two.getU(102, 2), quint32(2));

        const AisBits four = pack(type, {{"mmsi1", 1}, {"mmsi2", 2}, {"mmsi3", 3}, {"mmsi4", 4}});
        QCOMPARE(four.size(), 168);
    }
}

void AisCodecTest::type8Binary()
{
    const AisBits b = pack(8, {{"dac", 1}, {"fid", 31}, {"data", "01 02"}});
    QCOMPARE(b.size(), 72);
    QCOMPARE(b.getU(40, 10), quint32(1));
    QCOMPARE(b.getU(50, 6), quint32(31));
    QCOMPARE(b.getU(56, 16), quint32(0x0102));
}

void AisCodecTest::type10Inquiry()
{
    const AisBits b = pack(10, {{"dest", 366999999}});
    QCOMPARE(b.size(), 72);
    QCOMPARE(b.getU(40, 30), quint32(366999999));
}

void AisCodecTest::type12AddressedText()
{
    const AisBits b = pack(12, {{"dest", 123456789}, {"seqno", 1}, {"text", "Hello 123"}});
    QCOMPARE(b.size(), 72 + 6 * 9);
    QCOMPARE(b.getU(38, 2), quint32(1));
    QCOMPARE(b.getU(40, 30), quint32(123456789));
    QCOMPARE(b.getText(72, 9), QString("HELLO 123"));
}

void AisCodecTest::type15Interrogation()
{
    QVariantMap q = {{"mmsi1", 111111111}, {"type1_1", 3}, {"offset1_1", 100}};
    const AisBits one = pack(15, q);
    QCOMPARE(one.size(), 88);
    QCOMPARE(one.getU(40, 30), quint32(111111111));
    QCOMPARE(one.getU(70, 6), quint32(3));
    QCOMPARE(one.getU(76, 12), quint32(100));

    q.insert("type1_2", 5);
    q.insert("offset1_2", 7);
    const AisBits two = pack(15, q);
    QCOMPARE(two.size(), 110);
    QCOMPARE(two.getU(90, 6), quint32(5));
    QCOMPARE(two.getU(96, 12), quint32(7));

    q.insert("mmsi2", 222222222);
    q.insert("type2_1", 9);
    q.insert("offset2_1", 42);
    const AisBits b = pack(15, q);
    QCOMPARE(b.size(), 160);
    QCOMPARE(b.getU(110, 30), quint32(222222222));
    QCOMPARE(b.getU(140, 6), quint32(9));
    QCOMPARE(b.getU(146, 12), quint32(42));
}

void AisCodecTest::type16Assignment()
{
    QVariantMap q = {{"mmsiA", 111111111}, {"offsetA", 300}, {"incrementA", 20}};
    const AisBits one = pack(16, q);
    QCOMPARE(one.size(), 96);
    QCOMPARE(one.getU(40, 30), quint32(111111111));
    QCOMPARE(one.getU(70, 12), quint32(300));
    QCOMPARE(one.getU(82, 10), quint32(20));

    q.insert("mmsiB", 222222222);
    q.insert("offsetB", 5);
    q.insert("incrementB", 6);
    const AisBits two = pack(16, q);
    QCOMPARE(two.size(), 144);
    QCOMPARE(two.getU(92, 30), quint32(222222222));
    QCOMPARE(two.getU(122, 12), quint32(5));
    QCOMPARE(two.getU(134, 10), quint32(6));
}

void AisCodecTest::type17Dgnss()
{
    const AisBits b = pack(17, {{"lon", -122.5}, {"lat", 37.5}, {"data", "AA"}});
    QCOMPARE(b.size(), 88);
    QCOMPARE(b.getI(40, 18), -73500);
    QCOMPARE(b.getI(58, 17), 22500);
    QCOMPARE(b.getU(80, 8), quint32(0xAA));
}

void AisCodecTest::type20DataLinkManagement()
{
    QVariantMap q = {{"offset1", 10}, {"slots1", 2}, {"timeout1", 3}, {"incr1", 100}};
    const AisBits one = pack(20, q);
    QCOMPARE(one.size(), 72); // 70 бит, добито до границы байта
    QCOMPARE(one.getU(40, 12), quint32(10));
    QCOMPARE(one.getU(52, 4), quint32(2));
    QCOMPARE(one.getU(56, 3), quint32(3));
    QCOMPARE(one.getU(59, 11), quint32(100));

    for (int i = 2; i <= 4; ++i)
        q.insert(QString("slots%1").arg(i), 1);
    QCOMPARE(pack(20, q).size(), 160);
}

void AisCodecTest::type22ChannelManagement()
{
    QVariantMap q = {{"channelA", 2087}, {"channelB", 2088}, {"txrx", 1}, {"power", true},
                     {"neLon", -122.0}, {"neLat", 38.0}, {"swLon", -123.0}, {"swLat", 37.0},
                     {"bandA", true}, {"zone", 5}};
    const AisBits b = pack(22, q);
    QCOMPARE(b.size(), 168);
    QCOMPARE(b.getU(40, 12), quint32(2087));
    QCOMPARE(b.getU(52, 12), quint32(2088));
    QCOMPARE(b.getU(64, 4), quint32(1));
    QCOMPARE(b.getU(68, 1), quint32(1));
    QCOMPARE(b.getI(69, 18), -122 * 600);
    QCOMPARE(b.getI(87, 17), 38 * 600);
    QCOMPARE(b.getI(104, 18), -123 * 600);
    QCOMPARE(b.getI(122, 17), 37 * 600);
    QCOMPARE(b.getU(139, 1), quint32(0));
    QCOMPARE(b.getU(140, 1), quint32(1));
    QCOMPARE(b.getU(142, 3), quint32(5));

    q.insert("addressed", true);
    q.insert("dest1", 111111111);
    q.insert("dest2", 222222222);
    const AisBits a = pack(22, q);
    QCOMPARE(a.size(), 168);
    QCOMPARE(a.getU(69, 30), quint32(111111111));
    QCOMPARE(a.getU(104, 30), quint32(222222222));
    QCOMPARE(a.getU(139, 1), quint32(1));
}

void AisCodecTest::type23GroupAssignment()
{
    const AisBits b = pack(23, {{"neLon", -122.0}, {"neLat", 38.0}, {"swLon", -123.0}, {"swLat", 37.0},
                                {"stationType", 4}, {"shipType", 30}, {"txrx", 2}, {"interval", 7}, {"quiet", 3}});
    QCOMPARE(b.size(), 160);
    QCOMPARE(b.getI(40, 18), -122 * 600);
    QCOMPARE(b.getI(58, 17), 38 * 600);
    QCOMPARE(b.getI(75, 18), -123 * 600);
    QCOMPARE(b.getI(93, 17), 37 * 600);
    QCOMPARE(b.getU(110, 4), quint32(4));
    QCOMPARE(b.getU(114, 8), quint32(30));
    QCOMPARE(b.getU(144, 2), quint32(2));
    QCOMPARE(b.getU(146, 4), quint32(7));
    QCOMPARE(b.getU(150, 4), quint32(3));
}

void AisCodecTest::type25And26()
{
    const AisBits addressed = pack(25, {{"addressed", true}, {"structured", true}, {"dest", 123456789},
                                        {"dac", 1}, {"fid", 2}, {"data", "FF"}});
    QCOMPARE(addressed.size(), 94);
    QCOMPARE(addressed.getU(38, 1), quint32(1));
    QCOMPARE(addressed.getU(39, 1), quint32(1));
    QCOMPARE(addressed.getU(40, 30), quint32(123456789));
    QCOMPARE(addressed.getU(70, 10), quint32(1));
    QCOMPARE(addressed.getU(80, 6), quint32(2));
    QCOMPARE(addressed.getU(86, 8), quint32(0xFF));

    const AisBits broadcast = pack(25, {{"data", "AA BB"}});
    QCOMPARE(broadcast.size(), 56);
    QCOMPARE(broadcast.getU(40, 16), quint32(0xAABB));

    // один слот: не более 168 бит, лишние данные отбрасываются
    QCOMPARE(pack(25, {{"data", QString(60, 'A')}}).size(), 168);

    const AisBits multi = pack(26, {{"structured", true}, {"dac", 1}, {"fid", 2}, {"data", "01 02 03"}});
    QCOMPARE(multi.size(), 56 + 24 + 20); // + radio status
    QCOMPARE(multi.getU(40, 10), quint32(1));
    QCOMPARE(multi.getU(50, 6), quint32(2));
    QCOMPARE(multi.getU(56, 24), quint32(0x010203));
}

void AisCodecTest::longBinaryMessageSplits()
{
    const QString hex = QString("AB").repeated(200); // больше максимума, обрежется до 120 байт
    const AIS_Messages::MessageDef *def = AIS_Messages::find(26);
    QVERIFY(def);
    const QStringList s = AIS_Messages::encode(*def, {{"data", hex}}, 366123456);
    QVERIFY(s.size() > 1 && s.size() <= 9);
    for (const QString &line : s)
        QVERIFY2(line.size() <= 82, qPrintable(line));
}

QTEST_MAIN(AisCodecTest)
#include "AisCodecTest.moc"
