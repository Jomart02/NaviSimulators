#include "AisStructures.h"

using namespace AIS_Data_Type;
using namespace AIS_NMEA_Builder;

Type123Decoder::Type123Decoder() : BaseNmeaString(){

}
Type123Decoder::~Type123Decoder(){

}
QString Type123Decoder::decodeParam(){
    std::vector<bool> bitField(168, false);
        // Message ID (6 бит)
    encodeValueBytes(bitField, 1 , 0, 5);

    // Repeat indicator (2 бита)
    encodeValueBytes(bitField, 0, 6, 7);

    // MMSI (30 бит)
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
    // if (lonBits > ((1u << 28) - 1)) {
    //     lonBits = (1u << 28) - 1; // Обработка случая, когда значение выходит за пределы диапазона
    // }
    encodeValueBytes(bitField, lonBits, 61, 88);

    // Latitude (27 бит)
    unsigned int latBits = static_cast<unsigned int>((paramets.lat ) * 600000.0);
    // if (latBits > ((1u << 27) - 1)) {
    //     latBits = (1u << 27) - 1; // Обработка случая, когда значение выходит за пределы диапазона
    // }
    encodeValueBytes(bitField, latBits, 89, 115);
    // COG (12 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.COG*10), 116, 127);

    // True heading (9 бит)
    encodeValueBytes(bitField, paramets.HDG, 128, 136);

    // Time stamp (6 бит)
    encodeValueBytes(bitField, paramets.time, 137, 142);

    // Special maneuver indicator (2 бита)
    encodeValueBytes(bitField, paramets.maneuver, 143, 144);

    // Spare (3 бита)
    for (int i = 145; i < 148; ++i) {
        bitField[i] = false;
    }

    // RAIM flag (1 бит)
    bitField[148] = 0;

    //Communication state (19 бит)
    encodeValueBytes(bitField, 0, 149, 167);

    // Преобразуем битовое поле в строку символов
    return encodeString(bitField,LEN_TYPE123);
}



Type5Decoder::Type5Decoder() : BaseNmeaString(){

}
Type5Decoder::~Type5Decoder(){

}

std::vector<unsigned char> encodeAsciiBytes(const std::string &input) {
    std::vector<unsigned char> result;

    for (char c : input) {
        // Получаем исходное значение байта, вычитая 64
        unsigned char byte = static_cast<unsigned char>(c) - 64;
        
        // Разбиваем байт на 6 битов
        for (int j = 5; j >= 0; --j) {
            result.push_back((byte >> j) & 1);
        }
    }
    
    return result;
}
static void placeBitsInBitField(std::vector<bool>& bitField, const std::vector<unsigned char>& bits, int start, int end) {
    if (start < 0 || end > bitField.size() || start >= end) {
        throw std::out_of_range("Invalid range specified");
    }

    if ((end - start) +1 < static_cast<int>(bits.size())) {
        throw std::length_error("Not enough space in the specified range");
    }

    for (size_t i = 0; i < bits.size(); ++i) {
        bitField[start + i] = bits[i];
    }
}

