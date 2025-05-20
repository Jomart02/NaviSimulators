#include "SimulatorAIS.h"
#include <QRandomGenerator>
#include <ostream>
#include <iostream>
#include "ui_SimulatorAIS.h"
#include "ClassAPage.h"

SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS),
    timerClassA(new QTimer(this)),
    classAPage(new ClassAPage(this)){
    
    ui->setupUi(this);
    ui->tabWidget->addTab(classAPage, "Class A");
    
    // type123 = new Type123Simulator();
    // type5 = new Type5Simulator();
    // ui->widgetSimulatorsA->addWidget("Type 1-3",type123,"Type123");
    // ui->widgetSimulatorsA->addWidget("Type 5",type5,"Type5");

    // connect(ui->pushButton_addClassA,&QPushButton::clicked, this ,&SimulatorAIS::addNewClassA);
    connect(timerClassA, &QTimer::timeout, this, &SimulatorAIS::sendTypeA);
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

void SimulatorAIS::startSend(){
    if(!timerClassA->isActive())
        timerClassA->start(tickInterval);
}

void SimulatorAIS::stopSend(){
    if(timerClassA->isActive())
        timerClassA->stop();
}
bool SimulatorAIS::isActive(){
    return timerClassA->isActive();
}


void SimulatorAIS::sendTypeA(){

    QStringList messages =  classAPage->getData();
    if(!messages.isEmpty()) emit sendData(messages);

}

QStringList SimulatorAIS::getNavigationData() {
    return QStringList();
}

void SimulatorAIS::addNewClassA(){
    // sending = false;
    // unsigned int min = 100000000; 
    // unsigned int max = 999999999; 

    // unsigned int randomNumber = QRandomGenerator::global()->bounded(min, max + 1);
    // int index =  ui->comboBox_NumbersClassA->count();
    // ui->comboBox_NumbersClassA->addItem(QString::number(randomNumber),randomNumber);
    // ui->comboBox_NumbersClassA->setCurrentIndex(index);
    
    // type123->clearParam();

    // ParamClassA param;
    // param.t123.MMSI = randomNumber;
    // param.t5.MMSI = randomNumber;

    // paramsShip[randomNumber] = param;
    
    // sending = true;
}

void SimulatorAIS::updateType5(){
    // if(ui->comboBox_NumbersClassA->count() == 0) return  ;
    // if(!sending) return ;

    // QStringList messages;

    // for (int i = 0; i < ui->comboBox_NumbersClassA->count(); ++i) {
    //     unsigned int number = ui->comboBox_NumbersClassA->itemData(i, Qt::UserRole).toLongLong();
    //     Type5Decoder dec;
    //     if(i == ui->comboBox_NumbersClassA->currentIndex()){
    //         if(ui->checkBox_manual->isChecked()){
    //             paramsShip[number].t5 = type5->getData().value<ClassA5>();
    //             paramsShip[number].t5.MMSI = number;
    //             dec.setParamets(paramsShip[number].t5);
    //             messages.append(dec.getString());
    //         }else{
    //             ClassA5 param = paramsShip[number].t5;
    //             paramsShip[number].t5 = param;
    //             dec.setParamets(paramsShip[number].t5);
    //             messages.append(dec.getString());
    //         }
    //     }else{
    //         ClassA5 param = paramsShip[number].t5;
    //         paramsShip[number].t5 = param;
    //         dec.setParamets(paramsShip[number].t5);
    //         messages.append(dec.getString());
    //     }
    // }

    // emit sendData(messages);
}
