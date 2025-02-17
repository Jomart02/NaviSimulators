#pragma once
#include "BaseNaviWidget.h"  

#include "TargetModel.h"

namespace Ui
{
class ARPA;
}

class ARPA : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit ARPA(QWidget *parent = nullptr);
    ~ARPA() ;
    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
private slots:
    void updatePositionShip(double v);
private:
    Ui::ARPA *ui; 
     
    unsigned int tick_interval = 1000;
    TargetModel *modelT = nullptr;
};
