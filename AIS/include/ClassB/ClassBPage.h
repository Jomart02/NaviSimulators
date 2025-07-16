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
	virtual QStringList getData() override;

private:
	void processClassB18(ParamClassB* param, Type18Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number);
	void processClassB19(ParamClassB* param, Type19Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number);
protected:
	virtual std::unique_ptr<BaseParamClassAis> createParam() const override;
	virtual void swapTarget(unsigned int prevmmsi,unsigned int mmsi) override;
private:
	Ui::ClassBPage* ui;
	BaseAISSimulator* type18 = nullptr;
	BaseAISSimulator* type19 = nullptr;
};
