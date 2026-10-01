#include <QtTest>
#include <QPushButton>
#include "SimulatorAIS.h"
#include "AisBits.h"

// Проверка плагина целиком: добавляем цели на все вкладки и смотрим, какие типы сообщений уходят
class AisPluginTest : public QObject
{
    Q_OBJECT
private slots:
    void sendsAllImplementedTypes();
};

static bool checksumOk(const QString &s)
{
    const int star = s.indexOf('*');
    if (!s.startsWith('!') || star < 0) return false;
    int x = 0;
    for (int i = 1; i < star; ++i) x ^= s[i].toLatin1();
    return s.mid(star + 1, 2).toInt(nullptr, 16) == x;
}

// Тип сообщения по первому фрагменту; остальные проверки формата - здесь же
static void collectTypes(const QStringList &sentences, QSet<int> &types)
{
    for (const QString &s : sentences) {
        QVERIFY2(checksumOk(s), qPrintable(s));
        QVERIFY2(s.size() <= 82, qPrintable(s));
        const QStringList f = s.split(',');
        if (f.at(2) == "1") types.insert(AisBits::fromPayload(f.at(5).left(1)).getU(0, 6));
    }
}

void AisPluginTest::sendsAllImplementedTypes()
{
    SimulatorAIS w;
    int added = 0;
    for (QPushButton *b : w.findChildren<QPushButton *>())
        if (b->text() == "Добавить") {
            b->click();
            ++added;
        }
    QCOMPARE(added, 5); // Class A, Class B, SAR, ATON, Base station

    // периодические сообщения: 181 секунда покрывает самый редкий тип 27 (раз в 180 с)
    QSet<int> types;
    for (int tick = 0; tick < 181; ++tick) {
        QStringList out;
        QVERIFY(QMetaObject::invokeMethod(&w, "getNavigationData", Q_RETURN_ARG(QStringList, out)));
        collectTypes(out, types);
    }
    for (int t : {1, 4, 5, 9, 18, 19, 21, 24, 27})
        QVERIFY2(types.contains(t), qPrintable(QString("нет сообщений типа %1").arg(t)));

    // сообщения по кнопке уходят сразу
    QSignalSpy spy(&w, &BaseNaviWidget::sendData);
    int pressed = 0;
    for (QPushButton *b : w.findChildren<QPushButton *>())
        if (b->text() == "Отправить сейчас") {
            b->click();
            ++pressed;
        }
    QCOMPARE(pressed, 3); // типы 4, 11, 14
    QSet<int> onDemand;
    for (const QList<QVariant> &args : spy)
        collectTypes(args.at(0).toStringList(), onDemand);
    QCOMPARE(onDemand, (QSet<int>{4, 11, 14}));
}

QTEST_MAIN(AisPluginTest)
#include "AisPluginTest.moc"
