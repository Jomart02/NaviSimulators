#include "AisStructures.h"

using namespace AIS_Data_Type;
using namespace AIS_NMEA_Builder;

Type123Decoder::Type123Decoder() : BaseNmeaString(){

}
Type123Decoder::~Type123Decoder(){

}
unsigned char calculateChecksum(const QString &message) {
    unsigned char checksum = 0;
    for (int i = 1; i < message.length(); ++i) { // Начинаем с 1, чтобы пропустить '!'
        checksum ^= static_cast<unsigned char>(message.at(i).toLatin1());
    }
    return checksum;
}

QStringList Type123Decoder::getString(){
    QString encodedMessage =decodeParam();
    QStringList nmeaMessages;
    const int maxPayloadLength = 82 - 5; // Максимальная длина полезной нагрузки в символах (учитываем теги и запятые)
    int payloadLength = encodedMessage.length();
    int fragmentCount = (payloadLength + maxPayloadLength - 1) / maxPayloadLength; // Округление вверх

    for (int fragmentNumber = 1; fragmentNumber <= fragmentCount; ++fragmentNumber) {
        int start = (fragmentNumber - 1) * maxPayloadLength;
        int end = qMin(start + maxPayloadLength, payloadLength);
        QString payloadFragment = encodedMessage.mid(start, end - start);

        QString nmeaMessage = "!AIVDM," +
                              QString::number(fragmentCount) + "," +
                              QString::number(fragmentNumber) + "," +
                              (fragmentCount > 1 ? "1" : "") + "," + // Message ID (пустой для одного фрагмента)
                              "B," + // Channel code (B для канала B)
                              payloadFragment + "," +
                              "0"; // Fill bits (предполагаем, что нет дополнительных бит)

        unsigned char checksum = calculateChecksum(nmeaMessage);
        nmeaMessage += "*" + QString::number(checksum, 16).toUpper().rightJustified(2, '0') + "\r\n";;
        nmeaMessages.append(nmeaMessage);
    }

    return nmeaMessages;
} 
QString Type123Decoder::decodeParam(){
    std::vector<bool> bitField(168, false);
        // Message ID (6 бит)
    encodeValueBytes(bitField, 1 , 0, 5);

    // Repeat indicator (2 бита)
    encodeValueBytes(bitField, 0, 6, 7);

    // User ID (30 бит)
    encodeValueBytes(bitField, paramets.MMSI, 8, 37);

    // Navigational status (4 бита)
    encodeValueBytes(bitField, paramets.navigation, 38, 41);

    // Rate of turn (8 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.ROT + 128) & 0xFF, 42, 49);

    // SOG (10 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.SOG * 10), 50, 59);

    // Position accuracy (1 бит)
    bitField[60] = 0;

    // Longitude (28 бит)
    unsigned int lonBits = static_cast<unsigned int>((paramets.lon ) * 600000.0);
    if (lonBits > ((1u << 28) - 1)) {
        lonBits = (1u << 28) - 1; // Обработка случая, когда значение выходит за пределы диапазона
    }
    encodeValueBytes(bitField, lonBits, 61, 88);

    // Latitude (27 бит)
    unsigned int latBits = static_cast<unsigned int>((paramets.lat ) * 600000.0);
    if (latBits > ((1u << 27) - 1)) {
        latBits = (1u << 27) - 1; // Обработка случая, когда значение выходит за пределы диапазона
    }
    encodeValueBytes(bitField, latBits, 89, 115);
    // COG (12 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.COG*10), 116, 127);

    // True heading (9 бит)
    encodeValueBytes(bitField, paramets.HDG, 128, 136);

    // Time stamp (6 бит)
    encodeValueBytes(bitField, paramets.time, 137, 142);

    // Special maneuver indicator (2 бита)
    encodeValueBytes(bitField, paramets.DTE, 143, 144);

    // Spare (3 бита)
    for (int i = 145; i < 148; ++i) {
        bitField[i] = false;
    }

    // RAIM flag (1 бит)
    bitField[148] = 0;

    //Communication state (19 бит)
    encodeValueBytes(bitField, 0, 149, 167);

    // Преобразуем битовое поле в строку символов
        // Преобразуем битовое поле в строку символов
    std::string encodedMessage;
    for (int i = 0; i < 168; i += 6) {
        unsigned int sixBitsValue = 0;
        for (int j = 0; j < 6; ++j) {
            if (i + j < 168 && bitField[i + j]) {
                sixBitsValue |= (1 << (5 - j));
            }
        }
        if (sixBitsValue <= 40) {
            encodedMessage += static_cast<char>(sixBitsValue + 48); // '0'-'9', ':', ';', '<', '=', '>', '?'
        } else if (sixBitsValue > 40 && sixBitsValue <= 63) {
            encodedMessage += static_cast<char>(sixBitsValue + 48 - 8 + ('@' - '0')); // '@', 'A'-'W'
        } else {
            throw std::out_of_range("Six-bit value is out of range");
        }
    }
    return QString::fromStdString( encodedMessage);
}
