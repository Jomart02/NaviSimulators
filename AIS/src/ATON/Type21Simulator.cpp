#include "Type21Simulator.h"
#include <QStandardItemModel>
#include <QRegularExpressionValidator>
#include "ui_Type21Simulator.h"

Type21Simulator::Type21Simulator(QWidget *parent) :
    BaseAISSimulator(parent),
    ui(new Ui::Type21Simulator){
        
    ui->setupUi(this);
    
    init();

}

Type21Simulator::~Type21Simulator(){

}

void Type21Simulator::init(){
    QStandardItemModel* modelAID = new QStandardItemModel();//добавление данных в combobox
    for (const auto& AidType : aidTypes) {
        QStandardItem* item = new QStandardItem(AidType.name);
        item->setData(AidType.id, Qt::UserRole); 
        modelAID->appendRow(item);
    }
    ui->comboBox_aidType->setModel(modelAID);

    QStandardItemModel* modelNav = new QStandardItemModel();
    for (const auto& PosType : posTypes) {
        QStandardItem* item = new QStandardItem(PosType.name);
        item->setData(PosType.id, Qt::UserRole); 
        modelNav->appendRow(item);
    }
    ui->comboBox_EPFD->setModel(modelNav);

    
    QRegularExpression regex20("^[\\s\\S]{1,20}$");  
    QRegularExpressionValidator *validator20 = new QRegularExpressionValidator(regex20, ui->lineEdit_atonName);
    ui->lineEdit_atonName->setValidator(validator20);
    ui->lineEdit_atonName->setPlaceholderText("Введите до 20 символов");

    QRegularExpression regex20_2("^[\\s\\S]{1,20}$");  
    QRegularExpressionValidator *validator20_2 = new QRegularExpressionValidator(regex20_2, ui->lineEdit_extensionAton);
    ui->lineEdit_extensionAton->setValidator(validator20);
    ui->lineEdit_extensionAton->setPlaceholderText("Введите до 20 символов");

}


void Type21Simulator::updateAisData(QStringList &aisMess){
    
}
QVariant Type21Simulator::getData() {
    AIS_Data_Type::ClassAton21 data;
    data.lat = ui->doubleSpinBox_Lat->value();
    data.lon = ui->doubleSpinBox_Lon->value();
    data.nameAton = ui->lineEdit_atonName->text().toUpper();
    data.extensionAton = ui->lineEdit_extensionAton->text().toUpper();
    int index = ui->comboBox_aidType->currentIndex();
    int index2 = ui->comboBox_EPFD->currentIndex();
    data.AIDType = ui->comboBox_aidType->model()->data(ui->comboBox_aidType->model()->index(index, 0), Qt::UserRole).toInt();
    data.PositionType = ui->comboBox_EPFD->model()->data(ui->comboBox_EPFD->model()->index(index, 0), Qt::UserRole).toInt();
    data.DimensionBow = ui->spinBox_bow->value();
    data.DimensionPort = ui->spinBox_port->value();
    data.DimensionStern = ui->spinBox_stern->value();
    data.DimensionStarboard = ui->spinBox_starboard->value();
    data.time = ui->spinBox_timeStamp->value();
    data.RAIM = ui->comboBox_RAIM->currentIndex();
    data.PositionAccuracy = ui->comboBox_Accuracy->currentIndex();
    data.offPos = ui->comboBox_offPos->currentIndex();
    data.virtualAton = ui->comboBox_virtualAton->currentIndex();
    data.Assigned = ui->comboBox_Assigned->currentIndex();
    return QVariant::fromValue(data);
}
void Type21Simulator::setData(QVariant data) {
    AIS_Data_Type::ClassAton21 param = data.value<AIS_Data_Type::ClassAton21>();
    ui->lineEdit_atonName->setText(param.nameAton);
    ui->lineEdit_extensionAton->setText(param.extensionAton);
    int Id = param.AIDType; 
    QModelIndex Index;
    for (int row = 0; row < ui->comboBox_aidType->model()->rowCount(); ++row) {
        QModelIndex index = ui->comboBox_aidType->model()->index(row, 0);
        QVariant userData = ui->comboBox_aidType->model()->data(index, Qt::UserRole);
        if (userData.toInt() == Id) {
            Index = index;
            break;
        }
    }

    if (Index.isValid()) {
        ui->comboBox_aidType->setCurrentIndex(Index.row());
    }

    int Id2 = param.PositionType; 
    QModelIndex Index2;
    for (int row = 0; row < ui->comboBox_EPFD->model()->rowCount(); ++row) {
        QModelIndex index = ui->comboBox_EPFD->model()->index(row, 0);
        QVariant userData = ui->comboBox_EPFD->model()->data(index, Qt::UserRole);
        if (userData.toInt() == Id2) {
            Index2 = index;
            break;
        }
    }

    if (Index2.isValid()) {
        ui->comboBox_EPFD->setCurrentIndex(Index2.row());
    }

    ui->spinBox_bow->setValue(param.DimensionBow);
    ui->spinBox_port->setValue(param.DimensionPort);
    ui->spinBox_stern->setValue(param.DimensionStern);
    ui->spinBox_starboard->setValue(param.DimensionStarboard);
    ui->spinBox_timeStamp->setValue(param.time);
    ui->doubleSpinBox_Lat->setValue(param.lat);
    ui->doubleSpinBox_Lon->setValue(param.lon);

}

void Type21Simulator::clearParam(){
    ui->lineEdit_atonName->clear();
    ui->lineEdit_extensionAton->clear();
    ui->spinBox_bow->setValue(0);
    ui->spinBox_port->setValue(0);
    ui->spinBox_stern->setValue(0);
    ui->spinBox_starboard->setValue(0);
    ui->spinBox_timeStamp->setValue(0);
    ui->doubleSpinBox_Lat->setValue(0);
    ui->doubleSpinBox_Lon->setValue(0);
    ui->comboBox_RAIM->setCurrentIndex(0);
    ui->comboBox_Accuracy->setCurrentIndex(0);
    ui->comboBox_offPos->setCurrentIndex(0);
    ui->comboBox_virtualAton->setCurrentIndex(0);
    ui->comboBox_Assigned->setCurrentIndex(0);    
}