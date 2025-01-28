#include "SimulatorAIS.h"

SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS){
        
    ui->setupUi(this);
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

