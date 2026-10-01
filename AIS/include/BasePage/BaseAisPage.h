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

signals:
    /// @brief Сообщения, которые нужно отправить сразу (по кнопке), не дожидаясь таймера
    void sendNow(QStringList messages);

protected slots:
    void addNewTargetClass();
    virtual void boxIndexChange(int index);
    virtual void activeChange(bool flag);
protected:
    /// @brief true, когда прошёл период interval секунд (вызывается раз в секунду); interval <= 0 - никогда
    static bool due(int interval, int &elapsed);
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