#include "Compas.h"
#include "ui_Compas.h"

Compas::Compas(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::Compas),
    VHW_nmea("HEVHW",{"0.0","T","0.0","M","0.0","N","0.0","K"}),
    VHW2_nmea("ECVHW",{"0.0","T","0.0","M","0.0","N","0.0","K"}),
    HDT_nmea("HEHDT",{"0.0","T"}),
    THS_nmea("HETHS",{"0.0","T"}),
    HDG_nmea("ECHDG",{"0.0","0.0","E","0.0","E"})
{

    ui->setupUi(this);
    #ifdef BUILD_TEST
        startSend();
    #endif
}

Compas::~Compas()
{
    delete ui;
}

QIcon Compas::icon() const {
    return QIcon();
}
QString Compas::name() const {
    return tr("Compas");
}
QString Compas::description() const {
    return QString("");
}

QStringList Compas::getNavigationData(){
    QStringList nmea;
    if(ui->radioButton_Gyro){
        VHW_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Heading_T->value(), 6, 'f', 2, QChar('0')).toStdString());
        VHW_nmea.set(2, "T");
        VHW_nmea.set(3, QString("%1").arg(0.0, 6, 'f', 2, QChar('0')).toStdString());
        VHW_nmea.set(4, "M");
        VHW_nmea.set(5, QString("%1").arg(0.0, 3, 'f', 1, QChar('0')).toStdString());
        VHW_nmea.set(6, "N");
        VHW_nmea.set(7, QString("%1").arg(0.0, 3, 'f', 1, QChar('0')).toStdString());
        VHW_nmea.set(8, "K");
        nmea.push_back(QString::fromStdString(VHW_nmea.get_string()));

        HDT_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Heading_T->value(), 6, 'f', 2, QChar('0')).toStdString());
        HDT_nmea.set(2, "T");
        nmea.push_back(QString::fromStdString(HDT_nmea.get_string()));

        THS_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Heading_T->value(), 6, 'f', 2, QChar('0')).toStdString());
        THS_nmea.set(2, "T");
        nmea.push_back(QString::fromStdString(THS_nmea.get_string()));
    }
    if(ui->radioButton_Magn){
        VHW2_nmea.set(1, QString("%1").arg(0.0, 6, 'f', 2, QChar('0')).toStdString());
        VHW2_nmea.set(2, "T");
        VHW2_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_Heading_M->value(), 6, 'f', 2, QChar('0')).toStdString());
        VHW2_nmea.set(4, "M");
        VHW2_nmea.set(5, QString("%1").arg(0.0, 3, 'f', 1, QChar('0')).toStdString());
        VHW2_nmea.set(6, "N");
        VHW2_nmea.set(7, QString("%1").arg(0.0, 3, 'f', 1, QChar('0')).toStdString());
        VHW2_nmea.set(8, "K");
        nmea.push_back(QString::fromStdString(VHW2_nmea.get_string()));

        HDG_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_Heading_M->value(), 6, 'f', 2, QChar('0')).toStdString());
        HDG_nmea.set(2, QString("%1").arg(ui->doubleSpinBox_Magnetic_Deviation->value(), 6, 'f', 2, QChar('0')).toStdString());
        HDG_nmea.set(3, (ui->radioButton_Deviation_E->isChecked() ? "E" : ui->radioButton_Deviation_W->isChecked() ? "W" : "E"));
        HDG_nmea.set(4, QString("%1").arg(ui->doubleSpinBox_Magnetic_Variation->value(), 6, 'f', 2, QChar('0')).toStdString());
        HDG_nmea.set(5, (ui->radioButton_Variation_E->isChecked() ? "E" : ui->radioButton_Variation_W->isChecked() ? "W" : "E"));
        nmea.push_back(QString::fromStdString(HDG_nmea.get_string()));
    }
    return nmea;
}
