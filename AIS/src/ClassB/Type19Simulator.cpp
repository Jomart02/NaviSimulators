#include "Type19Simulator.h"
#include "ui_Type19Simulator.h"
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

Type19Simulator::Type19Simulator(QWidget* parent) :
	BaseAISSimulator(parent),
	ui(new Ui::Type19Simulator) {
	ui->setupUi(this);
	init();
}
Type19Simulator::~Type19Simulator()
{
}

void Type19Simulator::init() {
	QStandardItemModel* modelShip = new QStandardItemModel();
	for (const auto& shipType :shipTypes )
	{
		QStandardItem* item = new QStandardItem(shipType.name);
		item->setData(shipType.id, Qt::UserRole);
		modelShip->appendRow(item);
	}
	ui->comboBox_ShipType->setModel(modelShip);

	QStandardItemModel* modelPos = new QStandardItemModel();
	for (const auto& posType :posTypes )
	{
		QStandardItem* item = new QStandardItem(posType.name);
		item->setData(posType.id, Qt::UserRole);
		modelPos->appendRow(item);
	}
	ui->comboBox_PosType->setModel(modelPos);
	QRegularExpression regex20("^[\\s\\S]{1,20}$");  
	QRegularExpressionValidator *validator20 = new QRegularExpressionValidator(regex20, ui->lineEdit_VesselName);
	ui->lineEdit_VesselName->setValidator(validator20);
	ui->lineEdit_VesselName->setPlaceholderText("Введите до 20 символов");
}

void Type19Simulator::updateAisData(QStringList &aisMess){
    
}
QVariant Type19Simulator::getData(){
	AIS_Data_Type::ClassB19 data;
	data.VesselName = ui->lineEdit_VesselName->text().toUpper();
	int indexShip = ui->comboBox_ShipType->currentIndex();
	int indexPos = ui->comboBox_PosType->currentIndex();
	data.ShipType = ui->comboBox_ShipType->model()->data(ui->comboBox_ShipType->model()->index(indexShip,0),Qt::UserRole).toInt();
	data.PositionType = ui->comboBox_PosType->model()->data(ui->comboBox_PosType->model()->index(indexPos,0),Qt::UserRole).toInt();
	data.time = ui->spinBox_timeStamp->value();
	data.SOG = ui->spinBox_SOG->value();
	data.COG = ui->doubleSpinBox_COG->value();
	data.HDG = ui->spinBox_HDG->value();
	data.lat = ui->doubleSpinBox_Lat->value();
	data.lon = ui->doubleSpinBox_Lon->value();
	data.DimensionBow = ui->spinBox_bow->value();
	data.DimensionStern = ui->spinBox_stern->value();
	data.DimensionPort = ui->spinBox_port->value();
	data.DimensionStarboard = ui->spinBox_starboard->value();
	data.PositionAccuracy = ui->radioButton_Accuracy1->isChecked() ? 0 : 1;
	data.RAIM = ui->radioButton_RAIM_Used->isChecked() ? 1 : 0;
	return QVariant::fromValue(data);
}

void Type19Simulator::setData(QVariant data){
	AIS_Data_Type::ClassB19 param = data.value<AIS_Data_Type::ClassB19>();
	ui->lineEdit_VesselName->setText(param.VesselName);
	int shipId = param.ShipType;
	QModelIndex shipIndex;
	for (int row = 0; row < ui->comboBox_ShipType->model()->rowCount(); ++row){
		QModelIndex index = ui->comboBox_ShipType->model()->index(row,0);
		QVariant userData = ui->comboBox_ShipType->model()->data(index, Qt::UserRole);
		if (userData.toInt() == shipId){
			shipIndex = index;
			break;
		}
	}
	if (shipIndex.isValid()){
		ui->comboBox_ShipType->setCurrentIndex(shipIndex.row());
	}

	int posId = param.PositionType;
	QModelIndex posIndex;
	for (int row = 0; row < ui->comboBox_PosType->model()->rowCount(); ++row){
		QModelIndex index = ui->comboBox_PosType->model()->index(row,0);
		QVariant userData = ui->comboBox_PosType->model()->data(index,Qt::UserRole);
		if(userData.toInt()==posId){
			posIndex = index;
			break;
		}
	}

	if (posIndex.isValid())
		ui->comboBox_PosType->setCurrentIndex(posIndex.row());

		ui->spinBox_bow->setValue(param.DimensionBow);
		ui->spinBox_port->setValue(param.DimensionPort);
		ui->spinBox_starboard->setValue(param.DimensionStarboard);
		ui->spinBox_stern->setValue(param.DimensionStern);


		ui->spinBox_timeStamp->setValue(param.time);
		ui->doubleSpinBox_Lat->setValue(param.lat);
		ui->doubleSpinBox_Lon->setValue(param.lon);
		ui->spinBox_SOG->setValue(param.SOG);
		ui->doubleSpinBox_COG->setValue(param.SOG);
		ui->spinBox_HDG->setValue(param.SOG);
}

void Type19Simulator::clearParam(){

	ui->lineEdit_VesselName->clear();
	ui->spinBox_bow->setValue(0);
	ui->spinBox_port->setValue(0);
	ui->spinBox_starboard->setValue(0);
	ui->spinBox_stern->setValue(0);
	ui->spinBox_SOG->setValue(0);
	ui->spinBox_HDG->setValue(0);
	ui->doubleSpinBox_COG->setValue(0);
	ui->doubleSpinBox_Lat->setValue(0);
	ui->doubleSpinBox_Lon->setValue(0);
    ui->spinBox_timeStamp->setValue(0);
	
}