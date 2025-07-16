#pragma once

#include <QWidget>
#include "BaseAisPage.h"
#include "BaseAISSimulator.h"
#include "AisStructures.h"
using namespace AIS_NMEA_Builder;

namespace Ui { class ClassAPage; }

class ClassAPage : public BaseAisPage{
    Q_OBJECT

public:
    ClassAPage(QWidget *parent);
    ~ClassAPage();
    virtual QStringList getData() override;

protected:
    virtual std::unique_ptr<BaseParamClassAis> createParam() const override;
    virtual void swapTarget(unsigned int prevmmsi,unsigned int mmsi) override;
private:
    void processClassA123(ParamClassA* param, Type123Decoder& dec, QStringList& messages, bool isCurrent, bool isManual,unsigned int number);
    void processClassA5(ParamClassA* param, Type5Decoder& dec5, QStringList& messages, int deltaTimeSec, bool isCurrent, bool isManual,unsigned int number);
private:
    Ui::ClassAPage *ui;
    BaseAISSimulator * type123 = nullptr;
    BaseAISSimulator * type5 = nullptr;
    int deltaTimeSec = 0;
};

