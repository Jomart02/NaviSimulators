#include "AisStructures.h"

using namespace AIS_Data_Type;
using namespace AIS_NMEA_Builder;

// Раскладка полей - ITU-R M.1371 (см. https://gpsd.gitlab.io/gpsd/AIVDM.html).
// Статус связи (radio status) не симулируется и передаётся нулями.

// Типы 1, 2, 3 - 168 бит
std::vector<AisBits> Type123Decoder::pack() const
{
    const ClassA123 &p = paramets;
    AisBits w;
    w.u(qBound(1, p.messageType, 3), 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(p.navigation, 4)
        .rot(p.ROT)
        .scaled(p.SOG, 10, 10, 1023)
        .flag(p.PositionAccuracy)
        .lon(p.lon).lat(p.lat)
        .scaled(p.COG, 10, 12, 3600)
        .u(p.HDG, 9)
        .u(p.time, 6)
        .u(p.maneuver, 2)
        .spare(3)
        .flag(p.RAIM)
        .spare(19);
    return {w};
}

// Тип 5 - 424 бита
std::vector<AisBits> Type5Decoder::pack() const
{
    const ClassA5 &p = paramets;
    const bool eta = p.ETA.isValid();
    AisBits w;
    w.u(5, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(0, 2) // версия AIS
        .u(p.IMO, 30)
        .text(p.CallSign, 7)
        .text(p.VesselName, 20)
        .u(p.ShipType, 8)
        .u(p.DimensionBow, 9).u(p.DimensionStern, 9).u(p.DimensionPort, 6).u(p.DimensionStarboard, 6)
        .u(p.PositionType, 4)
        .u(eta ? p.ETA.date().month() : 0, 4)
        .u(eta ? p.ETA.date().day() : 0, 5)
        .u(eta ? p.ETA.time().hour() : 24, 5)
        .u(eta ? p.ETA.time().minute() : 60, 6)
        .u(qMin(qRound(p.Draught * 10), 255), 8)
        .text(p.Destination, 20)
        .flag(p.DTE)
        .spare(1);
    return {w};
}

// Тип 18 - 168 бит
std::vector<AisBits> Type18Decoder::pack() const
{
    const ClassB18 &p = paramets;
    AisBits w;
    w.u(18, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .spare(8)
        .scaled(p.SOG, 10, 10, 1023)
        .flag(p.PositionAccuracy)
        .lon(p.lon).lat(p.lat)
        .scaled(p.COG, 10, 12, 3600)
        .u(p.HDG, 9)
        .u(p.time, 6)
        .spare(2)
        .flag(p.aisType)     // 1 - CS
        .flag(p.displayFlag)
        .flag(p.DSC)
        .flag(p.BandFlag)
        .flag(0)             // сообщение 22
        .flag(p.AssignedMode)
        .flag(p.RAIM)
        .spare(20);
    return {w};
}

// Тип 19 - 312 бит
std::vector<AisBits> Type19Decoder::pack() const
{
    const ClassB19 &p = paramets;
    AisBits w;
    w.u(19, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .spare(8)
        .scaled(p.SOG, 10, 10, 1023)
        .flag(p.PositionAccuracy)
        .lon(p.lon).lat(p.lat)
        .scaled(p.COG, 10, 12, 3600)
        .u(p.HDG, 9)
        .u(p.time, 6)
        .spare(4)
        .text(p.VesselName, 20)
        .u(p.ShipType, 8)
        .u(p.DimensionBow, 9).u(p.DimensionStern, 9).u(p.DimensionPort, 6).u(p.DimensionStarboard, 6)
        .u(p.PositionType, 4)
        .flag(p.RAIM)
        .flag(0)             // DTE
        .flag(0)             // режим назначения
        .spare(4);
    return {w};
}

// Тип 24: часть A (имя) и часть B (тип, оборудование, позывной, размеры) - по 168 бит
std::vector<AisBits> Type24Decoder::pack() const
{
    const ClassB24 &p = paramets;

    AisBits a;
    a.u(24, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(0, 2)
        .text(p.VesselName, 20)
        .spare(8);

    AisBits b;
    b.u(24, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(1, 2)
        .u(p.ShipType, 8)
        .text(p.VendorId, 3).u(p.Model, 4).u(p.Serial, 20)
        .text(p.CallSign, 7);
    if (p.MMSI / 10000000 == 98) // вспомогательное судно: вместо размеров MMSI головного судна
        b.u(p.MothershipMMSI, 30);
    else
        b.u(p.DimensionBow, 9).u(p.DimensionStern, 9).u(p.DimensionPort, 6).u(p.DimensionStarboard, 6);
    b.spare(6);
    return {a, b};
}

// Тип 9 - 168 бит
std::vector<AisBits> Type9Decoder::pack() const
{
    const SAR &p = paramets;
    AisBits w;
    w.u(9, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(p.altitude, 12)
        .scaled(p.SOG, 1, 10, 1023) // у SAR скорость в целых узлах
        .flag(p.PositionAccuracy)
        .lon(p.lon).lat(p.lat)
        .scaled(p.COG, 10, 12, 3600)
        .u(p.time, 6)
        .spare(8)
        .flag(0)             // DTE
        .spare(3)
        .flag(p.Assigned)
        .flag(p.RAIM)
        .spare(20);
    return {w};
}

// Тип 21 - от 272 до 360 бит (расширение имени), добивка до границы байта
std::vector<AisBits> Type21Decoder::pack() const
{
    const ClassAton21 &p = paramets;
    AisBits w;
    w.u(21, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(p.AIDType, 5)
        .text(p.nameAton, 20)
        .flag(p.PositionAccuracy)
        .lon(p.lon).lat(p.lat)
        .u(p.DimensionBow, 9).u(p.DimensionStern, 9).u(p.DimensionPort, 6).u(p.DimensionStarboard, 6)
        .u(p.PositionType, 4)
        .u(p.time, 6)
        .flag(p.offPos)
        .spare(8)
        .flag(p.RAIM)
        .flag(p.virtualAton)
        .flag(p.Assigned)
        .spare(1)
        .textVar(p.extensionAton, 14)
        .padToByte();
    return {w};
}

// Тип 4 / 11 - 168 бит
std::vector<AisBits> Type4Decoder::pack() const
{
    const BaseStation4 &p = paramets;
    const QDateTime t = (p.useSystemTime ? QDateTime::currentDateTimeUtc() : p.utc).toUTC();
    const bool ok = t.isValid();
    AisBits w;
    w.u(p.response ? 11 : 4, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .u(ok ? t.date().year() : 0, 14)
        .u(ok ? t.date().month() : 0, 4)
        .u(ok ? t.date().day() : 0, 5)
        .u(ok ? t.time().hour() : 24, 5)
        .u(ok ? t.time().minute() : 60, 6)
        .u(ok ? t.time().second() : 60, 6)
        .flag(p.PositionAccuracy)
        .lon(p.lon).lat(p.lat)
        .u(p.PositionType, 4)
        .spare(10)
        .flag(p.RAIM)
        .spare(19);
    return {w};
}

// Тип 14 - 40 бит + 6 бит на символ (до 161)
std::vector<AisBits> Type14Decoder::pack() const
{
    const Safety14 &p = paramets;
    AisBits w;
    w.u(14, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .spare(2)
        .textVar(p.text, 161);
    return {w};
}

// Тип 27 - 96 бит
std::vector<AisBits> Type27Decoder::pack() const
{
    const LongRange27 &p = paramets;
    AisBits w;
    w.u(27, 6).u(p.repeat, 2).u(p.MMSI, 30)
        .flag(p.PositionAccuracy)
        .flag(p.RAIM)
        .u(p.navigation, 4)
        .lon(p.lon, true).lat(p.lat, true)
        .u(p.SOG > 62 ? 63 : p.SOG, 6)                                  // 63 - недоступно
        .u(p.COG < 0 || p.COG > 359 ? 511 : p.COG, 9)                   // 511 - недоступно
        .flag(p.GNSSPositionStatus)
        .spare(1);
    return {w};
}
