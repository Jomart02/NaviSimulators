#include "AisBits.h"
#include <QtMath>

namespace {
// 6-битный ASCII: коды 64..95 -> 0..31, коды 32..63 -> 32..63
quint32 sixBit(QChar c)
{
    const ushort code = c.toUpper().unicode();
    if (code >= 64 && code <= 95) return code - 64;
    if (code >= 32 && code <= 63) return code;
    return '?';
}

QChar fromSixBit(quint32 v)
{
    return QChar(v < 32 ? v + 64 : v);
}
}

AisBits &AisBits::u(quint32 value, int bits)
{
    for (int b = bits - 1; b >= 0; --b)
        m_bits.push_back((value >> b) & 1u);
    return *this;
}

AisBits &AisBits::i(qint32 value, int bits)
{
    return u(static_cast<quint32>(value), bits); // дополнительный код, лишние старшие биты отбрасываются в u()
}

AisBits &AisBits::scaled(double value, double mult, int bits, quint32 notAvailable)
{
    if (qIsNaN(value) || value < 0) return u(notAvailable, bits);
    return u(static_cast<quint32>(qMin<qint64>(qRound64(value * mult), notAvailable - 1)), bits);
}

AisBits &AisBits::rot(double degPerMin)
{
    const double v = qMin(4.733 * qSqrt(qAbs(degPerMin)), 126.0);
    return i(degPerMin < 0 ? -qRound(v) : qRound(v), 8);
}

AisBits &AisBits::coord(double deg, int bits, double perDegree, double maxAbsDeg, double naDeg)
{
    if (qIsNaN(deg) || qAbs(deg) > maxAbsDeg) deg = naDeg;
    return i(static_cast<qint32>(qRound64(deg * perDegree)), bits);
}

AisBits &AisBits::lon(double deg, bool longRange)
{
    return longRange ? coord(deg, 18, 600.0, 180, 181) : coord(deg, 28, 600000.0, 180, 181);
}

AisBits &AisBits::lat(double deg, bool longRange)
{
    return longRange ? coord(deg, 17, 600.0, 90, 91) : coord(deg, 27, 600000.0, 90, 91);
}

AisBits &AisBits::text(const QString &s, int chars)
{
    for (int k = 0; k < chars; ++k)
        u(k < s.size() ? sixBit(s[k]) : 0, 6);
    return *this;
}

AisBits &AisBits::textVar(const QString &s, int maxChars)
{
    return text(s.left(maxChars), qMin<int>(s.size(), maxChars));
}

AisBits &AisBits::padToByte()
{
    return spare((8 - size() % 8) % 8);
}

int AisBits::fillBits() const
{
    return (6 - size() % 6) % 6;
}

QString AisBits::payload() const
{
    QString out;
    for (int pos = 0; pos < size(); pos += 6) {
        quint32 v = 0;
        for (int k = 0; k < 6; ++k)
            v = (v << 1) | ((pos + k < size() && m_bits[pos + k]) ? 1u : 0u); // хвост добиваем нулями (fill bits)
        out += QChar(v < 40 ? '0' + v : '0' + v + 8);
    }
    return out;
}

AisBits AisBits::fromPayload(const QString &payload, int fillBits)
{
    AisBits r;
    for (QChar c : payload) {
        quint32 v = c.unicode() - '0';
        if (v > 40) v -= 8;
        r.u(v, 6);
    }
    r.m_bits.resize(r.m_bits.size() - fillBits);
    return r;
}

quint32 AisBits::getU(int pos, int bits) const
{
    quint32 v = 0;
    for (int k = 0; k < bits; ++k)
        v = (v << 1) | ((pos + k < size() && m_bits[pos + k]) ? 1u : 0u);
    return v;
}

qint32 AisBits::getI(int pos, int bits) const
{
    quint32 v = getU(pos, bits);
    if (bits < 32 && (v >> (bits - 1)) & 1u) v |= ~0u << bits;
    return static_cast<qint32>(v);
}

QString AisBits::getText(int pos, int chars) const
{
    QString s;
    for (int k = 0; k < chars; ++k)
        s += fromSixBit(getU(pos + 6 * k, 6));
    while (s.endsWith('@')) s.chop(1);
    return s.trimmed();
}

namespace AIS_NMEA_Builder {

QStringList frame(const AisBits &bits, QChar channel, const QString &talker)
{
    // "!AIVDM,2,1,3,B," (15) + payload + ",0*XX\r\n" (7) <= 82 -> payload <= 60
    constexpr int maxPayload = 60;
    static int sequenceId = 0;

    const QString payload = bits.payload();
    const int count = qMax(1, (payload.size() + maxPayload - 1) / maxPayload);
    const QString seq = count > 1 ? QString::number(sequenceId++ % 10) : QString();

    QStringList out;
    for (int n = 1; n <= count; ++n) {
        QString s = QString("!%1,%2,%3,%4,%5,%6,%7")
                        .arg(talker)
                        .arg(count)
                        .arg(n)
                        .arg(seq)
                        .arg(channel)
                        .arg(payload.mid((n - 1) * maxPayload, maxPayload))
                        .arg(n == count ? bits.fillBits() : 0);
        quint8 checksum = 0;
        for (int k = 1; k < s.size(); ++k) checksum ^= static_cast<quint8>(s[k].toLatin1());
        out << s + QString("*%1\r\n").arg(checksum, 2, 16, QChar('0')).toUpper();
    }
    return out;
}

}
