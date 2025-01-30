#pragma once
#include <QString>
#include <QTime>
#include <QBitArray>
#include <bitset>
#include <QRandomGenerator>
namespace AIS_Data_Type {

    typedef struct{
        unsigned int typeMessage;
        unsigned int MMSI;
    }BaseAis;


    struct ClassA123 : BaseAis{
        int navigation =0;         // состояние навигации
        int ROT=0;                // скорость поворота
        int SOG=0;                // скорость относительно земли
        int PositionAccuracy=0;   // точность положения
        double lon=0;             // долгота
        double lat=0;             // широта
        double COG=0;             // курс относительно земли
        unsigned int HDG=0;                // истинное направление от 0 до 359, недоступно-511
        unsigned int time=60;               // отметка времени
        int DTE=0;                // индикатор манёвра
        int RAIM=0;               // индикатор манёвра


        static inline void calculatePos(ClassA123 &data){
            // Генерируем случайные изменения в диапазоне [-5, 5]
            int deltaCOG = QRandomGenerator::global()->bounded(-5, 6);
            int deltaSOG = QRandomGenerator::global()->bounded(-5, 6);
            int deltaHDG = QRandomGenerator::global()->bounded(-5, 6);
            int hdg = data.HDG;
            // Обновляем COG и обеспечиваем, что он остается в диапазоне [0, 359]
            data.COG += deltaCOG;
            if (data.COG < 0) data.COG += 360;
            if (data.COG >= 360) data.COG -= 360;

            // Обновляем HDG и обеспечиваем, что он остается в диапазоне [0, 359]
            hdg += deltaHDG;
            if (hdg < 0) hdg += 360;
            if (hdg >= 360) hdg -= 360;
            data.HDG = hdg;
            // Обновляем SOG и обеспечиваем, что он остается неотрицательным
            data.SOG += deltaSOG;
            if (data.SOG < 0) data.SOG = 0;

            // Преобразование углов в радианы
            double cogRadians = qDegreesToRadians(data.COG);

            // Расстояние, пройденное за время deltaTime (в километрах)
            double distanceTravelled = (data.SOG * 60) / 3600.0 * 1.852; // Узлы -> Км/ч -> Км

            // Рассчитываем новые координаты
            double deltaLat = (distanceTravelled / 111.32) * qSin(cogRadians);
            double deltaLon = (distanceTravelled / (111.32 * qCos(qDegreesToRadians(data.lat)))) * qCos(cogRadians);

            // Обновляем координаты
            data.lat += deltaLat;
            data.lon += deltaLon;

            // Ограничение широты и долготы в допустимых пределах
            if (data.lat > 90.0) data.lat = 90.0;
            if (data.lat < -90.0) data.lat = -90.0;
            if (data.lon > 180.0) data.lon -= 360.0;
            if (data.lon < -180.0) data.lon += 360.0;
        }
    };


    struct ClassA5 : BaseAis{

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
    };

    struct ParamClassA {
        ClassA123 t123;
        ClassA5 t5;
    };

};


// inline  QString BitCode(uint32_t val, int length = 0) {
//     QString res;
//     do {
//         if (val % 2 == 0) {
//             res.prepend('0');
//         } else {
//             res.prepend('1');
//         }
//         val /= 2;
//     } while (val >= 1);

//     // // Если нужно дополнить строку до определенной длины нулями слева
//     // if (length > 0 && res.length() < length) {
//     //     res.prepend(QString(length - res.length(), '0'));
//     // }

//     return res;
// }

// Функция для преобразования числа в бинарную строку фиксированной длины
inline std::string toBinaryString(unsigned int number, int length) {
    std::string binary = std::bitset<32>(number).to_string();
    // Убираем лишние нули слева
    binary = binary.substr(32 - length);
    return binary;
}

// Функция для кодирования значения в биты
inline void encodeValueBytes(std::vector<bool> &bitField, unsigned int value, int start, int end) {
    std::string binary = toBinaryString(value, end - start + 1);
    for (int i = 0; i < binary.length(); ++i) {
        bitField[start + i] = (binary[i] == '1');
    }
}


// // Функция для преобразования числа в бинарную строку фиксированной длины
// inline QString toBinaryString(unsigned int number, int length) {
//     QString binary = QString::number(number, 2); // Преобразуем число в бинарную строку
//     if (binary.length() < length) {
//         // Дополняем строку нулями слева, чтобы получить нужную длину
//         binary.prepend(QString(length - binary.length(), '0'));
//     }
//     return binary;
// }

// // Функция для кодирования значения в биты
// inline void encodeValueBytes(QBitArray &bitField, unsigned int value, int start, int end) {
//     QString binary = toBinaryString(value, end - start + 1);
//     for (int i = 0; i < binary.length(); ++i) {
//         bitField.setBit(start + i, binary[i] == '1');
//     }
// }

// // Функция для кодирования строки в биты
// inline void encodeAsciiBytes(QBitArray &bitField, const std::string &value, int start, int end) {
//     int index = 0;
//     for (size_t i = start; i < end; i += 6) {
//         char byte = value[index++];
//         unsigned int sixBitsValue = (byte >= '@') ? (byte - 64) : byte;
//         QString sixBitsBinary = toBinaryString(sixBitsValue, 6);
//         for (int j = 0; j < 6 && (i + j) < end; ++j) {
//             bitField.setBit(i + j, sixBitsBinary[j] == '1');
//         }
//     }
// }

namespace AIS_NMEA_Builder {
    template<class T>
    class BaseNmeaString {
    public:
        BaseNmeaString();
        ~BaseNmeaString();
        void setParamets(const T &params);
        virtual QStringList getString() = 0;
        
    
    protected:
        void get_checksum(std::string data, char *result);
        T paramets;
    private:
        
    };

    template<class T>
    BaseNmeaString<T>::BaseNmeaString(){
    }

    template<class T>
    BaseNmeaString<T>::~BaseNmeaString(){
    }

    template<class T>
    void BaseNmeaString<T>::setParamets(const T &params){
        paramets = params;
    }

    template<class T>
    void BaseNmeaString<T>::get_checksum(std::string data, char *result){
        unsigned int checksum = 0;
        for(char c: data){
            checksum = checksum ^ (int) c;
        }
        sprintf(result,"%02X",checksum);
    }
    class Type123Decoder : public BaseNmeaString<AIS_Data_Type::ClassA123 >{
        public:
            Type123Decoder();
            ~Type123Decoder();
        virtual QStringList getString() override;
        private:
            QString decodeParam();

    };

};
