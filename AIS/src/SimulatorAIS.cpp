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
    classAPage(new ClassAPage(this)),
    remoteAIS(new RemoteAis(this)){
    
    ui->setupUi(this);
    ui->tabWidget->addTab(classAPage, "Class A");
    ui->tabWidget->addTab(remoteAIS, "Remote");

    connect(timerClassA, &QTimer::timeout, this, &SimulatorAIS::sendTypeA);
    connect(remoteAIS, &RemoteAis::dataResived, this, &SimulatorAIS::sendData);
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

    remoteAIS->setActiveSend(true);
}

void SimulatorAIS::stopSend(){
    if(timerClassA->isActive())
        timerClassA->stop();
    
    remoteAIS->setActiveSend(false);
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
