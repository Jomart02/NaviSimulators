#include "Weather.h"
#include "ui_Weather.h"

Weather::Weather(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::Weather),
    MWV_nmea("MEMWV",{"0.0","T","0.0","K","A"}),
    MWD_nmea("MEMWD",{"0.0","T","0.0","M","0.0","N","0.0","M"}),
    VWR_nmea("MEVWR",{"0.0","L","0.0","N","0.0","M","0.0","K"}),
    VWT_nmea("MEVWT",{"0.0","L","0.0","N","0.0","M","0.0","K"}),
    MDA_nmea("MEMDA",{"0.00","I","0.000","B","0.0","C","0.0","C","0.0","0","0.0","C","0.0","T","0.0","M","0.0","N","0.0","M"})
{

    ui->setupUi(this);

    connect(ui->doubleSpinBox_Direction_T,&QDoubleSpinBox::valueChanged,this,&Weather::calcAngleT);
    connect(ui->doubleSpinBox_Curs_ship,&QDoubleSpinBox::valueChanged,this,&Weather::calcAngleT);

    connect(ui->doubleSpinBox_Direction_R,&QDoubleSpinBox::valueChanged,this,&Weather::calcAngleR);
    connect(ui->doubleSpinBox_Curs_ship,&QDoubleSpinBox::valueChanged,this,&Weather::calcAngleR);

    #ifdef BUILD_TEST
        startSend();
    #endif
}

void Weather::calcAngleT(double v){
    double Direction = ui->doubleSpinBox_Direction_T->value();
    double Cource = ui->doubleSpinBox_Curs_ship->value();
     // Вычисляем относительный угол ветра
    double relativeWindAngle = Direction - Cource;

    // Нормализуем угол в диапазоне от -180 до 180 градусов
    if (relativeWindAngle > 180) {
        relativeWindAngle -= 360;
    } else if (relativeWindAngle < -180) {
        relativeWindAngle += 360;
    }
    ui->doubleSpinBox_Angle_T->setValue(relativeWindAngle);
}
void Weather::calcAngleR(double v){
    double Direction = ui->doubleSpinBox_Direction_R->value();
    double Cource = ui->doubleSpinBox_Curs_ship->value();
     // Вычисляем относительный угол ветра
    double relativeWindAngle = Direction - Cource;

    // Нормализуем угол в диапазоне от -180 до 180 градусов
    if (relativeWindAngle > 180) {
        relativeWindAngle -= 360;
    } else if (relativeWindAngle < -180) {
        relativeWindAngle += 360;
    }
    ui->doubleSpinBox_Angle_R->setValue(relativeWindAngle);
}
Weather::~Weather()
{
    delete ui;
}

QIcon Weather::icon() const {
    return QIcon();
}
QString Weather::name() const {
    return tr("Weather");
}
QString Weather::description() const {
    return QString("");
}

