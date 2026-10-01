#pragma once
#include <QString>
#include <QStringList>
#include <vector>

/// @brief Битовое поле AIS-сообщения: запись полей подряд (MSB first), упаковка в 6-битный payload AIVDM
/// и обратный разбор (для тестов и обработки входящих запросов).
class AisBits
{
public:
    int size() const { return static_cast<int>(m_bits.size()); }

    /// @brief Беззнаковое поле шириной bits (лишние старшие биты отбрасываются)
    AisBits &u(quint32 value, int bits);
    /// @brief Знаковое поле в дополнительном коде
    AisBits &i(qint32 value, int bits);
    AisBits &flag(bool value) { return u(value ? 1 : 0, 1); }
    AisBits &spare(int bits) { return u(0, bits); }
    /// @brief Значение value * mult в unsigned поле; отрицательное -> notAvailable, большое -> notAvailable - 1
    AisBits &scaled(double value, double mult, int bits, quint32 notAvailable);
    /// @brief Rate of turn: градусы/мин -> ROT_AIS = 4.733 * sqrt(deg/min) со знаком, 8 бит
    AisBits &rot(double degPerMin);
    /// @brief Долгота: 28 бит в 1/10000 мин (longRange: 18 бит в 1/10 мин); вне диапазона -> 181 (недоступно)
    AisBits &lon(double deg, bool longRange = false);
    /// @brief Широта: 27 бит в 1/10000 мин (longRange: 17 бит в 1/10 мин); вне диапазона -> 91 (недоступно)
    AisBits &lat(double deg, bool longRange = false);
    /// @brief 6-битный текст фиксированной длины: верхний регистр, не поддерживаемые символы -> '?', добивка '@'
    AisBits &text(const QString &s, int chars);
    /// @brief 6-битный текст переменной длины (без добивки), не более maxChars символов
    AisBits &textVar(const QString &s, int maxChars);
    /// @brief Добить нулями до границы байта (требование ITU-R M.1371 к длине сообщения)
    AisBits &padToByte();

    /// @brief 6-битная "броня" в символы payload
    QString payload() const;
    /// @brief Сколько бит добавлено в последний символ payload
    int fillBits() const;

    static AisBits fromPayload(const QString &payload, int fillBits = 0);
    quint32 getU(int pos, int bits) const;
    qint32 getI(int pos, int bits) const;
    QString getText(int pos, int chars) const;

private:
    AisBits &coord(double deg, int bits, double perDegree, double maxAbsDeg, double naDeg);
    std::vector<bool> m_bits;
};

namespace AIS_NMEA_Builder {
/// @brief Разбивает сообщение на предложения AIVDM/AIVDO (не более 82 символов с CRLF), считает fill bits и checksum.
/// Для многофрагментных сообщений sequential id идёт по кругу 0-9.
QStringList frame(const AisBits &bits, QChar channel = 'B', const QString &talker = "AIVDM");
}
