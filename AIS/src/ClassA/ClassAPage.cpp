#include "ClassAPage.h"
#include "ui_ClassAPage.h"
#include "Type123Simulator.h"
#include "Type5Simulator.h"

using namespace AIS_NMEA_Builder;

ClassAPage::ClassAPage(QWidget *parent) : BaseAisPage(parent) , ui(new Ui::ClassAPage){
    ui->setupUi(this);
    setComboBoxMMSI(ui->comboBox_NumbersClassA);
    setButtonAdd(ui->pushButton_addClassA);
    setCheckBoxManual(ui->checkBox_manual);

    type123 = new Type123Simulator();
    type5 = new Type5Simulator();
    ui->widgetSimulatorsA->addWidget("Type 1-3",type123,"Type123");
    ui->widgetSimulatorsA->addWidget("Type 5",type5,"Type5");

}

ClassAPage::~ClassAPage()
{
    delete ui;
}


QStringList ClassAPage::getData(){

    if (!sending || ui->comboBox_NumbersClassA->count() == 0) {
        return QStringList();
    }

    QStringList messages;

    for (int i = 0; i < ui->comboBox_NumbersClassA->count(); ++i) {
            unsigned int number = ui->comboBox_NumbersClassA->itemData(i, Qt::UserRole).toLongLong();
            auto* param = dynamic_cast<ParamClassA*>(paramsShip[number].get());
            if (!param) {
                continue; 
            }

            Type123Decoder dec;
            Type5Decoder dec5;

            if (i == ui->comboBox_NumbersClassA->currentIndex() && ui->checkBox_manual->isChecked()) {
                param->t123 = type123->getData().value<ClassA123>();
                param->t123.MMSI = number;
                dec.setParamets(param->t123);
                messages.append(dec.getString());

                if (deltaTimeSec == 5) {
                    param->t5 = type5->getData().value<ClassA5>();
                    param->t5.MMSI = number;
                    dec5.setParamets(param->t5);
                    messages.append(dec5.getString());
                }
            } else {
                ClassA123 updatedParam = param->t123;
                ClassA123::calculatePos(updatedParam);
                param->t123 = updatedParam;
                dec.setParamets(param->t123);
                messages.append(dec.getString());
                type123->setData(QVariant::fromValue(param->t123));

                if (deltaTimeSec == 5) {
                    dec5.setParamets(param->t5);
                    messages.append(dec5.getString());
                }
            }
        }

        deltaTimeSec = (deltaTimeSec != 5) ? deltaTimeSec + 1 : 0;


    return messages;
   
}

std::unique_ptr<BaseParamClassAis> ClassAPage::createParam() const{
    return std::make_unique<ParamClassA>();
}