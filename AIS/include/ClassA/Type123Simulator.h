#pragma once

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
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
private:
    Ui::Type123Simulator *ui;
};
