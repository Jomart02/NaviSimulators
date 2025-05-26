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
