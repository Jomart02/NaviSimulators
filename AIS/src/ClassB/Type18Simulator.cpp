#include "Type18Simulator.h"
#include "ui_Type18Simulator.h"
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

Type18Simulator::Type18Simulator(QWidget* parent) :
	BaseAISSimulator(parent),
	ui(new Ui::Type18Simulator) {
	ui->setupUi(this);
	
}

Type18Simulator::~Type18Simulator()
{
}


void Type18Simulator::updateAisData(QStringList &aisMess){
    
}
QVariant Type18Simulator::getData() {
	AIS_Data_Type::ClassB18 data;
	data.DSC = ui->comboBox_DSC->currentIndex();
	data.displayFlag = ui->comboBox_Disp->currentIndex();
	data.aisType = ui->comboBox_aisType->currentIndex();
	data.SOG = ui->spinBox_SOG->value();
	data.COG = ui->doubleSpinBox_COG->value();
	data.HDG = ui->spinBox_HDG->value();
	data.lat = ui->doubleSpinBox_Lat->value();
	data.lon = ui->doubleSpinBox_Lon->value();
	data.PositionAccuracy = ui->radioButton_Accuracy0->isChecked() ? 0 : 1;
	data.RAIM = ui->radioButton_RAIM_used->isChecked() ? 1 : 0;

	
	data.time = ui->spinBox_TimeStamp->value();
	return QVariant::fromValue(data);
}

void Type18Simulator::setData(QVariant data) {
	AIS_Data_Type::ClassB18 param = data.value<AIS_Data_Type::ClassB18>();
	ui->spinBox_SOG->setValue(param.SOG);
	ui->spinBox_HDG->setValue(param.HDG);
	ui->doubleSpinBox_COG->setValue(param.COG);
	ui->doubleSpinBox_Lat->setValue(param.lat);
	ui->doubleSpinBox_Lon->setValue(param.lon);

}

void Type18Simulator::clearParam() {
	ui->comboBox_aisType->setCurrentIndex(1);
	ui->comboBox_DSC->setCurrentIndex(1);
	ui->comboBox_Disp->setCurrentIndex(1);
	ui->spinBox_SOG->setValue(0);
	ui->doubleSpinBox_Lon->setValue(0);
	ui->doubleSpinBox_Lat->setValue(0);
	ui->doubleSpinBox_COG->setValue(0);
	ui->spinBox_HDG->setValue(0);
	ui->spinBox_TimeStamp->setValue(0);


}