QString Type5Decoder::decodeParam(){

    std::vector<bool> bitField(LEN_TYPE5, false);
    // Message ID (6 бит)
    encodeValueBytes(bitField, 5 , 0, 5);
    // Repeat indicator (2 бита)
    encodeValueBytes(bitField, 0, 6, 7);
    // MMSI (30 бит)
    encodeValueBytes(bitField, paramets.MMSI, 8, 37);
    // AIS Version (2 бита)
    encodeValueBytes(bitField, 0, 38, 39);
    // IMO Number (30 бит)
    encodeValueBytes(bitField, paramets.IMO, 40, 69);
    // Call Sign (42 бит)
    //encodeAsciiBytes(bitField,paramets.CallSign.toStdString(),70,111,7);
    placeBitsInBitField(bitField,encodeAsciiBytes(paramets.CallSign.toStdString()),70,111);
    // Vessel Name (120 бит)
    //encodeAsciiBytes(bitField,paramets.VesselName.toStdString(),112,231,20);
    placeBitsInBitField(bitField,encodeAsciiBytes(paramets.VesselName.toStdString()),112,231);
    // Ship Type (8 бит)
    encodeValueBytes(bitField, paramets.ShipType, 232, 239);
    // Dimension to Bow(9 бит)
    encodeValueBytes(bitField, paramets.DimensionBow, 240, 248);
    // Dimension to Stern (9 бит)
    encodeValueBytes(bitField, paramets.DimensionStern, 249, 257);
    // Dimension to Port (6 бит)
    encodeValueBytes(bitField, paramets.DimensionPort, 258, 263);
    // Dimension to Starboard (6 бит)
    encodeValueBytes(bitField, paramets.DimensionStarboard, 264, 269);
    // Position Fix Type (4 бит)
    encodeValueBytes(bitField, paramets.PositionType, 270, 273);
    // ETA month (4 бит)
    encodeValueBytes(bitField, paramets.ETA.date().month(), 274, 277);
    // ETA day(5 бит)
    encodeValueBytes(bitField, paramets.ETA.date().day(), 278, 282);
    // ETA hour (5 бит)
    encodeValueBytes(bitField, paramets.ETA.time().hour(), 283, 287);
    // ETA minute (6 бит)
    encodeValueBytes(bitField, paramets.ETA.time().minute(), 288, 293);

    // Draught (8 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.Draught*10), 294, 301);
    // Destination (120 бит)
    placeBitsInBitField(bitField,encodeAsciiBytes(paramets.Destination.toStdString()),302,421);
    
    // DTE (1 бит)
    encodeValueBytes(bitField, paramets.DTE, 422, 422);
    // Spare (1 бит)
    bitField[423] = false;

    return encodeString(bitField,LEN_TYPE5);
}

Type18Decoder::Type18Decoder() : BaseNmeaString(){

}
Type18Decoder::~Type18Decoder(){

}

QString Type18Decoder::decodeParam(){
    std::vector<bool> bitField(LEN_TYPE18, false);
    
    // Message ID (6 бит)
    encodeValueBytes(bitField, 18 , 0, 5);
    
    // Repeat indicator (2 бита)
    encodeValueBytes(bitField, 0 , 6, 7);
    
    // MMSI (30 бит)
    encodeValueBytes(bitField,paramets.MMSI,8,37);
    
    // Regional reserved (8 бит)
    for (int i = 38; i < 46; ++i) {
        bitField[i] = false;
    }
    
    // SOG (10 бит)
    encodeValueBytes(bitField,static_cast<unsigned int>(paramets.SOG + 10), 46, 55);
    
    // Position accuracy (1 бит)
    encodeValueBytes(bitField, paramets.PositionAccuracy, 56, 56);
    
    // Longtitude (28 бит)
    unsigned int lonBits = static_cast<unsigned int>((paramets.lon) * 600000.0);
    encodeValueBytes(bitField, lonBits,57,84);

    // Latitude (27 бит)
    unsigned int latBits = static_cast<unsigned int>((paramets.lat) * 600000.0);
    encodeValueBytes(bitField, latBits,85,111);

    // COG (12 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.COG*10),112,123);

    // True heading (9 бит)
    encodeValueBytes(bitField, paramets.HDG, 124, 132);

    // Time stamp (6 бит)
    encodeValueBytes(bitField, paramets.time, 133, 138);

    // Regional reserved (2 бита)
    for (int i = 139; i < 141; ++i) {
        bitField[i] = false;
    }

    // CS unit (1 бит)
    encodeValueBytes(bitField, paramets.aisType, 141,141);

    // Display flag (1 бит)
    encodeValueBytes(bitField, paramets.displayFlag, 142,142);

    // DSC Flag
    encodeValueBytes(bitField, paramets.DSC, 143, 143);

    // Band flag (1 бит)
    encodeValueBytes(bitField, paramets.BandFlag, 144, 144);

    // Message 22 flag (1 бит)
    encodeValueBytes(bitField, 0, 145,145);

    // Assigned (1 бит)
    encodeValueBytes(bitField, paramets.AssignedMode, 146,146);

    // RAIM (1 бит)
    encodeValueBytes(bitField, paramets.RAIM, 147, 147);

    // Communication state (20 бит)
    encodeValueBytes(bitField, 0, 148, 167);

    // Преобразуем битовое поле в строку символов
    return encodeString(bitField, LEN_TYPE18);
}


