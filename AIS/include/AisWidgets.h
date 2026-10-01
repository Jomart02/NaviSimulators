#pragma once
#include <QSpinBox>

// Мелкие помощники для виджетов, собираемых в коде

inline QSpinBox *makeSpin(int max, QWidget *parent = nullptr)
{
    auto *s = new QSpinBox(parent);
    s->setRange(0, max);
    return s;
}

// Период отправки в секундах; 0 - выключено / только по кнопке
inline QSpinBox *makeIntervalSpin(int value, const QString &zeroText, QWidget *parent = nullptr)
{
    auto *s = makeSpin(86400, parent);
    s->setSuffix(QObject::tr(" с"));
    s->setSpecialValueText(zeroText);
    s->setValue(value);
    return s;
}
