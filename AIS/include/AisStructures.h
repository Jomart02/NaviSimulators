#pragma once
#include <QString>
#include <QTime>
#include <QDateTime>
#include <QBitArray>
#include <bitset>
#include <map>
#include <memory>
#include <vector>
#include <QRandomGenerator>
#include <cmath>
#include <QtMath>
#include <QPair>
#include "AisBits.h"

namespace AIS_Data_Type {

    struct BaseAis{
        unsigned int MMSI = 0;
        unsigned int repeat = 0;    // индикатор повтора (0-3, 3 - не повторять)
    };

    inline QPair<double, double> calculateNewPosition(double lat, double lon, double COG, double distance) {
        // Константы
        const double DEG_TO_RAD = M_PI / 180.0; // Преобразование градусов в радианы
        const double EARTH_RADIUS = 6371000.0;  // Радиус Земли в метрах

        // Преобразование входных данных в радианы
        double latRadians = lat * DEG_TO_RAD;
        double lonRadians = lon * DEG_TO_RAD;
        double cogRadians = COG * DEG_TO_RAD;

        // Вычисление углового расстояния
        double angularDistance = distance / EARTH_RADIUS;

        // Вычисление новой широты
        double newLatRadians = asin(sin(latRadians) * cos(angularDistance) +
                                    cos(latRadians) * sin(angularDistance) * cos(cogRadians));

        // Вычисление изменения долготы
        double deltaLon = atan2(sin(cogRadians) * sin(angularDistance) * cos(latRadians),
                                cos(angularDistance) - sin(latRadians) * sin(newLatRadians));

        // Вычисление новой долготы
        double newLonRadians = lonRadians + deltaLon;

        // Нормализация долготы в диапазон [-180, 180]
        if (newLonRadians > M_PI) newLonRadians -= 2 * M_PI;
        if (newLonRadians < -M_PI) newLonRadians += 2 * M_PI;

        // Преобразование обратно в градусы
        double newLat = newLatRadians * (180.0 / M_PI);
        double newLon = newLonRadians * (180.0 / M_PI);

        return qMakePair(newLat, newLon);
    }

    // Один шаг (1 с) случайного блуждания курса и скорости со сдвигом позиции; SOG в узлах
    template<class Cog>
    inline void stepMotion(Cog &cog, int &sog, double &lat, double &lon) {
        cog += QRandomGenerator::global()->bounded(-5, 6);
        if (cog < 0) cog += 360;
        if (cog >= 360) cog -= 360;

        sog = qMax(0, sog + QRandomGenerator::global()->bounded(-5, 6));

        QPair<double, double> newPosition = calculateNewPosition(lat, lon, cog, sog * 0.51444444444);
        lat = newPosition.first;
        lon = newPosition.second;
    }

    inline void stepHeading(unsigned int &hdg) {
        int h = static_cast<int>(hdg) + QRandomGenerator::global()->bounded(-5, 6);
        if (h < 0) h += 360;
        if (h >= 360) h -= 360;
        hdg = h;
    }

    // Типы 1, 2, 3: отчёт о позиции класса A
    struct ClassA123 : BaseAis{
        int messageType = 1;      // 1 - плановый, 2 - по назначенному расписанию, 3 - ответ на опрос
        int navigation = 0;       // состояние навигации
        int ROT = 0;              // скорость поворота, градусы/мин
        int SOG = 0;              // скорость относительно земли
        int PositionAccuracy = 0; // точность положения (1 - лучше 10 м)
        double lon = 0;           // долгота
        double lat = 0;           // широта
        double COG = 0;           // курс относительно земли
        unsigned int HDG = 0;     // истинное направление от 0 до 359, недоступно-511
        unsigned int time = 60;   // отметка времени
        int maneuver = 0;         // индикатор манёвра
        int RAIM = 0;             // флаг RAIM

        static inline void calculatePos(ClassA123 &data){
            stepMotion(data.COG, data.SOG, data.lat, data.lon);
            stepHeading(data.HDG);
        }
    };

    // Тип 5: статические данные и данные о рейсе класса A
    struct ClassA5 : BaseAis{
        unsigned int IMO = 0;               // номер IMO
        QString CallSign;                   // позывной
        QString VesselName;                 // наименование судна
        unsigned int ShipType = 0;          // тип судна
        unsigned int DimensionBow = 0;      // размерности -до носа
        unsigned int DimensionStern = 0;    //             -до кормы
        unsigned int DimensionPort = 0;     //             -до левого борта
        unsigned int DimensionStarboard = 0;//             -до правого борта
        int PositionType = 0;               // тип системы позиционирования
        QDateTime ETA;                      // дата прибытия
        double Draught = 0;                 // осадка
        QString Destination;                // место назначения 20- шестибитных символов
        unsigned int DTE = 0;
    };