Type19Decoder::Type19Decoder() : BaseNmeaString(){

}
Type19Decoder::~Type19Decoder(){

}

QString Type19Decoder::decodeParam(){
    std::vector<bool> bitField(LEN_TYPE19, false);
    
    // Message ID (6 бит)
    encodeValueBytes(bitField, 19 , 0, 5);
    
    // Repeat indicator (2 бита)
    encodeValueBytes(bitField, 0 , 6, 7);
    
    // MMSI (30 бит)
    encodeValueBytes(bitField,paramets.MMSI,8,37);
    
    // Regional reserved (8 бит)
    for (int i = 38; i < 46; ++i) {
        bitField[i] = false;
    }
    
    // SOG (10 бит)
    encodeValueBytes(bitField,static_cast<unsigned int>(paramets.SOG + 10), 46, 55);
    
    // Position accuracy (1 бит)
    
    encodeValueBytes(bitField,paramets.PositionAccuracy, 56, 56);
    
    // Longtitude (28 бит)
    unsigned int lonBits = static_cast<unsigned int>((paramets.lon) * 600000.0);
    encodeValueBytes(bitField, lonBits,57,84);

    // Latitude (27 бит)
    unsigned int latBits = static_cast<unsigned int>((paramets.lat) * 600000.0);
    encodeValueBytes(bitField, latBits,85,111);

    // COG (12 бит)
    encodeValueBytes(bitField, static_cast<unsigned int>(paramets.COG*10),112,123);

    // True heading (9 бит)
    encodeValueBytes(bitField, paramets.HDG, 124, 132);

    // Time stamp (6 бит)
    encodeValueBytes(bitField, paramets.time, 133, 138);

    // Regional reserved (4 бита)
    for (int i = 139; i < 143; ++i) {
        bitField[i] = false;
    }
    
    // Vessel Name (120 бит)
    placeBitsInBitField(bitField, encodeAsciiBytes(paramets.VesselName.toStdString()), 143,262);

    // Ship Type (8 бит)
    encodeValueBytes(bitField, paramets.ShipType, 263, 270);

    // Dimension to Bow (9 бит)
    encodeValueBytes(bitField, paramets.DimensionBow, 271, 279);

    // Dimension to Stern (9 бит)
    encodeValueBytes(bitField, paramets.DimensionStern, 280, 288);

    // Dimension to Port (6 бит)
    encodeValueBytes(bitField, paramets.DimensionPort, 289, 294);

    // Dimension to Starboard (6 бит)
    encodeValueBytes(bitField, paramets.DimensionStarboard, 295, 300);

    // Position Fix Type (4 бита)
    encodeValueBytes(bitField, paramets.PositionType, 301 , 304);

    // RAIM (1 бит)
    
    encodeValueBytes(bitField, paramets.RAIM, 305 , 305);
    // DTE (1 бит)
    bitField[306] = 0;

    // Assigned flag (1 бит)
    encodeValueBytes(bitField, 0, 307, 307);

    // Spare (4 бита)
    for(int i = 308; i < 312; ++i){
        bitField[i] = false;
    }

    // Преобразуем битовое поле в строку символов
    return encodeString(bitField, LEN_TYPE19);

}

Type9Decoder::Type9Decoder() : BaseNmeaString(){

}
Type9Decoder::~Type9Decoder(){

}

