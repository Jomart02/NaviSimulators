#include "PageSAR.h"
#include "ui_PageSAR.h"
#include "Type9Simulator.h"

PageSAR::PageSAR(QWidget *parent) : BaseAisPage(parent), ui(new Ui::PageSAR)
{
	ui->setupUi(this);
	setComboBoxMMSI(ui->comboBox_numberSar);
	setButtonAdd(ui->pushButton_addSar);
	setCheckBoxManual(ui->checkBox_manual);
	setCheckBoxActive(ui->checkBox_Active);
	type9 = new Type9Simulator();
	ui->gridLayout->addWidget(type9);
	
}

PageSAR::~PageSAR()
{
	delete ui;
}

QStringList PageSAR::getData(){
	if (!m_sending || ui->comboBox_numberSar->count() == 0) {
        return QStringList();
    }

    QStringList messages;
    Type9Decoder dec9;

    for (int i = 0; i < ui->comboBox_numberSar->count(); ++i) {
        unsigned int number = ui->comboBox_numberSar->itemData(i, Qt::UserRole).toLongLong();
        auto it = paramsShip.find(number);
        if (it == paramsShip.end()) {
            continue;
        }
        if(!it->second.get()->getEnabled()) continue;
        auto* param = dynamic_cast<ParamSAR*>(it->second.get());
        if (!param) {
            continue;
        }

        bool isCurrent = (i == ui->comboBox_numberSar->currentIndex());
        bool isManual = ui->checkBox_manual->isChecked();

        processClassSar(param, dec9, messages, isCurrent, isManual, number);
        
        
    }

    return messages;
}

void PageSAR::processClassSar(ParamSAR* param, Type9Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
    SAR updatedParam = param->t9;
    if (isCurrent && isManual) {
        QVariant data = type9->getData();
        if (data.canConvert<SAR>()) {
            updatedParam = data.value<SAR>();
            updatedParam.MMSI = number;
        }
    } else {
        SAR::calculatePos(updatedParam);
    }
    updatedParam.MMSI = number;
    param->t9 = updatedParam;
    dec.setParamets(updatedParam);
    messages.append(dec.getString());
    if (isCurrent) {
        type9->setData(QVariant::fromValue(updatedParam));
    }
}

std::unique_ptr<BaseParamClassAis> PageSAR::createParam() const{
    return std::make_unique<ParamSAR>();
}

void PageSAR::swapTarget(unsigned int prevmmsi,unsigned int mmsi){

    if(prevmmsi != 0){
        auto* paramPrev = dynamic_cast<ParamSAR*>(paramsShip.at(prevmmsi).get());
        paramPrev->t9 = type9->getData().value<SAR>();

    }

    auto* param = dynamic_cast<ParamSAR*>(paramsShip.at(mmsi).get());
    type9->setData(QVariant::fromValue(param->t9));
    
}