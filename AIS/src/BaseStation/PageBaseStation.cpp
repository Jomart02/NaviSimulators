#include "PageBaseStation.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include "ToolBox.h"
#include "Type4Simulator.h"
#include "Type14Simulator.h"

PageBaseStation::PageBaseStation(QWidget *parent) : BaseAisPage(parent)
{
    auto *active = new QCheckBox(tr("Активна"), this);
    active->setChecked(true);
    auto *numbers = new QComboBox(this);
    auto *add = new QPushButton(tr("Добавить"), this);
    auto *toolBox = new ToolBox(this);

    auto *grid = new QGridLayout(this);
    grid->addWidget(active, 0, 0);
    grid->addWidget(numbers, 1, 0);
    grid->addWidget(add, 2, 0);
    grid->setRowStretch(3, 1);
    grid->addWidget(toolBox, 0, 1, 4, 1);

    setComboBoxMMSI(numbers);
    setButtonAdd(add);
    setCheckBoxActive(active);

    auto *t4 = new Type4Simulator(false);
    auto *t11 = new Type4Simulator(true);
    auto *t14 = new Type14Simulator();
    type4 = t4;
    type11 = t11;
    type14 = t14;
    toolBox->addWidget("Type 4", t4, "Type4");
    toolBox->addWidget("Type 11", t11, "Type11");
    toolBox->addWidget("Type 14", t14, "Type14");

    connect(t4, &Type4Simulator::sendRequested, this, [this] { sendOnce<Type4Decoder, AIS_Data_Type::BaseStation4>(type4); });
    connect(t11, &Type4Simulator::sendRequested, this, [this] { sendOnce<Type4Decoder, AIS_Data_Type::BaseStation4>(type11); });
    connect(t14, &Type14Simulator::sendRequested, this, [this] { sendOnce<Type14Decoder, AIS_Data_Type::Safety14>(type14); });
}

// Отправка по кнопке: параметры берутся из виджета, MMSI - выбранной станции
template<class Decoder, class Data>
void PageBaseStation::sendOnce(BaseAISSimulator *simulator)
{
    if (m_comboBox_NumbersMMSI->count() == 0) return;
    Data data = simulator->getData().value<Data>();
    data.MMSI = m_comboBox_NumbersMMSI->currentData().toUInt();
    Decoder dec;
    dec.setParamets(data);
    emit sendNow(dec.getString());
}

QStringList PageBaseStation::getData()
{
    if (!m_sending || m_comboBox_NumbersMMSI->count() == 0) {
        return QStringList();
    }

    QStringList messages;
    Type4Decoder dec4;
    Type14Decoder dec14;

    for (int i = 0; i < m_comboBox_NumbersMMSI->count(); ++i) {
        unsigned int number = m_comboBox_NumbersMMSI->itemData(i, Qt::UserRole).toUInt();
        auto it = paramsShip.find(number);
        if (it == paramsShip.end() || !it->second->getEnabled()) continue;
        auto *param = dynamic_cast<ParamBase *>(it->second.get());
        if (!param) continue;

        if (i == m_comboBox_NumbersMMSI->currentIndex()) {
            param->t4 = type4->getData().value<BaseStation4>();
            param->t11 = type11->getData().value<BaseStation4>();
            param->t14 = type14->getData().value<Safety14>();
        }
        param->setMMSI(number);

        if (due(param->t4.intervalSec, param->elapsed4)) {
            dec4.setParamets(param->t4);
            messages += dec4.getString();
        }
        if (due(param->t11.intervalSec, param->elapsed11)) {
            dec4.setParamets(param->t11);
            messages += dec4.getString();
        }
        if (due(param->t14.intervalSec, param->elapsed14)) {
            dec14.setParamets(param->t14);
            messages += dec14.getString();
        }
    }
    return messages;
}

std::unique_ptr<BaseParamClassAis> PageBaseStation::createParam() const
{
    return std::make_unique<ParamBase>();
}

void PageBaseStation::swapTarget(unsigned int prevmmsi, unsigned int mmsi)
{
    if (prevmmsi != 0) {
        auto *paramPrev = dynamic_cast<ParamBase *>(paramsShip.at(prevmmsi).get());
        paramPrev->t4 = type4->getData().value<BaseStation4>();
        paramPrev->t11 = type11->getData().value<BaseStation4>();
        paramPrev->t14 = type14->getData().value<Safety14>();
    }

    auto *param = dynamic_cast<ParamBase *>(paramsShip.at(mmsi).get());
    type4->setData(QVariant::fromValue(param->t4));
    type11->setData(QVariant::fromValue(param->t11));
    type14->setData(QVariant::fromValue(param->t14));
}