QString Type9Decoder::decodeParam(){
    std::vector<bool> bitField(LEN_TYPE9, false);

    // Message Type (6 бит)
    encodeValueBytes(bitField, 9, 0, 5);

    // Repeat indicator (2 бит)
    encodeValueBytes(bitField, 0, 6, 7);

    // MMSI (30 бит)
    encodeValueBytes(bitField, paramets.MMSI, 8, 37);

    // Altitude (12 бит)
    encodeValueBytes(bitField, paramets.altitude, 38, 49);

    // SOG (10 бит)
    encodeValueBytes(bitField, paramets.SOG, 50, 59);

    // Position Accuracy (1 бит)
    encodeValueBytes(bitField, paramets.PositionAccuracy, 60, 60);

    // Longitude (28 бит)
    unsigned int lonBits = static_cast<unsigned int>((paramets.lon) * 600000.0);
    encodeValueBytes(bitField, lonBits,61,88);

    // Latitude (27 бит)
    unsigned int latBits = static_cast<unsigned int>((paramets.lat) * 600000.0);
    encodeValueBytes(bitField, latBits,89,115);

  

    // COG (12 бит)
    encodeValueBytes(bitField, paramets.COG, 116, 127);

    // Time Stamp (6 бит)
    encodeValueBytes(bitField, paramets.time, 128, 133);

    // Regional Reserved (8 бит)
    for(int i = 134; i < 142; ++i){
        bitField[i] = false;
    }

    // DTE (1 бит)
    bitField[142] = 0;

    // Spare (3 бит)
    for(int i = 143; i < 146; ++i){
        bitField[i] = false;
    }

    // Assigned (1 бит)
    encodeValueBytes(bitField, paramets.Assigned, 146, 146);

    // RAIM (1 бит)
    encodeValueBytes(bitField, paramets.RAIM, 147,147);

    // Communication state (19 бит)
    encodeValueBytes(bitField, 0, 148, 167);

    return encodeString(bitField, LEN_TYPE9);
}

Type21Decoder::Type21Decoder() : BaseNmeaString(){

}
Type21Decoder::~Type21Decoder(){

}

QString Type21Decoder::decodeParam(){
    std::vector<bool> bitField(LEN_TYPE21, false);
    
    // Message Type (6 бит)
    encodeValueBytes(bitField, 21, 0, 5);

    // Repeat indicator (2 бит)
    encodeValueBytes(bitField, 0, 6, 7);

    // MMSI (30 бит)
    encodeValueBytes(bitField, paramets.MMSI, 8, 37);

    // AID type (5 бит)
    encodeValueBytes(bitField, paramets.AIDType,38,42);

    // Name (120 бит)
    placeBitsInBitField(bitField,encodeAsciiBytes(paramets.nameAton.toStdString()),43,162);

    // Position Accuracy (1 бит)
    encodeValueBytes(bitField, paramets.PositionAccuracy, 163,163);

    // Longitude (28 бит)
    unsigned int lonBits = static_cast<unsigned int>((paramets.lon) * 600000.0);
    encodeValueBytes(bitField,lonBits,164, 191);

    // Latitude (27 бит)
    unsigned int latBits = static_cast<unsigned int>((paramets.lat) * 600000.0);
    encodeValueBytes(bitField,latBits,192, 218);

    // Dimension to Bow (9 бит)
    encodeValueBytes(bitField, paramets.DimensionBow, 219, 227);

    // Dimension to Stern (9 бит)
    encodeValueBytes(bitField, paramets.DimensionStern, 228, 236);

    // Dimension to Port (6 бит)
    encodeValueBytes(bitField, paramets.DimensionPort, 237, 242);

    // Dimension to Starboard (6 бит)
    encodeValueBytes(bitField, paramets.DimensionStarboard, 243, 248);

    // Type of EPDF (position type) [4 бит]
    encodeValueBytes(bitField,paramets.PositionType, 249,252);

    // Time stamp (6 бит)
    encodeValueBytes(bitField,paramets.time,253,258);

    // Off-position indicator (1 бит)
    encodeValueBytes(bitField,paramets.offPos,259,259);

    // Reserved (8 бит)
    for(int i = 260; i < 268; ++i){
        bitField[i] = false;
    }

    // RAIM (1 бит)
    encodeValueBytes(bitField, paramets.RAIM, 268,268);

    // Virtual-aid flag (1 бит)
    encodeValueBytes(bitField, paramets.virtualAton, 269,269);

    // Assigned (1 бит)
    encodeValueBytes(bitField, paramets.Assigned, 270, 270);

    // Spare (1 бит)
    encodeValueBytes(bitField, 0, 271, 271);

    // Name Extension (88 бит)
    placeBitsInBitField(bitField,encodeAsciiBytes(paramets.extensionAton.toStdString()),272,360);

    return encodeString(bitField, LEN_TYPE21);
}