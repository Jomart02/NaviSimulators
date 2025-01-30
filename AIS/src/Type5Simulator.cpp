#include "Type5Simulator.h"
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

Type5Simulator::Type5Simulator(QWidget *parent) :
    BaseAISSimulator(parent),
    ui(new Ui::Type5Simulator){
        
    ui->setupUi(this);

    init();

}

Type5Simulator::~Type5Simulator(){

}

void Type5Simulator::init(){
    QStandardItemModel* model = new QStandardItemModel();
    for (const auto& shipType : shipTypes) {
        QStandardItem* item = new QStandardItem(shipType.name);
        item->setData(shipType.id, Qt::UserRole); 
        model->appendRow(item);
    }
    ui->comboBox_ShipType->setModel(model);

    QStandardItemModel* model2 = new QStandardItemModel();
    for (const auto& posType : posTypes) {
        QStandardItem* item = new QStandardItem(posType.name);
        item->setData(posType.id, Qt::UserRole); 
        model2->appendRow(item);
    }
    ui->comboBox_PosType->setModel(model2);

    QRegularExpression regex7("^[\\s\\S]{1,7}$"); 
    QRegularExpressionValidator *validator7 = new QRegularExpressionValidator(regex7, ui->lineEdit_CallSign);
    ui->lineEdit_CallSign->setValidator(validator7);
    ui->lineEdit_CallSign->setPlaceholderText("Введите до 7 символов");
    QRegularExpression regex20("^[\\s\\S]{1,20}$");  
    QRegularExpressionValidator *validator20 = new QRegularExpressionValidator(regex20, ui->lineEdit_VesselName);
    ui->lineEdit_VesselName->setValidator(validator20);
    ui->lineEdit_VesselName->setPlaceholderText("Введите до 20 символов");

    QRegularExpression regex20_2("^[\\s\\S]{1,20}$");  
    QRegularExpressionValidator *validator20_2 = new QRegularExpressionValidator(regex20_2, ui->lineEdit_Destination);
    ui->lineEdit_Destination->setValidator(validator20);
    ui->lineEdit_Destination->setPlaceholderText("Введите до 20 символов");

    ui->dateTimeEdit_ETA->setDateTime(QDateTime::currentDateTime());
}


void Type5Simulator::updateAisData(QStringList &aisMess){
    
}
QVariant Type5Simulator::getData() {

    AIS_Data_Type::ClassA5 data;
    data.CallSign = ui->lineEdit_CallSign->text().toUpper();
    data.IMO = ui->spinBox_IMO->value();
    data.VesselName = ui->lineEdit_VesselName->text().toUpper();
    data.Destination = ui->lineEdit_Destination->text().toUpper();
    data.Draught = ui->doubleSpinBox_Draught->value();
    int index = ui->comboBox_ShipType->currentIndex();
    int index2 = ui->comboBox_PosType->currentIndex();
    data.ShipType = ui->comboBox_ShipType->model()->data(ui->comboBox_ShipType->model()->index(index, 0), Qt::UserRole).toInt();
    data.PositionType = ui->comboBox_PosType->model()->data(ui->comboBox_PosType->model()->index(index, 0), Qt::UserRole).toInt();
    data.DimensionBow = ui->spinBox_bow->value();
    data.DimensionPort = ui->spinBox_port->value();
    data.DimensionStern = ui->spinBox_stern->value();
    data.DimensionStarboard = ui->spinBox_starboard->value();
    data.ETA = ui->dateTimeEdit_ETA->dateTime();

    return QVariant::fromValue(data);
}
void Type5Simulator::setData(QVariant data) {
    AIS_Data_Type::ClassA5 param = data.value<AIS_Data_Type::ClassA5>();
    ui->lineEdit_CallSign->setText(param.CallSign);
    ui->spinBox_IMO->setValue(param.IMO);
    ui->lineEdit_VesselName->setText(param.VesselName);
    ui->lineEdit_Destination->setText(param.Destination);
    ui->doubleSpinBox_Draught->setValue(param.Draught);

    int Id = param.ShipType; 
    QModelIndex Index;
    for (int row = 0; row < ui->comboBox_ShipType->model()->rowCount(); ++row) {
        QModelIndex index = ui->comboBox_ShipType->model()->index(row, 0);
        QVariant userData = ui->comboBox_ShipType->model()->data(index, Qt::UserRole);
        if (userData.toInt() == Id) {
            Index = index;
            break;
        }
    }

    if (Index.isValid()) {
        ui->comboBox_ShipType->setCurrentIndex(Index.row());
    }

    int Id2 = param.PositionType; 
    QModelIndex Index2;
    for (int row = 0; row < ui->comboBox_PosType->model()->rowCount(); ++row) {
        QModelIndex index = ui->comboBox_PosType->model()->index(row, 0);
        QVariant userData = ui->comboBox_PosType->model()->data(index, Qt::UserRole);
        if (userData.toInt() == Id2) {
            Index2 = index;
            break;
        }
    }

    if (Index2.isValid()) {
        ui->comboBox_PosType->setCurrentIndex(Index2.row());
    }

    ui->spinBox_bow->setValue(param.DimensionBow);
    ui->spinBox_port->setValue(param.DimensionPort);
    ui->spinBox_stern->setValue(param.DimensionStern);
    ui->spinBox_starboard->setValue(param.DimensionStarboard);
    ui->dateTimeEdit_ETA->setDateTime(param.ETA);
}

void Type5Simulator::clearParam(){
    ui->lineEdit_CallSign->clear();
    ui->spinBox_IMO->setValue(0);
    ui->lineEdit_VesselName->clear();
    ui->lineEdit_Destination->clear();
    ui->doubleSpinBox_Draught->setValue(0);


    ui->spinBox_bow->setValue(0);
    ui->spinBox_port->setValue(0);
    ui->spinBox_stern->setValue(0);
    ui->spinBox_starboard->setValue(0);
    ui->dateTimeEdit_ETA->setDateTime(QDateTime::currentDateTime());
}