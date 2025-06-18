#include "SimulatorAIS.h"
#include <QRandomGenerator>
#include <ostream>
#include <iostream>
#include "ui_SimulatorAIS.h"
#include "ClassAPage.h"
#include "ClassB/ClassBPage.h"

SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS),
    timerClassA(new QTimer(this)),
    timerClassB(new QTimer(this)),
    classBPage(new ClassBPage(this)),
    classAPage(new ClassAPage(this))
{
    ui->setupUi(this);
    ui->tabWidget->addTab(classAPage, "Class A");
    ui->tabWidget->addTab(classBPage, "Class B");

    connect(timerClassA, &QTimer::timeout, this, &SimulatorAIS::sendTypeA);
    connect(timerClassB, &QTimer::timeout, this, &SimulatorAIS::sendTypeB);

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
    if(!timerClassB->isActive())
        timerClassB->start(tickInterval);
}


void SimulatorAIS::stopSend(){
    if(timerClassA->isActive())
        timerClassA->stop(); 
    if(timerClassB->isActive())
        timerClassB->stop(); 
}


bool SimulatorAIS::isActive(){
    
    return timerClassA->isActive() || timerClassB->isActive(); 
    
}




void SimulatorAIS::sendTypeA(){

    QStringList messages =  classAPage->getData();
    if(!messages.isEmpty()) emit sendData(messages);

}


void SimulatorAIS::sendTypeB(){

    QStringList messages =  classBPage->getData();
    if(!messages.isEmpty()) emit sendData(messages);

}
QStringList SimulatorAIS::getNavigationData() {
    return QStringList();
}
