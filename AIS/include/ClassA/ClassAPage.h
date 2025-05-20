#pragma once

#include <QWidget>
#include "BaseAisPage.h"
#include "BaseAISSimulator.h"

namespace Ui { class ClassAPage; }

class ClassAPage : public BaseAisPage{
    Q_OBJECT

public:
    ClassAPage(QWidget *parent);
    ~ClassAPage();
    virtual QStringList getData() override;

protected:
    virtual std::unique_ptr<BaseParamClassAis> createParam() const override;

private:
    Ui::ClassAPage *ui;
    BaseAISSimulator * type123 = nullptr;
    BaseAISSimulator * type5 = nullptr;
    int deltaTimeSec = 0;
};

