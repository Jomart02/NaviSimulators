#pragma once

#include "BaseAISSimulator.h"

namespace Ui
{
class Type21Simulator;
}

class Type21Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type21Simulator(QWidget *parent = nullptr);
    ~Type21Simulator();
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
     virtual void updateAisData(QStringList &aisMess) override;
private:    
    void init();

private:

    struct AidType{
        int id;
        QString name;
    };

    struct PosType {
        int id;
        QString name;
    };


    QList<AidType> aidTypes = {
        {0, QObject::tr("По умолчанию, тип AID не выбран")},
        {1, QObject::tr("Опорная точка")},
        {2, QObject::tr("RACON")},
        {3, QObject::tr("Стационарная морская конструкция (платформы, ветряки)")},
        {4, QObject::tr("Зарезервированно")},
        {5, QObject::tr("Огонь без секторов")},
        {6, QObject::tr("Огонь с секторами")},
        {7, QObject::tr("Передний створный огонь")},
        {8, QObject::tr("Задний створный огонь")},
        {9, QObject::tr("Буй кардинальный (N)")},
        {10, QObject::tr("Буй кардинальный (E)")},
        {11, QObject::tr("Буй кардинальный (S)")},
        {12, QObject::tr("Буй кардинальный (W)")},
        {13, QObject::tr("Beacon, Port hand")},
        {14, QObject::tr("Beacon, Starboard hand")},
        {15, QObject::tr("Beacon, Preferred Channel port hand")},
        {16, QObject::tr("Beacon, Preferred Channel starboard hand")},
        {17, QObject::tr("Beacon, Isolated danger")},
        {18, QObject::tr("Beacon, Safe water")},
        {19, QObject::tr("Beacon, Special mark")},
        {20, QObject::tr("Cardinal Mark N")},
        {21, QObject::tr("Cardinal Mark E")},
        {22, QObject::tr("Cardinal Mark S")},
        {23, QObject::tr("Cardinal Mark W")},
        {24, QObject::tr("Port hand Mark")},
        {25, QObject::tr("Starboard hand Mark")},
        {26, QObject::tr("Preferred Channel Port hand")},
        {27, QObject::tr("Preferred Channel Starboard hand")},
        {28, QObject::tr("Isolated danger")},
        {29, QObject::tr("Safe Water")},
        {30, QObject::tr("Special Mark")},
        {31, QObject::tr("Light Vessel / LANBY / Rigs")}
    };

    QList<PosType> posTypes = {
        {0, QObject::tr("Не доступно")},
        {1, QObject::tr("GPS")},
        {2, QObject::tr("GLONASS")},
        {3, QObject::tr("Combined GPS/GLONASS")},
        {4, QObject::tr("Loran-C")},
        {5, QObject::tr("Chayka")},
        {6, QObject::tr("Integrated navigation system")},
        {7, QObject::tr("Surveyed")},
        {8, QObject::tr("Galileo")},
        {15, QObject::tr("Internal GNSS")}
    };

    Ui::Type21Simulator *ui;
};
