#pragma once

#include "ui_Type5Simulator.h"

namespace Ui
{
class Type5Simulator;
}

class Type5Simulator : public QWidget
{
    Q_OBJECT

public:
    explicit Type5Simulator(QWidget *parent = nullptr);
    ~Type5Simulator();

private:
    Ui::Type5Simulator *ui;
};
