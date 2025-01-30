#include "SimulatorAIS.h"
#include <QRandomGenerator>
#include <ostream>
#include <iostream>
using namespace AIS_NMEA_Builder;


SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS){
        
    ui->setupUi(this);
    type123 = new Type123Simulator();
    type5 = new Type5Simulator();
    ui->widgetSimulatorsA->addWidget("Type 1-3",type123,"Type123");
    ui->widgetSimulatorsA->addWidget("Type 5",type5,"Type5");

    connect(ui->pushButton_addClassA,&QPushButton::clicked, this ,&SimulatorAIS::addNewClassA);
    
    #ifdef BUILD_TEST
        startSend();
    #endif

   
}

SimulatorAIS::~SimulatorAIS()
{
    delete ui;
}
 
QIcon SimulatorAIS::icon() const {
    return QIcon();
}
QString SimulatorAIS::name() const {
    return tr("AIS");
}
QString SimulatorAIS::description() const {
    return QString("");
}
QStringList SimulatorAIS::getNavigationData() {
    if(ui->comboBox_NumbersClassA->count() == 0) return  QStringList();
    if(!sending) return QStringList();

    QStringList messages;

    for (int i = 0; i < ui->comboBox_NumbersClassA->count(); ++i) {
        unsigned int number = ui->comboBox_NumbersClassA->itemData(i, Qt::UserRole).toLongLong();
        Type123Decoder type;
        if(i == ui->comboBox_NumbersClassA->currentIndex()){
            if(ui->checkBox_manual->isChecked()){
                paramsShip[number].t123 = type123->getData().value<AIS_Data_Type::ClassA123>();
                paramsShip[number].t123.MMSI = number;
                type.setParamets(paramsShip[number].t123);
                messages.append(type.getString());
            }else{
                ClassA123 param = paramsShip[number].t123;
                ClassA123::calculatePos(param);
                paramsShip[number].t123 = param;
                type.setParamets(paramsShip[number].t123);
                messages.append(type.getString());
                type123->setData(QVariant::fromValue(param));
            }
        }else{
            ClassA123 param = paramsShip[number].t123;
            ClassA123::calculatePos(param);
            paramsShip[number].t123 = param;
            type.setParamets(paramsShip[number].t123);
            messages.append(type.getString());
        }
    }

    return messages;
}

void SimulatorAIS::addNewClassA(){
    sending = false;
    unsigned int min = 100000000; 
    unsigned int max = 999999999; 

    unsigned int randomNumber = QRandomGenerator::global()->bounded(min, max + 1);
    int index =  ui->comboBox_NumbersClassA->count();
    ui->comboBox_NumbersClassA->addItem(QString::number(randomNumber),randomNumber);
    ui->comboBox_NumbersClassA->setCurrentIndex(index);
    
    type123->clearParam();

    ParamClassA param;
    param.t123.MMSI = randomNumber;
    param.t5.MMSI = randomNumber;

    paramsShip[randomNumber] = param;
    
    sending = true;
}