#include "Type123Simulator.h"
#include "ui_Type123Simulator.h"
#include <QComboBox>

Type123Simulator::Type123Simulator(QWidget *parent) :
    BaseAISSimulator(parent),
    ui(new Ui::Type123Simulator){
        
    ui->setupUi(this);

    comboMessageType = new QComboBox(this);
    comboMessageType->addItem(tr("Тип 1 (по расписанию)"), 1);
    comboMessageType->addItem(tr("Тип 2 (назначенное расписание)"), 2);
    comboMessageType->addItem(tr("Тип 3 (ответ на опрос)"), 3);
    ui->gridLayout_6->addWidget(comboMessageType, 1, 0);
}

Type123Simulator::~Type123Simulator(){

}
void Type123Simulator::updateAisData(QStringList &aisMess){

}
QVariant Type123Simulator::getData() {

    AIS_Data_Type::ClassA123 data;
    data.messageType = comboMessageType->currentData().toInt();
    data.navigation = ui->comboBox_NavState->currentIndex();         
    data.ROT = ui->spinBox_ROT->value();                
    data.SOG = ui->spinBox_SOG->value(); ;               
    data.PositionAccuracy = ui->radioButton_Accuracy0->isChecked() ? 1 : 0; // 1 - точнее 10 м   
    data.lon = ui->doubleSpinBox_Lon->value(); ;             
    data.lat = ui->doubleSpinBox_Lat->value(); ;            
    data.COG = ui->doubleSpinBox_COG->value(); ;               
    data.HDG = ui->spinBox_HDG->value(); ;                
    data.time = ui->spinBox_timeStamp->value(); ;             
    
    int Man = 0;
    if(ui->radioButton_ManNo->isChecked()) Man = 0;
    else if(ui->radioButton_ManNoSpecial->isChecked()) Man = 1;
    else if(ui->radioButton_ManSpecial->isChecked()) Man = 2;
    data.maneuver = Man;


    data.RAIM = ui->radioButton_RAIM_Used->isChecked() ? 1 : 0;
    return QVariant::fromValue(data);
}
void Type123Simulator::setData(QVariant data) {
    AIS_Data_Type::ClassA123 param = data.value<AIS_Data_Type::ClassA123>();
    comboMessageType->setCurrentIndex(qBound(1, param.messageType, 3) - 1);
    ui->comboBox_NavState->setCurrentIndex(param.navigation);
    ui->spinBox_ROT->setValue(param.ROT);                
    ui->spinBox_SOG->setValue(param.SOG);              
    ui->doubleSpinBox_Lon->setValue(param.lon);            
    ui->doubleSpinBox_Lat->setValue(param.lat);           
    ui->doubleSpinBox_COG->setValue(param.COG);              
    ui->spinBox_HDG->setValue(param.HDG);            
    ui->spinBox_timeStamp->setValue(param.time);
    (param.PositionAccuracy ? ui->radioButton_Accuracy0 : ui->radioButton_Accuracy1)->setChecked(true);
    (param.RAIM ? ui->radioButton_RAIM_Used : ui->radioButton_RAIM_NotUsed)->setChecked(true);
    (param.maneuver == 1 ? ui->radioButton_ManNoSpecial
     : param.maneuver == 2 ? ui->radioButton_ManSpecial : ui->radioButton_ManNo)->setChecked(true);
}

void Type123Simulator::clearParam(){
    comboMessageType->setCurrentIndex(0);
    ui->comboBox_NavState->setCurrentIndex(1);         
    ui->spinBox_ROT->setValue(0);                
    ui->spinBox_SOG->setValue(0);              
    ui->doubleSpinBox_Lon->setValue(0);            
    ui->doubleSpinBox_Lat->setValue(0);           
    ui->doubleSpinBox_COG->setValue(0);              
    ui->spinBox_HDG->setValue(0);               
    ui->spinBox_timeStamp->setValue(0);     
}