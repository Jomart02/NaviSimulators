#include <QWidget>

#include "BaseNaviWidget.h"  
#include "nmea.h"
namespace Ui
{
class EchoSounder;
}

class EchoSounder  : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    EchoSounder(QWidget *parent = nullptr);
    ~EchoSounder() ;
    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;

protected slots:
    virtual QStringList getNavigationData() override;
private:
    Ui::EchoSounder *ui;

    Nmea DBT_nmea;
    Nmea DPT_nmea;
    Nmea DPL_nmea;
};
