#pragma once

#include <QList>
#include <QWidget>
#include "BaseAisPage.h"
#include "AisStructures.h"

class FormSimulator;

// Остальные типы сообщений (6, 7, 8, 10, 12, 13, 15, 16, 17, 20, 22, 23, 25, 26):
// формы собираются по описанию AIS_Messages, отправка по кнопке или с периодом
class PageOther : public BaseAisPage
{
    Q_OBJECT

public:
    PageOther(QWidget *parent);
    virtual QStringList getData() override;

protected:
    virtual std::unique_ptr<BaseParamClassAis> createParam() const override;
    virtual void swapTarget(unsigned int prevmmsi, unsigned int mmsi) override;
private:
    void storeForms(ParamOther *param);

    QList<FormSimulator *> forms;
    QList<int> types; // тип сообщения для каждой формы
};
