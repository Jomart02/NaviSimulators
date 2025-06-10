#pragma once

#include "BaseAISSimulator.h"
namespace Ui
{
	class Type19Simulator;
}
class Type19Simulator : public QWidget
{
	Q_OBJECT

public:
	Type19Simulator(QWidget *parent = nullptr);
	~Type19Simulator();

private:
	Ui::Type19Simulator *ui;
};
