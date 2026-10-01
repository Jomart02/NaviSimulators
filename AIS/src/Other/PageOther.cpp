#include "PageOther.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include "AisMessages.h"
#include "FormSimulator.h"
#include "ToolBox.h"

PageOther::PageOther(QWidget *parent) : BaseAisPage(parent)
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

    for (const AIS_Messages::MessageDef &def : AIS_Messages::defs()) {
        auto *form = new FormSimulator(def);
        forms.append(form);
        types.append(def.type);
        toolBox->addWidget(QString("Type %1: %2").arg(def.type).arg(def.title), form, QString("Type%1").arg(def.type));

        // отправка по кнопке: значения берутся из формы, источник - выбранный MMSI
        connect(form, &FormSimulator::sendRequested, this, [this, form, def] {
            if (m_comboBox_NumbersMMSI->count() == 0) return;
            emit sendNow(AIS_Messages::encode(def, form->getData().toMap(), m_comboBox_NumbersMMSI->currentData().toUInt()));
        });
    }
}

void PageOther::storeForms(ParamOther *param)
{
    for (int i = 0; i < forms.size(); ++i)
        param->values[types[i]] = forms[i]->getData().toMap();
}

QStringList PageOther::getData()
{
    if (!m_sending || m_comboBox_NumbersMMSI->count() == 0) {
        return QStringList();
    }

    QStringList messages;
    for (int i = 0; i < m_comboBox_NumbersMMSI->count(); ++i) {
        unsigned int number = m_comboBox_NumbersMMSI->itemData(i, Qt::UserRole).toUInt();
        auto it = paramsShip.find(number);
        if (it == paramsShip.end() || !it->second->getEnabled()) continue;
        auto *param = dynamic_cast<ParamOther *>(it->second.get());
        if (!param) continue;

        if (i == m_comboBox_NumbersMMSI->currentIndex()) storeForms(param);

        for (const AIS_Messages::MessageDef &def : AIS_Messages::defs()) {
            const QVariantMap &values = param->values[def.type];
            if (due(values.value("_interval").toInt(), param->elapsed[def.type]))
                messages += AIS_Messages::encode(def, values, number);
        }
    }
    return messages;
}

std::unique_ptr<BaseParamClassAis> PageOther::createParam() const
{
    return std::make_unique<ParamOther>();
}

void PageOther::swapTarget(unsigned int prevmmsi, unsigned int mmsi)
{
    if (prevmmsi != 0)
        storeForms(dynamic_cast<ParamOther *>(paramsShip.at(prevmmsi).get()));

    auto *param = dynamic_cast<ParamOther *>(paramsShip.at(mmsi).get());
    for (int i = 0; i < forms.size(); ++i)
        forms[i]->setData(param->values[types[i]]);
}
