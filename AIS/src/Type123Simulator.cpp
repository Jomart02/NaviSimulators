#include "Type123Simulator.h"

Type123Simulator::Type123Simulator(QWidget *parent) :
    BaseAISSimulator(parent),
    ui(new Ui::Type123Simulator){
        
    ui->setupUi(this);
}

Type123Simulator::~Type123Simulator(){

}
void Type123Simulator::updateAisData(QStringList &aisMess){

}
QVariant Type123Simulator::getData() {

    AIS_Data_Type::ClassA123 data;
    data.navigation = ui->comboBox_NavState->currentIndex();         
    data.ROT = ui->spinBox_ROT->value();                
    data.SOG = ui->spinBox_SOG->value(); ;               
    data.PositionAccuracy = ui->radioButton_Accuracy0->isChecked() ? 0 : 1;   
    data.lon = ui->doubleSpinBox_Lat->value(); ;             
    data.lat = ui->doubleSpinBox_Lon->value(); ;            
    data.COG = ui->doubleSpinBox_COG->value(); ;               
    data.HDG = ui->spinBox_HDG->value(); ;                
    data.time = ui->spinBox_timeStamp->value(); ;             
    
    int dte = 0;
    if(ui->radioButton_ManNo->isChecked()) dte = 0;
    else if(ui->radioButton_ManNoSpecial->isChecked()) dte = 1;
    else if(ui->radioButton_ManSpecial->isChecked()) dte = 2;
    data.DTE = dte;


    data.RAIM = ui->radioButton_RAIM_Used->isChecked() ? 1 : 0;
    return QVariant::fromValue(data);
}
void Type123Simulator::setData(QVariant data) {

}