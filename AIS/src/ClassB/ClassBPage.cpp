#include "ClassBPage.h"
#include "ui_ClassBPage.h"
#include "Type18Simulator.h"
#include "Type19Simulator.h"

ClassBPage::ClassBPage(QWidget *parent):BaseAisPage(parent), ui(new Ui::ClassBPage)
{
	ui->setupUi(this);
	setComboBoxMMSI(ui->comboBox_numberClassB);
    setButtonAdd(ui->pushButton_addClassB);
    setCheckBoxManual(ui->checkBox_manual);
    setCheckBoxActive(ui->checkBox_Active);
	type18 = new Type18Simulator();
	type19 = new Type19Simulator();
	ui->widgetSimulatorB->addWidget("Type 18",type18,"Type18");
	ui->widgetSimulatorB->addWidget("Type 19",type19,"Type19");
}

ClassBPage::~ClassBPage()
{
	delete ui;
}

QStringList ClassBPage::getData(){
	QStringList messages;
	return messages;
}

void ClassBPage::processClassB18(ParamClassB* param, Type18Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
}

void ClassBPage::processClassB19(ParamClassB* param, Type19Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number){
}

std::unique_ptr<BaseParamClassAis> ClassBPage::createParam() const{
    return std::make_unique<ParamClassB>();
}

void ClassBPage::swapTarget(int prevmmsi,unsigned int mmsi){

    if(prevmmsi != 0){
        auto* paramPrev = dynamic_cast<ParamClassB*>(paramsShip.at(prevmmsi).get());
        paramPrev->t18 = type18->getData().value<ClassB18>();
        paramPrev->t19 = type19->getData().value<ClassB19>();
    }

    auto* param = dynamic_cast<ParamClassB*>(paramsShip.at(mmsi).get());
    type18->setData(QVariant::fromValue(param->t18));
    type19->setData(QVariant::fromValue(param->t19));
}