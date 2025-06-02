#pragma once

#include <QWidget>
#include "BaseNaviWidget.h"  
#include "nmea.h"
#include <random>
namespace Ui
{
class SNS;
}

class SNS : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit SNS(QWidget *parent = nullptr);
    ~SNS();

    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;
public slots:
    virtual void setPos(double lat, double lon) override;
protected slots:
    virtual QStringList getNavigationData() override;
protected:
    virtual QString getRetranslateName(QString retranslateName) override;
    virtual void retranslate() override;
private:
    void updateCOGandSOG();
private:
    Ui::SNS *ui;
    Nmea rmc_nmea;
    Nmea vtg_nmea;
    Nmea zda_nmea;
    Nmea gga_nmea;
    Nmea gll_nmea;

    int freezeCounter = 0;        // счётчик заморозки
    int freezeDuration = 0;       // длительность заморозки
    std::default_random_engine generator;
    std::normal_distribution<double> cogChangeDist{0.0, 2.0};  // нормальное распределение для курса
    std::normal_distribution<double> sogChangeDist{0.0, 0.5};  // нормальное распределение для скорости
};
