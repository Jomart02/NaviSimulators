#pragma once

#include "BaseAISSimulator.h"
#include "AisDictionaries.h"

namespace Ui
{
	class Type19Simulator;
}
class Type19Simulator : public BaseAISSimulator
{
	Q_OBJECT

public:
	explicit Type19Simulator(QWidget *parent = nullptr);
	~Type19Simulator();
	virtual QVariant getData() override;
	virtual void setData(QVariant data) override;
    virtual void clearParam() override;

public slots:
    virtual void updateAisData(QStringList &aisMess) override;

private:
	void init();


    private:



	const QList<AIS_Dict::Item> &shipTypes = AIS_Dict::shipTypes();

	const QList<AIS_Dict::Item> &posTypes = AIS_Dict::posTypes();

	Ui::Type19Simulator *ui;
};
