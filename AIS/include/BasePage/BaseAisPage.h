#pragma once 
#include <QWidget>
#include "AisStructures.h"
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

using namespace AIS_Data_Type;

class BaseAisPage : public QWidget
{
    Q_OBJECT
public:
    explicit BaseAisPage(QWidget *parent = nullptr);
    virtual ~BaseAisPage() = default;
    virtual QStringList getData() = 0;

protected slots:
    void addNewTargetClass();
protected:
    void setComboBoxMMSI(QComboBox* box);
    void setButtonAdd(QPushButton* add);
    void setCheckBoxManual(QCheckBox* manual);
    virtual std::unique_ptr<BaseParamClassAis> createParam() const = 0;
protected:

    bool sending = false;
    QComboBox* comboBox_NumbersMMSI = nullptr;
    std::map<unsigned int, std::unique_ptr<BaseParamClassAis>> paramsShip;
};