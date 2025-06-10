#pragma once

#include "BaseAISSimulator.h"

namespace Ui
{
	class Type18Simulator;
}

class Type18Simulator : public BaseAISSimulator
{
	Q_OBJECT

public:
	Type18Simulator(QWidget *parent = nullptr);
	~Type18Simulator();

private:
	Ui::Type18Simulator *ui;
};
