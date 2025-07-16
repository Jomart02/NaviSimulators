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
    virtual void boxIndexChange(int index);
    virtual void activeChange(bool flag);
protected:
    void setComboBoxMMSI(QComboBox* box);
    void setButtonAdd(QPushButton* add);
    void setCheckBoxManual(QCheckBox* manual);
    void setCheckBoxActive(QCheckBox* active);
    virtual std::unique_ptr<BaseParamClassAis> createParam() const = 0;
    virtual void swapTarget(unsigned int prevmmsi ,unsigned int mmsi) = 0;
protected:
    int previousIndex = -1;
    bool m_sending = false;
    QComboBox* m_comboBox_NumbersMMSI = nullptr;
    std::map<unsigned int, std::unique_ptr<BaseParamClassAis>> paramsShip;
    QCheckBox *m_active = nullptr; 
};