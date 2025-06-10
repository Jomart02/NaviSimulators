#pragma once

#include <QWidget>
#include "BaseAisPage.h"
#include "BaseAISSimulator.h"
#include "AisStructures.h"

using namespace AIS_NMEA_Builder;

namespace Ui { class ClassBPage; }

class ClassBPage : public BaseAisPage
{
	Q_OBJECT

public:
	ClassBPage(QWidget *parent);
	~ClassBPage();
	//virtual QStringList getData() override;

private:
	Ui::ClassBPage* ui;
};
