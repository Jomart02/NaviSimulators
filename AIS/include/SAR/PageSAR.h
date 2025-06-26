#pragma once

#include <QWidget>
#include "BaseAisPage.h"
#include "BaseAISSimulator.h"
#include "AisStructures.h"

using namespace AIS_NMEA_Builder;

namespace Ui { class PageSAR; }

class PageSAR : public BaseAisPage
{
	Q_OBJECT

public:
	PageSAR(QWidget *parent);
	~PageSAR();
	virtual QStringList getData() override;

protected:
	virtual std::unique_ptr<BaseParamClassAis> createParam() const override;
	virtual void swapTarget(int prevmmsi,unsigned int mmsi) override;
private:
	void processClassSar(ParamSAR* param, Type9Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number);
private:
	Ui::PageSAR* ui;
	BaseAISSimulator* type9 = nullptr;
};