QStringList Weather::getNavigationData(){
    QStringList nmea;

    MWD_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Direction_T->value(), 6, 'f', 2, QChar('0')).toStdString());
    MWD_nmea.set(2, "T");
    MWD_nmea.set(3, QString("%1").arg(0.0, 6, 'f', 2, QChar('0')).toStdString());
    MWD_nmea.set(4, "M");
    MWD_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Speed_T->value(), 3, 'f', 1, QChar('0')).toStdString());
    MWD_nmea.set(6, "N");
    MWD_nmea.set(7, QString("%1").arg(ui->doubleSpinBox_Speed_T->value() * 0.514, 3, 'f', 1, QChar('0')).toStdString());
    MWD_nmea.set(8, "M");
    nmea.push_back(QString::fromStdString(MWD_nmea.get_string()));

    MDA_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_pressure_B->value() * 29.61, 5, 'f', 2, QChar('0')).toStdString());
    MDA_nmea.set(2, "I");
    MDA_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_pressure_B->value(), 6, 'f', 3, QChar('0')).toStdString());
    MDA_nmea.set(4, "B");
    MDA_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Air_Temp->value(), 3, 'f', 1, QChar('0')).toStdString());
    MDA_nmea.set(6, "C");
    MDA_nmea.set(7, QString("%1").arg(0.0, 3, 'f', 1, QChar('0')).toStdString());
    MDA_nmea.set(8, "C");
    MDA_nmea.set(9, QString("%1").arg(ui->doubleSpinBox_humidity_R->value(), 6, 'f', 2, QChar('0')).toStdString());
    MDA_nmea.set(10, "0");
    MDA_nmea.set(11, QString("%1").arg(ui->doubleSpinBox_Dew->value(), 3, 'f', 1, QChar('0')).toStdString());
    MDA_nmea.set(12, "C");
    MDA_nmea.set(13, QString("%1").arg(ui->doubleSpinBox_Direction_T->value(), 6, 'f', 2, QChar('0')).toStdString());
    MDA_nmea.set(14, "T");
    MDA_nmea.set(15, QString("%1").arg(0.0, 6, 'f', 2, QChar('0')).toStdString());
    MDA_nmea.set(16, "M");
    MDA_nmea.set(17, QString("%1").arg(ui->doubleSpinBox_Speed_T->value(), 3, 'f', 1, QChar('0')).toStdString());
    MDA_nmea.set(18, "N");
    MDA_nmea.set(19, QString("%1").arg(ui->doubleSpinBox_Speed_T->value() * 0.514, 3, 'f', 1, QChar('0')).toStdString());
    MDA_nmea.set(20, "M");
    nmea.push_back(QString::fromStdString(MDA_nmea.get_string()));

//     MWV_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Direction_R->value(), 6, 'f', 2, QChar('0')).toStdString());
// MWV_nmea.set(2, "R");
// MWV_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_Speed_R->value(), 3, 'f', 1, QChar('0')).toStdString());
// MWV_nmea.set(4, "N");
// MWV_nmea.set(5, "A");

// // MWV_nmea (второй блок)
// MWV_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Direction_T->value(), 6, 'f', 2, QChar('0')).toStdString());
// MWV_nmea.set(2, "R");
// MWV_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_Speed_T->value(), 3, 'f', 1, QChar('0')).toStdString());
// MWV_nmea.set(4, "N");
// MWV_nmea.set(5, "A");

    VWR_nmea.set(1, QString("%1").arg(qAbs(ui->doubleSpinBox_Angle_R->value()), 6, 'f', 2, QChar('0')).toStdString());
    VWR_nmea.set(2, ui->doubleSpinBox_Angle_R->value() > 0 ? "R" : "L");
    VWR_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_Speed_R->value(), 3, 'f', 1, QChar('0')).toStdString());
    VWR_nmea.set(4, "N");
    VWR_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Speed_R->value() * 0.514, 3, 'f', 1, QChar('0')).toStdString());
    VWR_nmea.set(6, "M");
    VWR_nmea.set(7, QString("%1").arg(ui->doubleSpinBox_Speed_R->value() * 1.852, 3, 'f', 1, QChar('0')).toStdString());
    VWR_nmea.set(8, "K");
    nmea.push_back(QString::fromStdString(VWR_nmea.get_string()));

    VWT_nmea.set(1, QString("%1").arg(qAbs(ui->doubleSpinBox_Angle_T->value()), 6, 'f', 2, QChar('0')).toStdString());
    VWT_nmea.set(2, ui->doubleSpinBox_Angle_T->value() > 0 ? "R" : "L");
    VWT_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_Speed_T->value(), 3, 'f', 1, QChar('0')).toStdString());
    VWT_nmea.set(4, "N");
    VWT_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_Speed_T->value() * 0.514, 3, 'f', 1, QChar('0')).toStdString());
    VWT_nmea.set(6, "M");
    VWT_nmea.set(7, QString("%1").arg(ui->doubleSpinBox_Speed_T->value() * 1.852, 3, 'f', 1, QChar('0')).toStdString());
    VWT_nmea.set(8, "K");
    nmea.push_back(QString::fromStdString(VWT_nmea.get_string()));

    return nmea;
}
