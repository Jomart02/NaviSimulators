#include "EchoSounder.h"
#include "ui_EchoSounder.h"

EchoSounder::EchoSounder(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::EchoSounder),
    DBT_nmea("SDDBT",{"0.0","f","0.0","M","0.0","F"}),
    DPT_nmea("SDDPT",{"0.0","0.0"}),
    DPL_nmea("SDDPL",{"0.0"})
{

    ui->setupUi(this);
    #ifdef BUILD_TEST
        startSend();
    #endif
}

EchoSounder::~EchoSounder()
{
    delete ui;
}
QIcon EchoSounder::icon() const {
    return QIcon();
}
QString EchoSounder::name() const {
    return tr("EchoSounder");
}
QString EchoSounder::description() const {
    return QString("");
}

QStringList EchoSounder::getNavigationData(){
    QStringList nmea;
    if (ui->radioButton_Echo->isChecked()) {
        DBT_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_depth->value() * 3.28084, 3, 'f', 1, QChar('0')).toStdString());
        DBT_nmea.set(2, "f");
        DBT_nmea.set(3, QString("%1").arg(ui->doubleSpinBox_depth->value(), 3, 'f', 1, QChar('0')).toStdString());
        DBT_nmea.set(4, "M");
        DBT_nmea.set(5, QString("%1").arg(ui->doubleSpinBox_depth->value() * 0.546807, 3, 'f', 1, QChar('0')).toStdString());
        DBT_nmea.set(6, "F");
        nmea.push_back(QString::fromStdString(DBT_nmea.get_string()));

        DPT_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_depth->value(), 3, 'f', 1, QChar('0')).toStdString());
        DPT_nmea.set(2, QString("%1").arg(ui->doubleSpinBox_offset->value(), 3, 'f', 1, QChar('0')).toStdString());
        nmea.push_back(QString::fromStdString(DPT_nmea.get_string()));
    } else if (ui->radioButton_Depth->isChecked()) {
        DPL_nmea.set(1, QString("%1").arg(ui->doubleSpinBox_depth->value(), 3, 'f', 1, QChar('0')).toStdString());
        nmea.push_back(QString::fromStdString(DPL_nmea.get_string()));
    }


    return nmea;
}

