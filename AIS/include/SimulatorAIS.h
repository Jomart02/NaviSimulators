#pragma once
#include <QWidget>
#include "BaseNaviWidget.h"  
#include "BaseAisPage.h"
namespace Ui
{
class SimulatorAIS;
}


class SimulatorAIS : public BaseNaviWidget
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit SimulatorAIS(QWidget *parent = nullptr);
    ~SimulatorAIS();

    virtual bool isActive() override;
    virtual void startSend() override;
    virtual void stopSend() override;

    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;
private slots:
    void sendTypeA();
    void sendTypeB();
protected slots:
    virtual QStringList getNavigationData() override;
private:
    Ui::SimulatorAIS *ui;

    QTimer *timerClassA = nullptr;
    QTimer *timerClassB = nullptr;
    BaseAisPage *classAPage = nullptr;
    BaseAisPage *classBPage = nullptr;//16
};
