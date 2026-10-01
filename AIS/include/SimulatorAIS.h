#pragma once
#include <QWidget>
#include <QList>
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

    virtual QIcon icon() const override;
    virtual QString name() const override;
    virtual QString description() const override;
protected slots:
    virtual QStringList getNavigationData() override;
private:
    void addPage(BaseAisPage *page, const QString &title);

    Ui::SimulatorAIS *ui;
    QList<BaseAisPage *> pages;
};
