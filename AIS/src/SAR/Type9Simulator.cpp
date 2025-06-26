#include "Type9Simulator.h"
#include "ui_Type9Simulator.h"

Type9Simulator::Type9Simulator(QWidget *parent) : BaseAISSimulator(parent), ui(new Ui::Type9Simulator)
{
	ui->setupUi(this);
}

Type9Simulator::~Type9Simulator()
{
}

void Type9Simulator::updateAisData(QStringList &aisMess){

}

QVariant Type9Simulator::getData() {
	AIS_Data_Type::SAR data;
	data.lat = ui->doubleSpinBox_Lat->value();
	data.lon = ui->doubleSpinBox_Lon->value();
	data.COG = ui->doubleSpinBox_COG->value();
	data.SOG = ui->spinBox_SOG->value();
	data.time = ui->spinBox_time->value();
	data.PositionAccuracy = ui->radioButton_Accuracy1->isChecked() ? 0 : 1;
	data.RAIM = ui->radioButton_RAIM_used->isChecked() ? 1: 0;
	data.Assigned = ui->radioButton_auto->isChecked() ? 0 : 1;
	data.altitude = ui->spinBox_altitude->value();
	return QVariant::fromValue(data);
}

void Type9Simulator::setData(QVariant data) {
	AIS_Data_Type::SAR param = data.value<AIS_Data_Type::SAR>();
	ui->spinBox_SOG->setValue(param.SOG);
	ui->doubleSpinBox_COG->setValue(param.COG);
	ui->doubleSpinBox_Lat->setValue(param.lat);
	ui->doubleSpinBox_Lon->setValue(param.lon);
	ui->spinBox_time->setValue(param.time);
	ui->spinBox_altitude->setValue(param.altitude);
}

void Type9Simulator::clearParam(){
	
	ui->spinBox_SOG->setValue(0);
	ui->doubleSpinBox_Lon->setValue(0);
	ui->doubleSpinBox_Lat->setValue(0);
	ui->doubleSpinBox_COG->setValue(0);
	ui->spinBox_time->setValue(0);
	ui->spinBox_altitude->setValue(0);
}