    // Тип 18: отчёт о позиции класса B CS
    struct ClassB18 : BaseAis {
        int COG = 0;                // курс относительно земли
        int SOG = 0;                // скорость относительно земли
        unsigned int HDG = 0;       // истинный курс
        double lon = 0;             // долгота
        double lat = 0;             // широта
        unsigned int time = 60;     // отметка времени
        int PositionAccuracy = 0;   // точность положения
        int RAIM = 0;               // флаг RAIM
        int AssignedMode = 0;       // режим работа (автономный/назначенный)
        int BandFlag = 0;           // диапазон частот
        int DSC = 0;                //
        int displayFlag = 0;        //
        int aisType = 0;            // 0 - SOTDMA, 1 - CS

        static inline void calculatePos(ClassB18& data) {
            stepMotion(data.COG, data.SOG, data.lat, data.lon);
            stepHeading(data.HDG);
        }
    };

    // Тип 19: расширенный отчёт о позиции класса B CS
    struct ClassB19 : BaseAis {
        int COG = 0;                // курс относительно земли
        int SOG = 0;                // скорость относительно земли
        unsigned int HDG = 0;       // истинный курс
        double lon = 0;             // долгота
        double lat = 0;             // широта
        QString VesselName;         // наименование судна
        unsigned int ShipType = 0;  // тип судна
        unsigned int DimensionBow = 0;
        unsigned int DimensionStern = 0;
        unsigned int DimensionPort = 0;
        unsigned int DimensionStarboard = 0;
        int PositionType = 0;       // тип системы позиционирования
        int time = 60;
        int RAIM = 0;               // флаг RAIM
        int PositionAccuracy = 0;   // точность положения

        static inline void calculatePos(ClassB19& data) {
            stepMotion(data.COG, data.SOG, data.lat, data.lon);
            stepHeading(data.HDG);
        }
    };

    // Тип 24: статические данные класса B (часть A - имя, часть B - тип, позывной, размеры)
    struct ClassB24 : BaseAis {
        QString VesselName;
        unsigned int ShipType = 0;
        QString VendorId;           // 3 символа
        unsigned int Model = 0;     // 4 бита
        unsigned int Serial = 0;    // 20 бит
        QString CallSign;
        unsigned int DimensionBow = 0;
        unsigned int DimensionStern = 0;
        unsigned int DimensionPort = 0;
        unsigned int DimensionStarboard = 0;
        unsigned int MothershipMMSI = 0; // только для вспомогательных судов (MMSI 98XXXYYYY)
        int intervalSec = 6;        // период отправки, 0 - не отправлять
    };

    // Тип 9: отчёт о позиции SAR-самолёта
    struct SAR : BaseAis {
        int COG = 0;                // курс относительно земли
        int SOG = 0;                // скорость относительно земли, узлы
        double lon = 0;             // долгота
        double lat = 0;             // широта
        int altitude = 0;           // высота, м
        int Assigned = 0;           //
        int time = 60;
        int RAIM = 0;               // флаг RAIM
        int PositionAccuracy = 0;   // точность положения

        static inline void calculatePos(SAR& data) {
            stepMotion(data.COG, data.SOG, data.lat, data.lon);
        }
    };

    // Тип 21: отчёт о средстве навигационного оборудования
    struct ClassAton21 : BaseAis {
        double lon = 0;             // долгота
        double lat = 0;             // широта
        QString nameAton;           // наименование
        unsigned int DimensionBow = 0;
        unsigned int DimensionStern = 0;
        unsigned int DimensionPort = 0;
        unsigned int DimensionStarboard = 0;
        int PositionType = 0;       // тип системы позиционирования
        int AIDType = 0;
        int virtualAton = 0;
        int offPos = 0;
        int time = 60;
        int RAIM = 0;               // флаг RAIM
        int PositionAccuracy = 0;   // точность положения
        int Assigned = 0;
        QString extensionAton;      // расширение наименования (до 14 символов)
    };

    // Типы 4 и 11: отчёт базовой станции / ответ на запрос UTC и даты (тот же формат)
    struct BaseStation4 : BaseAis {
        bool response = false;      // true - формировать тип 11
        bool useSystemTime = true;  // UTC берётся из системных часов в момент отправки
        QDateTime utc;              // UTC, если useSystemTime == false
        int PositionAccuracy = 0;
        double lon = 0;
        double lat = 0;
        int PositionType = 0;       // тип системы позиционирования (EPFD)
        int RAIM = 0;
        int intervalSec = 10;       // период отправки, 0 - только по запросу
    };

