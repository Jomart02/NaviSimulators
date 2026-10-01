#include "Type14Simulator.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include "AisStructures.h"
#include "AisWidgets.h"

Type14Simulator::Type14Simulator(QWidget *parent) : BaseAISSimulator(parent)
{
    m_text = new QLineEdit(this);
    m_text->setMaxLength(161);
    m_text->setPlaceholderText(tr("До 161 символа (латиница, цифры, знаки)"));
    m_interval = makeIntervalSpin(0, tr("Только по кнопке"), this);
    auto *send = new QPushButton(tr("Отправить сейчас"), this);
    connect(send, &QPushButton::clicked, this, &Type14Simulator::sendRequested);

    auto *form = new QFormLayout(this);
    form->addRow(tr("Текст"), m_text);
    form->addRow(tr("Период отправки"), m_interval);
    form->addRow(send);
}

QVariant Type14Simulator::getData()
{
    AIS_Data_Type::Safety14 data;
    data.text = m_text->text().toUpper();
    data.intervalSec = m_interval->value();
    return QVariant::fromValue(data);
}

void Type14Simulator::setData(QVariant data)
{
    const auto p = data.value<AIS_Data_Type::Safety14>();
    m_text->setText(p.text);
    m_interval->setValue(p.intervalSec);
}

void Type14Simulator::clearParam()
{
    setData(QVariant::fromValue(AIS_Data_Type::Safety14()));
}

void Type14Simulator::updateAisData(QStringList &)
{
}
