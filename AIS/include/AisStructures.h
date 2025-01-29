#pragma once
#include <QString>
#include <QTime>


namespace AIS_Data_Type {
    typedef struct{
        int navigation;         // состояние навигации
        int ROT;                // скорость поворота
        int SOG;                // скорость относительно земли
        int PositionAccuracy;   // точность положения
        double lon;             // долгота
        double lat;             // широта
        double COG;                // курс относительно земли
        int HDG;                // истинное направление от 0 до 359, недоступно-511
        int time;             // отметка времени
        int DTE;                // индикатор манёвра
        int RAIM;                // индикатор манёвра
    }ClassA123;


    typedef struct{

        QString    IMO;                // номер IMO
        QString    CallSign;           // позывной
        QString    VesselName;         // наименование судна
        int        ShipType;           // тип судна
        int       DimensionBow;       // размерности -до носа
        int    DimensionStern;     //             -до кормы
        int    DimensionPort;      //             -до левого борта
        int    DimensionStarboard; //             -до правого борта
        int    PositionType;       // тип системы позиционирования
        // char    ETAmonth;           // дата прибытия  -месяц
        // char    ETAday;             //                -день
        // char    ETAhour;            //                -час
        // char    ETAminute;          //                -минута
        QTime   ETA;
        int    Draught;            // осадка
        QString    Destination;        // место назначения 20- шестибитных символов
        int    DTE;
    }werkBloc5;

};