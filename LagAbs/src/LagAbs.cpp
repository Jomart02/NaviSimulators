#include "LagAbs.h"
#include "ui_LagAbs.h"

LagAbs::LagAbs(QWidget *parent)  :
    BaseNaviWidget(parent),
    ui(new Ui::LagAbs),
    VBW_nmea("VMVBW",{"0.0","0.0","A","0.0","0.0","A","0.0","A","0.0","A"})
{
    ui->setupUi(this);
    

}   

LagAbs::~LagAbs()
{
    delete ui;
}

QIcon LagAbs::icon() const {
    return QIcon();
}
QString LagAbs::name() const {
    return tr("Lag_Abs");
}
QString LagAbs::description() const {
    return QString("");
}

QStringList LagAbs::getNavigationData(){
    QStringList nmea;

    VBW_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Longitudinal_water_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(2, QString("%1").arg(ui->doubleSpinBox_Transverse_wated_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(3, ui->radioButton_StatusWater_A->isChecked() ? "A" : ui->radioButton_StatusWater_V->isChecked() ? "V" : "V");
    VBW_nmea.set(4, QString("%1").arg(ui->doubleSpinBox_Longitudinal_ground_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Transverse_ground_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(6, ui->radioButton_StatusGround_A->isChecked() ? "A" : ui->radioButton_StatusGround_V->isChecked() ? "V" : "V");
    VBW_nmea.set(7, QString("%1").arg(ui->doubleSpinBox_Stern_water->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(8, ui->radioButton_Stern_water_A->isChecked() ? "A" : ui->radioButton_Stern_water_V->isChecked() ? "V" : "V");
    VBW_nmea.set(9, QString("%1").arg(ui->doubleSpinBox_Stern_Ground->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(10, ui->radioButton_Stern_Ground_A->isChecked() ? "A" : ui->radioButton_Stern_Ground_V->isChecked() ? "V" : "V");

    nmea.push_back(QString::fromStdString(VBW_nmea.get_string()));
    
    return nmea;

}
