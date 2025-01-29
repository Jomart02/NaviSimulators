#include "SimulatorAIS.h"
#include <QRandomGenerator>

SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS){
        
    ui->setupUi(this);
    type123 = new Type123Simulator();
    type5 = new Type5Simulator();
    ui->widgetSimulatorsA->addWidget("Type 1-3",type123,"Type123");
    ui->widgetSimulatorsA->addWidget("Type 5",type5,"Type5");

    connect(ui->pushButton_addClassA,&QPushButton::clicked, this ,&SimulatorAIS::addNewClassA);
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
    

    return QStringList();
}

void SimulatorAIS::addNewClassA(){
    qint64 min = 100000000; 
    qint64 max = 999999999; 

    qint64 randomNumber = QRandomGenerator::global()->bounded(min, max + 1);
    ui->comboBox_NumbersClassA->addItem(QString::number(randomNumber));
}