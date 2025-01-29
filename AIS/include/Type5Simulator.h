#pragma once

#include "ui_Type5Simulator.h"
#include "BaseAISSimulator.h"

namespace Ui
{
class Type5Simulator;
}

class Type5Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type5Simulator(QWidget *parent = nullptr);
    ~Type5Simulator();

public slots:
    virtual void updateAisData(QStringList &aisMess) override;
private:    
    void init();

private:

    struct ShipType {
        int id;
        QString name;
    };
    struct PosType {
        int id;
        QString name;
    };

    QList<ShipType> shipTypes = {
        {0, QObject::tr("Не доступно")},
        {20, QObject::tr("Крыло над водой (WIG), все суда этого типа")},
        {21, QObject::tr("Крыло над водой (WIG), Опасная категория A")},
        {22, QObject::tr("Крыло над водой (WIG), Опасная категория B")},
        {23, QObject::tr("Крыло над водой (WIG), Опасная категория C")},
        {24, QObject::tr("Крыло над водой (WIG), Опасная категория D")},
        {30, QObject::tr("Рыболовное судно")},
        {31, QObject::tr("Буксир")},
        {32, QObject::tr("Буксир: длина превышает 200 м или ширина превышает 25 м")},
        {33, QObject::tr("Землечерпание или подводные работы")},
        {34, QObject::tr("Подводные работы")},
        {35, QObject::tr("Военные операции")},
        {36, QObject::tr("Парусное судно")},
        {37, QObject::tr("Судно для отдыха")},
        {40, QObject::tr("Высокоскоростное судно (HSC), все суда этого типа")},
        {41, QObject::tr("Высокоскоростное судно (HSC), Опасная категория A")},
        {42, QObject::tr("Высокоскоростное судно (HSC), Опасная категория B")},
        {43, QObject::tr("Высокоскоростное судно (HSC), Опасная категория C")},
        {44, QObject::tr("Высокоскоростное судно (HSC), Опасная категория D")},
        {49, QObject::tr("Высокоскоростное судно (HSC), Без дополнительной информации")},
        {50, QObject::tr("Пилотское судно")},
        {51, QObject::tr("Спасательное судно")},
        {52, QObject::tr("Буксир")},
        {53, QObject::tr("Портовой тендер")},
        {54, QObject::tr("Оборудование для предотвращения загрязнения")},
        {55, QObject::tr("Правоприменение")},
        {58, QObject::tr("Медицинская транспортировка")},
        {59, QObject::tr("Небоевые суда согласно Резолюции МАИ № 18")},
        {60, QObject::tr("Пассажирское судно, все суда этого типа")},
        {61, QObject::tr("Пассажирское судно, Опасная категория A")},
        {62, QObject::tr("Пассажирское судно, Опасная категория B")},
        {63, QObject::tr("Пассажирское судно, Опасная категория C")},
        {64, QObject::tr("Пассажирское судно, Опасная категория D")},
        {69, QObject::tr("Пассажирское судно, Без дополнительной информации")},
        {70, QObject::tr("Грузовое судно, все суда этого типа")},
        {71, QObject::tr("Грузовое судно, Опасная категория A")},
        {72, QObject::tr("Грузовое судно, Опасная категория B")},
        {73, QObject::tr("Грузовое судно, Опасная категория C")},
        {74, QObject::tr("Грузовое судно, Опасная категория D")},
        {79, QObject::tr("Грузовое судно, Без дополнительной информации")},
        {80, QObject::tr("Танкер, все суда этого типа")},
        {81, QObject::tr("Танкер, Опасная категория A")},
        {82, QObject::tr("Танкер, Опасная категория B")},
        {83, QObject::tr("Танкер, Опасная категория C")},
        {84, QObject::tr("Танкер, Опасная категория D")},
        {89, QObject::tr("Танкер, Без дополнительной информации")},
        {90, QObject::tr("Другой тип, все суда этого типа")},
        {91, QObject::tr("Другой тип, Опасная категория A")},
        {92, QObject::tr("Другой тип, Опасная категория B")},
        {93, QObject::tr("Другой тип, Опасная категория C")},
        {94, QObject::tr("Другой тип, Опасная категория D")},
        {99, QObject::tr("Другой тип, Без дополнительной информации")}
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

    Ui::Type5Simulator *ui;
};
