#pragma once
#include "BaseNaviWidget.h"

namespace Ui
{
class Weather;
}

class Weather : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    Weather(QWidget *parent = nullptr);
    ~Weather() ;
    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
private slots:

    void calcAngleT(double v);
    void calcAngleR(double v);
private:
    Ui::Weather *ui;

    Nmea MWV_nmea;
    Nmea MWD_nmea;
    Nmea MDA_nmea;

    Nmea VWR_nmea;
    Nmea VWT_nmea;


};
