#pragma once

#include <QWidget>
#include "BaseAisPage.h"
#include "BaseAISSimulator.h"
#include "AisStructures.h"

using namespace AIS_NMEA_Builder;

class Type4Simulator;
class Type14Simulator;

// Базовая станция: типы 4 (отчёт), 11 (ответ на запрос UTC) и 14 (текст о безопасности)
class PageBaseStation : public BaseAisPage
{
    Q_OBJECT

public:
    PageBaseStation(QWidget *parent);
    virtual QStringList getData() override;

protected:
    virtual std::unique_ptr<BaseParamClassAis> createParam() const override;
    virtual void swapTarget(unsigned int prevmmsi, unsigned int mmsi) override;
private:
    template<class Decoder, class Data>
    void sendOnce(BaseAISSimulator *simulator);

    BaseAISSimulator *type4 = nullptr;
    BaseAISSimulator *type11 = nullptr;
    BaseAISSimulator *type14 = nullptr;
};
