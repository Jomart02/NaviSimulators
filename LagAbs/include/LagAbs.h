#pragma once
#include <QWidget>
#include "BaseNaviWidget.h"  

namespace Ui
{
class LagAbs;
}

class LagAbs: public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit LagAbs(QWidget *parent = nullptr);
    ~LagAbs() ;
    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
private:

    Ui::LagAbs *ui;
    Nmea VBW_nmea;
     
};