    // Тип 14: широковещательное сообщение о безопасности
    struct Safety14 : BaseAis {
        QString text;               // до 161 символа 6-битного ASCII
        int intervalSec = 0;        // период отправки, 0 - только по кнопке
    };

    // Тип 27: сообщение дальнего действия (поля позиции копируются из типа 1-3)
    struct LongRange27 : BaseAis {
        int PositionAccuracy = 0;
        int RAIM = 0;
        int navigation = 0;         // состояние навигации
        double lon = 0;
        double lat = 0;
        int SOG = 0;                // узлы
        int COG = 0;                // градусы
        int GNSSPositionStatus = 0; // 0 - текущая GNSS позиция, 1 - не GNSS
        int intervalSec = 180;      // период отправки, 0 - не отправлять
    };

    struct BaseParamClassAis{
        virtual ~BaseParamClassAis() = default; // Виртуальный деструктор
        virtual void setMMSI(unsigned int mmsi) = 0; // Установка MMSI

        void setEnabled(bool flag){
            enabled = flag;
        }
        bool getEnabled(){
            return enabled;
        }

        private:
            bool enabled = true;
    };

    // elapsedXX - секунд с последней отправки сообщения XX
    struct ParamClassA : public BaseParamClassAis {
        ClassA123 t123;
        ClassA5 t5;
        LongRange27 t27;
        int elapsed27 = 0;

        void setMMSI(unsigned int mmsi) override {
            t123.MMSI = mmsi;
            t5.MMSI = mmsi;
            t27.MMSI = mmsi;
        }

    };

    struct ParamClassB : public BaseParamClassAis {
        ClassB18 t18;
        ClassB19 t19;
        ClassB24 t24;
        int elapsed24 = 0;

        void setMMSI(unsigned int mmsi) override {
            t18.MMSI = mmsi;
            t19.MMSI = mmsi;
            t24.MMSI = mmsi;
        }

    };

    struct ParamSAR : public BaseParamClassAis {
        SAR t9;
        void setMMSI(unsigned int mmsi) override {
            t9.MMSI = mmsi;
        }
    };

    struct ParamATON : public BaseParamClassAis {
        ClassAton21 t21;

        void setMMSI(unsigned int mmsi) override {
            t21.MMSI = mmsi;
        }

    };

    struct ParamBase : public BaseParamClassAis {
        BaseStation4 t4;
        BaseStation4 t11;
        Safety14 t14;
        int elapsed4 = 0;
        int elapsed11 = 0;
        int elapsed14 = 0;

        ParamBase() { t11.response = true; t11.intervalSec = 0; }

        void setMMSI(unsigned int mmsi) override {
            t4.MMSI = mmsi;
            t11.MMSI = mmsi;
            t14.MMSI = mmsi;
        }
    };

};

namespace AIS_NMEA_Builder {
    // Кодировщик параметров в предложения AIVDM; pack() возвращает одно или несколько (тип 24: часть A и B) сообщений
    template<class T>
    class BaseNmeaString {
    public:
        virtual ~BaseNmeaString() = default;
        void setParamets(const T &params) { paramets = params; }
        void setChannel(QChar ch) { channel = ch; }
        QStringList getString() const {
            QStringList nmeaMessages;
            for (const AisBits &message : pack())
                nmeaMessages += frame(message, channel);
            return nmeaMessages;
        }

    protected:
        virtual std::vector<AisBits> pack() const = 0;
        T paramets;
        QChar channel = 'B';
    };

    class Type123Decoder : public BaseNmeaString<AIS_Data_Type::ClassA123> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type5Decoder : public BaseNmeaString<AIS_Data_Type::ClassA5> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type18Decoder : public BaseNmeaString<AIS_Data_Type::ClassB18> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type19Decoder : public BaseNmeaString<AIS_Data_Type::ClassB19> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type24Decoder : public BaseNmeaString<AIS_Data_Type::ClassB24> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type9Decoder : public BaseNmeaString<AIS_Data_Type::SAR> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type21Decoder : public BaseNmeaString<AIS_Data_Type::ClassAton21> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    // Тип 4, либо тип 11 при BaseStation4::response
    class Type4Decoder : public BaseNmeaString<AIS_Data_Type::BaseStation4> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type14Decoder : public BaseNmeaString<AIS_Data_Type::Safety14> {
    protected:
        std::vector<AisBits> pack() const override;
    };

    class Type27Decoder : public BaseNmeaString<AIS_Data_Type::LongRange27> {
    protected:
        std::vector<AisBits> pack() const override;
    };

};
