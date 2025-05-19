#pragma once


#include "BaseNaviWidget.h"  
#include "Type123Simulator.h"
#include "Type5Simulator.h"

namespace Ui
{
class SimulatorAIS;
}
using namespace AIS_Data_Type;

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
    void addNewClassA();
    void updateType5();

    void sendTypeA();
protected slots:
    virtual QStringList getNavigationData() override;
private:
    Ui::SimulatorAIS *ui;
    Type123Simulator * type123 = nullptr;
    Type5Simulator * type5 = nullptr;
    std::map<unsigned int, ParamClassA> paramsShip;
    bool sending = false;
    int deltaTimeSec = 0;

    QTimer *timerClassA = nullptr;
};
