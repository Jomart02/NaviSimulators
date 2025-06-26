#include "ClassBPage.h"
#include "ui_ClassBPage.h"
#include "Type18Simulator.h"
#include "Type19Simulator.h"

ClassBPage::ClassBPage(QWidget *parent):BaseAisPage(parent), ui(new Ui::ClassBPage)
{
	ui->setupUi(this);
	setComboBoxMMSI(ui->comboBox_numberClassB);
    setButtonAdd(ui->pushButton_addClassB);
    setCheckBoxManual(ui->checkBox_manual);
    setCheckBoxActive(ui->checkBox_Active);
	type18 = new Type18Simulator();
	type19 = new Type19Simulator();

	
    ui->widgetSimulatorB->addWidget("Type 18",type18,"Type18");
    ui->widgetSimulatorB->addWidget("Type 19",type19,"Type19");
}

ClassBPage::~ClassBPage()
{
	delete ui;
}

QStringList ClassBPage::getData(){
	if (!m_sending || ui->comboBox_numberClassB->count() == 0) {
        return QStringList();
    }

    QStringList messages;
    Type18Decoder dec18;
    Type19Decoder dec19;

    for (int i = 0; i < ui->comboBox_numberClassB->count(); ++i) {
        unsigned int number = ui->comboBox_numberClassB->itemData(i, Qt::UserRole).toLongLong();
        auto it = paramsShip.find(number);
        if (it == paramsShip.end()) {
            continue;
        }
        if(!it->second.get()->getEnabled()) continue;
        auto* param = dynamic_cast<ParamClassB*>(it->second.get());
        if (!param) {
            continue;
        }

        bool isCurrent = (i == ui->comboBox_numberClassB->currentIndex());
        bool isManual = ui->checkBox_manual->isChecked();

        processClassB18(param, dec18, messages, isCurrent, isManual, number);
        processClassB19(param, dec19, messages, isCurrent, isManual, number);
        
    }

    return messages;
}

void ClassBPage::processClassB18(ParamClassB* param, Type18Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
    ClassB18 updatedParam = param->t18;
    if (isCurrent && isManual) {
        QVariant data = type18->getData();
        if (data.canConvert<ClassB18>()) {
            updatedParam = data.value<ClassB18>();
            updatedParam.MMSI = number;
        }
    } else {
        ClassB18::calculatePos(updatedParam);
    }
    param->t18 = updatedParam;
    dec.setParamets(updatedParam);
    messages.append(dec.getString());
    if (isCurrent) {
        type18->setData(QVariant::fromValue(updatedParam));
    }
}

void ClassBPage::processClassB19(ParamClassB* param, Type19Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
    ClassB19 updatedParam = param->t19;
    if (isCurrent && isManual) {
        QVariant data = type19->getData();
        if (data.canConvert<ClassB19>()) {
            updatedParam = data.value<ClassB19>();
            updatedParam.MMSI = number;
        }
    } else {
        ClassB19::calculatePos(updatedParam);
    }
    param->t19 = updatedParam;
    dec.setParamets(updatedParam);
    messages.append(dec.getString());
    if (isCurrent) {
        type19->setData(QVariant::fromValue(updatedParam));
    }
}

std::unique_ptr<BaseParamClassAis> ClassBPage::createParam() const{
    return std::make_unique<ParamClassB>();
}

void ClassBPage::swapTarget(int prevmmsi,unsigned int mmsi){

    if(prevmmsi != 0){
        auto* paramPrev = dynamic_cast<ParamClassB*>(paramsShip.at(prevmmsi).get());
        paramPrev->t18 = type18->getData().value<ClassB18>();
        paramPrev->t19 = type19->getData().value<ClassB19>();
    }

    auto* param = dynamic_cast<ParamClassB*>(paramsShip.at(mmsi).get());
    type18->setData(QVariant::fromValue(param->t18));
    type19->setData(QVariant::fromValue(param->t19));
}