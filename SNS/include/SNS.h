#pragma once

#include <QWidget>
#include "ui_SNS.h"
#include <QThread>
#include <QCheckBox>
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
    ~SNS() override;

private slots:

public slots:
    virtual void setText() override;
signals:


private:
    Ui::SNS *ui;
  
};
