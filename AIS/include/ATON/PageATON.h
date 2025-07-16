#pragma once

#include <QWidget>
#include "BaseAisPage.h"
#include "BaseAISSimulator.h"
#include "AisStructures.h"

using namespace AIS_NMEA_Builder;

namespace Ui { class PageATON; }

class PageATON : public BaseAisPage
{
	Q_OBJECT

public:
	PageATON(QWidget *parent);
	~PageATON();
	virtual QStringList getData() override;

protected:
	virtual std::unique_ptr<BaseParamClassAis> createParam() const override;
	virtual void swapTarget(unsigned int prevmmsi,unsigned int mmsi) override;
private:
	void processClassAton21(ParamATON* param, Type21Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number);
private:
	Ui::PageATON* ui;
	BaseAISSimulator* type21 = nullptr;
};
