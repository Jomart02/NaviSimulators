#pragma once
#include "BaseNaviWidget.h"

namespace Ui
{
class LagRel;
}

class LagRel : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit LagRel(QWidget *parent = nullptr);
    ~LagRel() ;

    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
private:
    Ui::LagRel *ui;
    Nmea VHW_nmea;
    Nmea VBW_nmea;
};
