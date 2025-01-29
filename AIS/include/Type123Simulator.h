#pragma once

#include "ui_Type123Simulator.h"
#include "BaseAISSimulator.h"
namespace Ui
{
class Type123Simulator;
}

class Type123Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type123Simulator(QWidget *parent = nullptr);
    ~Type123Simulator();
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
private:
    Ui::Type123Simulator *ui;
};
