#include "Type27Simulator.h"
#include <QComboBox>
#include <QFormLayout>
#include "AisStructures.h"
#include "AisWidgets.h"

Type27Simulator::Type27Simulator(QWidget *parent) : BaseAISSimulator(parent)
{
    m_gnss = new QComboBox(this);
    m_gnss->addItem(tr("Текущая GNSS позиция"), 0);
    m_gnss->addItem(tr("Не GNSS позиция"), 1);
    m_interval = makeIntervalSpin(180, tr("Выключено"), this);

    auto *form = new QFormLayout(this);
    form->addRow(tr("Статус GNSS позиции"), m_gnss);
    form->addRow(tr("Период отправки"), m_interval);
}

QVariant Type27Simulator::getData()
{
    AIS_Data_Type::LongRange27 data;
    data.GNSSPositionStatus = m_gnss->currentData().toInt();
    data.intervalSec = m_interval->value();
    return QVariant::fromValue(data);
}

void Type27Simulator::setData(QVariant data)
{
    const auto p = data.value<AIS_Data_Type::LongRange27>();
    m_gnss->setCurrentIndex(qMax(0, m_gnss->findData(p.GNSSPositionStatus)));
    m_interval->setValue(p.intervalSec);
}

void Type27Simulator::clearParam()
{
    setData(QVariant::fromValue(AIS_Data_Type::LongRange27()));
}

void Type27Simulator::updateAisData(QStringList &)
{
}
