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
}


void Type5Simulator::updateAisData(QStringList &aisMess){

}
QVariant Type5Simulator::getData() {

    

    return QVariant();
}
void Type5Simulator::setData(QVariant data) {

}

void Type5Simulator::clearParam(){

}