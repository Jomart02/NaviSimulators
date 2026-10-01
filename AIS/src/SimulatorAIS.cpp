#include "SimulatorAIS.h"
#include "ui_SimulatorAIS.h"
#include "ClassAPage.h"
#include "ClassB/ClassBPage.h"
#include "PageSAR.h"
#include "PageATON.h"
#include "PageBaseStation.h"
#include "PageOther.h"

SimulatorAIS::SimulatorAIS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SimulatorAIS)
{
    ui->setupUi(this);
    addPage(new ClassAPage(this), "Class A");
    addPage(new ClassBPage(this), "Class B");
    addPage(new PageSAR(this), "SAR");
    addPage(new PageATON(this), "ATON");
    addPage(new PageBaseStation(this), "Base station");
    addPage(new PageOther(this), "Other");
}


SimulatorAIS::~SimulatorAIS()
{
    delete ui;
}

void SimulatorAIS::addPage(BaseAisPage *page, const QString &title){
    ui->tabWidget->addTab(page, title);
    pages.append(page);
    // сообщения по кнопке уходят сразу, не дожидаясь таймера
    connect(page, &BaseAisPage::sendNow, this, &BaseNaviWidget::sendData);
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

// Раз в tickInterval базовый таймер собирает сообщения со всех страниц
QStringList SimulatorAIS::getNavigationData() {
    QStringList messages;
    for (BaseAisPage *page : pages)
        messages += page->getData();
    return messages;
}
