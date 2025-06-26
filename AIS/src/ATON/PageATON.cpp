#include "PageATON.h"
#include "ui_PageATON.h"
#include "Type21Simulator.h"


PageATON::PageATON(QWidget *parent):BaseAisPage(parent), ui(new Ui::PageATON)
{
	ui->setupUi(this);
	setComboBoxMMSI(ui->comboBox_numbersAton);
    setButtonAdd(ui->pushButton_addAton);
    setCheckBoxManual(ui->checkBox_manual);
    setCheckBoxActive(ui->checkBox_Active);
	type21 = new Type21Simulator();
	ui->gridLayout->addWidget(type21);

}

PageATON::~PageATON()
{
	delete ui;
}

QStringList PageATON::getData(){
	if (!m_sending || ui->comboBox_numbersAton->count() == 0) {
        return QStringList();
    }

    QStringList messages;
    Type21Decoder dec21;
    

    for (int i = 0; i < ui->comboBox_numbersAton->count(); ++i) {
        unsigned int number = ui->comboBox_numbersAton->itemData(i, Qt::UserRole).toLongLong();
        auto it = paramsShip.find(number);
        if (it == paramsShip.end()) {
            continue;
        }
        if(!it->second.get()->getEnabled()) continue;
        auto* param = dynamic_cast<ParamATON*>(it->second.get());
        if (!param) {
            continue;
        }

        bool isCurrent = (i == ui->comboBox_numbersAton->currentIndex());
        bool isManual = ui->checkBox_manual->isChecked();

        processClassAton21(param, dec21, messages, isCurrent, isManual, number);
        
        
    }

    return messages;
}

void PageATON::processClassAton21(ParamATON* param, Type21Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
    ClassAton21 updatedParam = param->t21;
    if (isCurrent && isManual) {
        QVariant data = type21->getData();
        if (data.canConvert<ClassAton21>()) {
            updatedParam = data.value<ClassAton21>();
            updatedParam.MMSI = number;
        }
    } else {
        // ClassAton21::calculatePos(updatedParam);
    }
    param->t21 = updatedParam;
    dec.setParamets(updatedParam);
    messages.append(dec.getString());
    if (isCurrent) {
        type21->setData(QVariant::fromValue(updatedParam));
    }
}


std::unique_ptr<BaseParamClassAis> PageATON::createParam() const{
    return std::make_unique<ParamATON>();
}

void PageATON::swapTarget(int prevmmsi,unsigned int mmsi){

    if(prevmmsi != 0){
        auto* paramPrev = dynamic_cast<ParamATON*>(paramsShip.at(prevmmsi).get());
        paramPrev->t21 = type21->getData().value<ClassAton21>();
        
    }

    auto* param = dynamic_cast<ParamATON*>(paramsShip.at(mmsi).get());
    type21->setData(QVariant::fromValue(param->t21));
    
}