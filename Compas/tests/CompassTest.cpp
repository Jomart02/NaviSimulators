#include <QtTest>
#include <QRadioButton>
#include "Compass.h"

class CompassTest : public QObject
{
    Q_OBJECT
private slots:
    void gyroSendsValidNmea();
};

// Контрольная сумма NMEA: XOR всех символов между '$' и '*'
static bool checksumOk(const QString &s)
{
    const int star = s.indexOf('*');
    if (!s.startsWith('$') || star < 0) return false;
    int x = 0;
    for (int i = 1; i < star; ++i) x ^= s[i].toLatin1();
    return s.mid(star + 1, 2).toInt(nullptr, 16) == x;
}

void CompassTest::gyroSendsValidNmea()
{
    Compass c;
    c.findChild<QRadioButton *>("radioButton_Gyro")->setChecked(true);
    QSignalSpy spy(&c, &BaseNaviWidget::sendData);
    c.startSend();
    QVERIFY(spy.wait(2500));

    const QStringList lines = spy.takeFirst().at(0).toStringList();
    QStringList heads;
    for (const QString &l : lines) {
        QVERIFY2(checksumOk(l), qPrintable(l));
        heads << l.mid(1, 5);
    }
    QCOMPARE(heads, (QStringList{"HEVHW", "HEHDT", "HETHS"}));
}

QTEST_MAIN(CompassTest)
#include "CompassTest.moc"
