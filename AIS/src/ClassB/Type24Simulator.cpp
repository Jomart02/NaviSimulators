#include "Type24Simulator.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include "AisDictionaries.h"
#include "AisStructures.h"
#include "AisWidgets.h"

Type24Simulator::Type24Simulator(QWidget *parent) : BaseAISSimulator(parent)
{
    m_name = new QLineEdit(this);
    m_name->setMaxLength(20);
    m_name->setPlaceholderText(tr("Введите до 20 символов"));
    m_shipType = new QComboBox(this);
    for (const auto &item : AIS_Dict::shipTypes())
        m_shipType->addItem(item.name, item.id);
    m_callSign = new QLineEdit(this);
    m_callSign->setMaxLength(7);
    m_vendor = new QLineEdit(this);
    m_vendor->setMaxLength(3);
    m_model = makeSpin(15, this);
    m_serial = makeSpin(1048575, this);
    m_bow = makeSpin(511, this);
    m_stern = makeSpin(511, this);
    m_port = makeSpin(63, this);
    m_starboard = makeSpin(63, this);
    m_mothership = makeSpin(999999999, this);
    m_mothership->setToolTip(tr("Только для вспомогательных судов (MMSI 98XXXYYYY), вместо размеров"));
    m_interval = makeIntervalSpin(6, tr("Выключено"), this);

    auto *form = new QFormLayout(this);
    form->addRow(tr("Часть A: название судна"), m_name);
    form->addRow(tr("Часть B: тип судна"), m_shipType);
    form->addRow(tr("Позывной"), m_callSign);
    form->addRow(tr("Производитель (3 символа)"), m_vendor);
    form->addRow(tr("Код модели"), m_model);
    form->addRow(tr("Серийный номер"), m_serial);
    form->addRow(tr("До носа, м"), m_bow);
    form->addRow(tr("До кормы, м"), m_stern);
    form->addRow(tr("До левого борта, м"), m_port);
    form->addRow(tr("До правого борта, м"), m_starboard);
    form->addRow(tr("MMSI головного судна"), m_mothership);
    form->addRow(tr("Период отправки"), m_interval);
}

QVariant Type24Simulator::getData()
{
    AIS_Data_Type::ClassB24 data;
    data.VesselName = m_name->text().toUpper();
    data.ShipType = m_shipType->currentData().toUInt();
    data.CallSign = m_callSign->text().toUpper();
    data.VendorId = m_vendor->text().toUpper();
    data.Model = m_model->value();
    data.Serial = m_serial->value();
    data.DimensionBow = m_bow->value();
    data.DimensionStern = m_stern->value();
    data.DimensionPort = m_port->value();
    data.DimensionStarboard = m_starboard->value();
    data.MothershipMMSI = m_mothership->value();
    data.intervalSec = m_interval->value();
    return QVariant::fromValue(data);
}

void Type24Simulator::setData(QVariant data)
{
    const auto p = data.value<AIS_Data_Type::ClassB24>();
    m_name->setText(p.VesselName);
    m_shipType->setCurrentIndex(qMax(0, m_shipType->findData(static_cast<int>(p.ShipType))));
    m_callSign->setText(p.CallSign);
    m_vendor->setText(p.VendorId);
    m_model->setValue(p.Model);
    m_serial->setValue(p.Serial);
    m_bow->setValue(p.DimensionBow);
    m_stern->setValue(p.DimensionStern);
    m_port->setValue(p.DimensionPort);
    m_starboard->setValue(p.DimensionStarboard);
    m_mothership->setValue(p.MothershipMMSI);
    m_interval->setValue(p.intervalSec);
}

void Type24Simulator::clearParam()
{
    setData(QVariant::fromValue(AIS_Data_Type::ClassB24()));
}

void Type24Simulator::updateAisData(QStringList &)
{
}
