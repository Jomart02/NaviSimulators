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
	explicit Type18Simulator(QWidget* parent = nullptr);
	~Type18Simulator();
	virtual QVariant getData() override;
	virtual void setData(QVariant data) override;
	virtual void clearParam() override;
public slots:
	virtual void updateAisData(QStringList &aisMess) override;





private:
	
	Ui::Type18Simulator* ui;
};
