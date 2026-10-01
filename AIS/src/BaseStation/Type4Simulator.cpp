#include "Type4Simulator.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QPushButton>
#include "AisDictionaries.h"
#include "AisStructures.h"
#include "AisWidgets.h"
#include "CoordinateEdit.h"

Type4Simulator::Type4Simulator(bool response, QWidget *parent) : BaseAISSimulator(parent), m_response(response)
{
    m_systemTime = new QCheckBox(tr("Время UTC из системных часов"), this);
    m_systemTime->setChecked(true);
    m_utc = new QDateTimeEdit(QDateTime::currentDateTimeUtc(), this);
    m_utc->setTimeSpec(Qt::UTC);
    m_utc->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    m_utc->setEnabled(false);
    connect(m_systemTime, &QCheckBox::toggled, m_utc, &QWidget::setDisabled);

    m_lat = new LatitudeEdit(this);
    m_lon = new LongitudeEdit(this);
    m_accuracy = new QComboBox(this);
    m_accuracy->addItem(tr("> 10 метров"), 0);
    m_accuracy->addItem(tr("< 10 метров"), 1);
    m_epfd = new QComboBox(this);
    for (const auto &item : AIS_Dict::posTypes())
        m_epfd->addItem(item.name, item.id);
    m_raim = new QComboBox(this);
    m_raim->addItem(tr("Не используется"), 0);
    m_raim->addItem(tr("Используется"), 1);
    // тип 4 - периодический, тип 11 - ответ на запрос, поэтому по умолчанию только по кнопке
    m_interval = makeIntervalSpin(response ? 0 : 10, tr("Только по кнопке"), this);
    auto *send = new QPushButton(tr("Отправить сейчас"), this);
    connect(send, &QPushButton::clicked, this, &Type4Simulator::sendRequested);

    auto *form = new QFormLayout(this);
    form->addRow(m_systemTime);
    form->addRow(tr("Дата и время UTC"), m_utc);
    form->addRow(tr("Широта"), m_lat);
    form->addRow(tr("Долгота"), m_lon);
    form->addRow(tr("Точность"), m_accuracy);
    form->addRow(tr("Система позиционирования"), m_epfd);
    form->addRow(tr("RAIM"), m_raim);
    form->addRow(tr("Период отправки"), m_interval);
    form->addRow(send);
}

QVariant Type4Simulator::getData()
{
    AIS_Data_Type::BaseStation4 data;
    data.response = m_response;
    data.useSystemTime = m_systemTime->isChecked();
    data.utc = m_utc->dateTime();
    data.lat = m_lat->value();
    data.lon = m_lon->value();
    data.PositionAccuracy = m_accuracy->currentData().toInt();
    data.PositionType = m_epfd->currentData().toInt();
    data.RAIM = m_raim->currentData().toInt();
    data.intervalSec = m_interval->value();
    return QVariant::fromValue(data);
}

void Type4Simulator::setData(QVariant data)
{
    const auto p = data.value<AIS_Data_Type::BaseStation4>();
    m_systemTime->setChecked(p.useSystemTime);
    if (p.utc.isValid()) m_utc->setDateTime(p.utc);
    m_lat->setValue(p.lat);
    m_lon->setValue(p.lon);
    m_accuracy->setCurrentIndex(qMax(0, m_accuracy->findData(p.PositionAccuracy)));
    m_epfd->setCurrentIndex(qMax(0, m_epfd->findData(p.PositionType)));
    m_raim->setCurrentIndex(qMax(0, m_raim->findData(p.RAIM)));
    m_interval->setValue(p.intervalSec);
}

void Type4Simulator::clearParam()
{
    AIS_Data_Type::BaseStation4 empty;
    empty.response = m_response;
    empty.intervalSec = m_response ? 0 : 10;
    setData(QVariant::fromValue(empty));
}

void Type4Simulator::updateAisData(QStringList &)
{
}
