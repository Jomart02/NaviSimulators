#include "LagRel.h"
#include "ui_LagRel.h"


LagRel::LagRel(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::LagRel),
    VHW_nmea("VMVHW",{"0.0","T","0.0","M","0.0","N","0.0","K"}),
    VBW_nmea("VMVBW",{"0.0","0.0","A","0.0","0.0","A","0.0","A","0.0","A"})
{
    ui->setupUi(this);

}

LagRel::~LagRel()
{
    delete ui;
}


QIcon LagRel::icon() const {
    return QIcon();
}
QString LagRel::name() const {
    return tr("Lag_Rel");
}
QString LagRel::description() const {
    return QString("");
}

QStringList LagRel::getNavigationData(){
    QStringList nmea;

    VBW_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Longitudinal_water_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(2, QString("%1").arg(ui->doubleSpinBox_Transverse_wated_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(3, ui->radioButton_StatusWater_A->isChecked() ? "A" : ui->radioButton_StatusWater_V->isChecked() ? "V" : "V"); // mode
    VBW_nmea.set(4, QString("%1").arg(ui->doubleSpinBox_Longitudinal_ground_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Transverse_ground_speed->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(6, ui->radioButton_StatusGround_A->isChecked() ? "A" : ui->radioButton_StatusGround_V->isChecked() ? "V" : "V"); // mode
    VBW_nmea.set(7, QString("%1").arg(ui->doubleSpinBox_Stern_water->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(8, ui->radioButton_Stern_water_A->isChecked() ? "A" : ui->radioButton_Stern_water_V->isChecked() ? "V" : "V"); // mode
    VBW_nmea.set(9, QString("%1").arg(ui->doubleSpinBox_Stern_Ground->value(), 3, 'f', 1, QChar('0')).toStdString());
    VBW_nmea.set(10, ui->radioButton_Stern_Ground_A->isChecked() ? "A" : ui->radioButton_Stern_Ground_V->isChecked() ? "V" : "V"); // mode

    nmea.push_back(QString::fromStdString(VBW_nmea.get_string()));

    VHW_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Heading_T->value(), 6, 'f', 2, QChar('0')).toStdString());
    VHW_nmea.set(2, "T");
    VHW_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_Heading_M->value(), 6, 'f', 2, QChar('0')).toStdString());
    VHW_nmea.set(4, "M");
    VHW_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Speed_kn->value(), 3, 'f', 1, QChar('0')).toStdString());
    VHW_nmea.set(6, "N");
    VHW_nmea.set(7, QString("%1").arg(ui->doubleSpinBox_Speed_km->value(), 3, 'f', 1, QChar('0')).toStdString());
    VHW_nmea.set(8, "K");

    nmea.push_back(QString::fromStdString(VHW_nmea.get_string()));

    return nmea;
}
