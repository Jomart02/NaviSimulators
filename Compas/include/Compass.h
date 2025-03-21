#pragma once
#include "BaseNaviWidget.h"  

namespace Ui
{
class Compass;
}

class Compass : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit Compass(QWidget *parent = nullptr);
    ~Compass() ;
    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
protected:
    virtual QString getRetranslateName(QString retranslateName) override;
    virtual void retranslate() override;
private:
    Ui::Compass *ui; 

    Nmea VHW_nmea;
    Nmea HDT_nmea;
    Nmea THS_nmea;

    Nmea VHW2_nmea;
    Nmea HDG_nmea;
    QTranslator *translator;
};
