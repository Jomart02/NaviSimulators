#include "SimulatorAIS.h"
#include <QRandomGenerator>
#include <ostream>
#include <iostream>
#include "ui_SimulatorAIS.h"
#include "ClassAPage.h"
#include "ClassB/ClassBPage.h"
#include "PageSAR.h"

SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS),
    timerClassA(new QTimer(this)),
    timerClassB(new QTimer(this)),
    timerClassSar(new QTimer(this)),
    classBPage(new ClassBPage(this)),
    classAPage(new ClassAPage(this)),
    pageSAR(new PageSAR(this))
    
{
    ui->setupUi(this);
    ui->tabWidget->addTab(classAPage, "Class A");
    ui->tabWidget->addTab(classBPage, "Class B");
    ui->tabWidget->addTab(pageSAR, "SAR");

    connect(timerClassA, &QTimer::timeout, this, &SimulatorAIS::sendTypeA);
    connect(timerClassB, &QTimer::timeout, this, &SimulatorAIS::sendTypeB);
    connect(timerClassSar, &QTimer::timeout, this, &SimulatorAIS::sendTypeSar);

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
    if(!timerClassSar->isActive())
        timerClassSar->start(tickInterval);
}


void SimulatorAIS::stopSend(){
    if(timerClassA->isActive())
        timerClassA->stop(); 
    if(timerClassB->isActive())
        timerClassB->stop(); 
    if(timerClassSar->isActive())
        timerClassSar->stop(); 
}


bool SimulatorAIS::isActive(){
    
    return timerClassA->isActive() || timerClassB->isActive() || timerClassSar->isActive(); 
    
}




void SimulatorAIS::sendTypeA(){

    QStringList messages =  classAPage->getData();
    if(!messages.isEmpty()) emit sendData(messages);

}


void SimulatorAIS::sendTypeB(){

    QStringList messages =  classBPage->getData();
    if(!messages.isEmpty()) emit sendData(messages);

}

void SimulatorAIS::sendTypeSar(){

    QStringList messages =  pageSAR->getData();
    if(!messages.isEmpty()) emit sendData(messages);

}
QStringList SimulatorAIS::getNavigationData() {
    return QStringList();
}
