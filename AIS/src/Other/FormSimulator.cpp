#include "FormSimulator.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <limits>
#include "AisWidgets.h"
#include "CoordinateEdit.h"

using AIS_Messages::FieldDef;

FormSimulator::FormSimulator(const AIS_Messages::MessageDef &def, QWidget *parent) :
    BaseAISSimulator(parent),
    m_def(def)
{
    auto *form = new QFormLayout(this);
    for (const FieldDef &f : m_def.fields) {
        QWidget *w = nullptr;
        switch (f.kind) {
        case FieldDef::UInt:
        case FieldDef::Int: {
            auto *s = new QSpinBox(this);
            s->setRange(static_cast<int>(f.kind == FieldDef::Int ? f.min : 0),
                        static_cast<int>(qMin<qint64>(f.max, std::numeric_limits<int>::max())));
            w = s;
            break;
        }
        case FieldDef::Flag:
            w = new QCheckBox(this);
            break;
        case FieldDef::Combo: {
            auto *c = new QComboBox(this);
            for (const auto &item : f.items) c->addItem(item.second, item.first);
            w = c;
            break;
        }
        case FieldDef::Text: {
            auto *e = new QLineEdit(this);
            e->setMaxLength(static_cast<int>(f.max));
            w = e;
            break;
        }
        case FieldDef::Hex: {
            auto *e = new QLineEdit(this);
            e->setMaxLength(static_cast<int>(f.max) * 3 - 1); // байты через пробел
            e->setPlaceholderText("AA BB CC");
            w = e;
            break;
        }
        case FieldDef::Lon:
            w = new LongitudeEdit(this);
            break;
        case FieldDef::Lat:
            w = new LatitudeEdit(this);
            break;
        }
        m_widgets.insert(f.key, w);
        form->addRow(f.label, w);
    }

    m_interval = makeIntervalSpin(0, tr("Только по кнопке"), this);
    auto *send = new QPushButton(tr("Отправить сейчас"), this);
    connect(send, &QPushButton::clicked, this, &FormSimulator::sendRequested);
    form->addRow(tr("Период отправки"), m_interval);
    form->addRow(send);
}

QVariant FormSimulator::getData()
{
    QVariantMap values;
    for (const FieldDef &f : m_def.fields) {
        QWidget *w = m_widgets.value(f.key);
        switch (f.kind) {
        case FieldDef::UInt:
        case FieldDef::Int:
            values[f.key] = static_cast<QSpinBox *>(w)->value();
            break;
        case FieldDef::Flag:
            values[f.key] = static_cast<QCheckBox *>(w)->isChecked();
            break;
        case FieldDef::Combo:
            values[f.key] = static_cast<QComboBox *>(w)->currentData();
            break;
        case FieldDef::Text:
        case FieldDef::Hex:
            values[f.key] = static_cast<QLineEdit *>(w)->text();
            break;
        case FieldDef::Lon:
            values[f.key] = static_cast<LongitudeEdit *>(w)->value();
            break;
        case FieldDef::Lat:
            values[f.key] = static_cast<LatitudeEdit *>(w)->value();
            break;
        }
    }
    values["_interval"] = m_interval->value();
    return values;
}

// Отсутствующие в data поля получают значения по умолчанию
void FormSimulator::setData(QVariant data)
{
    const QVariantMap values = data.toMap();
    for (const FieldDef &f : m_def.fields) {
        QWidget *w = m_widgets.value(f.key);
        const QVariant v = values.value(f.key);
        switch (f.kind) {
        case FieldDef::UInt:
        case FieldDef::Int:
            static_cast<QSpinBox *>(w)->setValue(v.isValid() ? v.toInt() : static_cast<int>(f.def));
            break;
        case FieldDef::Flag:
            static_cast<QCheckBox *>(w)->setChecked(v.toBool());
            break;
        case FieldDef::Combo: {
            auto *c = static_cast<QComboBox *>(w);
            c->setCurrentIndex(qMax(0, c->findData(v.isValid() ? v : QVariant(static_cast<int>(f.def)))));
            break;
        }
        case FieldDef::Text:
        case FieldDef::Hex:
            static_cast<QLineEdit *>(w)->setText(v.toString());
            break;
        case FieldDef::Lon:
            static_cast<LongitudeEdit *>(w)->setValue(v.toDouble());
            break;
        case FieldDef::Lat:
            static_cast<LatitudeEdit *>(w)->setValue(v.toDouble());
            break;
        }
    }
    m_interval->setValue(values.value("_interval").toInt());
}

void FormSimulator::clearParam()
{
    setData(QVariantMap());
}

void FormSimulator::updateAisData(QStringList &)
{
}
