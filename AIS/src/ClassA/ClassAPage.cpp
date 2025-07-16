#include "ClassAPage.h"
#include "ui_ClassAPage.h"
#include "Type123Simulator.h"
#include "Type5Simulator.h"



ClassAPage::ClassAPage(QWidget *parent) : BaseAisPage(parent) , ui(new Ui::ClassAPage){
    ui->setupUi(this);
    setComboBoxMMSI(ui->comboBox_NumbersClassA);
    setButtonAdd(ui->pushButton_addClassA);
    setCheckBoxManual(ui->checkBox_manual);
    setCheckBoxActive(ui->checkBox_Active);
    type123 = new Type123Simulator();
    type5 = new Type5Simulator();
    ui->widgetSimulatorsA->addWidget("Type 1-3",type123,"Type123");
    ui->widgetSimulatorsA->addWidget("Type 5",type5,"Type5");

}

ClassAPage::~ClassAPage()
{
    delete ui;
}


QStringList ClassAPage::getData(){
    if (!m_sending || ui->comboBox_NumbersClassA->count() == 0) {
        return QStringList();
    }

    QStringList messages;
    Type123Decoder dec;
    Type5Decoder dec5;

    for (int i = 0; i < ui->comboBox_NumbersClassA->count(); ++i) {
        unsigned int number = ui->comboBox_NumbersClassA->itemData(i, Qt::UserRole).toLongLong();
        
        auto it = paramsShip.find(number);
        if (it == paramsShip.end()) {
            continue;
        }
        if(!it->second.get()->getEnabled()) continue;
        auto* param = dynamic_cast<ParamClassA*>(it->second.get());
        if (!param) {
            continue;
        }

        bool isCurrent = (i == ui->comboBox_NumbersClassA->currentIndex());
        bool isManual = ui->checkBox_manual->isChecked();
        processClassA123(param, dec, messages, isCurrent, isManual, number);
        processClassA5(param, dec5, messages, deltaTimeSec, isCurrent, isManual, number);
    }

    deltaTimeSec = (deltaTimeSec != 5) ? deltaTimeSec + 1 : 0;
    return messages;
   
}


void ClassAPage::processClassA123(ParamClassA* param, Type123Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
    ClassA123 updatedParam = param->t123;
    if (isCurrent && isManual) {
        QVariant data = type123->getData();
        if (data.canConvert<ClassA123>()) {
            updatedParam = data.value<ClassA123>();
            
        }
    } else {
        ClassA123::calculatePos(updatedParam);
    }
    updatedParam.MMSI = number;
    param->t123 = updatedParam;
    updatedParam.MMSI = number;
    dec.setParamets(updatedParam);
    messages.append(dec.getString());
    if (isCurrent) {
        type123->setData(QVariant::fromValue(updatedParam));
    }
}

void ClassAPage::processClassA5(ParamClassA* param, Type5Decoder& dec5, QStringList& messages, int deltaTimeSec, bool isCurrent, bool isManual,unsigned int number){
    if (deltaTimeSec != 5) return;
    ClassA5 updatedParam = param->t5;
    if (isCurrent ) {
        updatedParam = type5->getData().value<ClassA5>();
        updatedParam.MMSI = param->t5.MMSI;
    }
    updatedParam.MMSI = number;
    param->t5 = updatedParam;

    dec5.setParamets(updatedParam);
    messages.append(dec5.getString());
}


std::unique_ptr<BaseParamClassAis> ClassAPage::createParam() const{
    return std::make_unique<ParamClassA>();
}

void ClassAPage::swapTarget(unsigned int prevmmsi,unsigned int mmsi){
    if(prevmmsi != 0){
        auto* paramPrev = dynamic_cast<ParamClassA*>(paramsShip.at(prevmmsi).get());
        paramPrev->t123 = type123->getData().value<ClassA123>();
        paramPrev->t5 = type5->getData().value<ClassA5>();
    }

    auto* param = dynamic_cast<ParamClassA*>(paramsShip.at(mmsi).get());
    type123->setData(QVariant::fromValue(param->t123));
    type5->setData(QVariant::fromValue(param->t5));
}