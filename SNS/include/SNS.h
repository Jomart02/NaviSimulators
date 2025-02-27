#pragma once

#include <QWidget>
#include "BaseNaviWidget.h"  


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
private:
    Ui::SNS *ui;
    Nmea rmc_nmea;
    Nmea vtg_nmea;
    Nmea zda_nmea;
    Nmea gga_nmea;
    Nmea gll_nmea;
};
