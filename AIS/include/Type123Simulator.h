#pragma once

#include "ui_Type123Simulator.h"

namespace Ui
{
class Type123Simulator;
}

class Type123Simulator : public QWidget
{
    Q_OBJECT

public:
    explicit Type123Simulator(QWidget *parent = nullptr);
    ~Type123Simulator();

private:
    Ui::Type123Simulator *ui;
};
