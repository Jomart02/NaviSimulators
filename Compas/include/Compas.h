#pragma once
#include <QWidget>
#include "BaseNaviWidget.h"  

namespace Ui
{
class Compas;
}

class Compas : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit Compas(QWidget *parent = nullptr);
    ~Compas() ;
    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
private slots:



private:
    Ui::Compas *ui; 
     
    unsigned int tick_interval = 1000;

    Nmea VHW_nmea;
    Nmea HDT_nmea;
    Nmea THS_nmea;

    Nmea VHW2_nmea;
    Nmea HDG_nmea;


};
