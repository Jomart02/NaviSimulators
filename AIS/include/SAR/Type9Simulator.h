#pragma once

#include "BaseAISSimulator.h"

namespace Ui { class Type9Simulator; }

class Type9Simulator : public BaseAISSimulator
{
	Q_OBJECT

public:
	explicit Type9Simulator(QWidget *parent = nullptr);
	~Type9Simulator();
	virtual QVariant getData() override;
	virtual void setData(QVariant data) override;
	virtual void clearParam() override;

public slots:
	virtual void updateAisData(QStringList &aisMess) override;

private:
	Ui::Type9Simulator* ui;
};
