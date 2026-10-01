#pragma once

#include "BaseAISSimulator.h"

class QComboBox;
class QLineEdit;
class QSpinBox;

// Тип 24: статические данные класса B (части A и B отправляются парой)
class Type24Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type24Simulator(QWidget *parent = nullptr);
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
private:
    QLineEdit *m_name;
    QComboBox *m_shipType;
    QLineEdit *m_callSign;
    QLineEdit *m_vendor;
    QSpinBox *m_model;
    QSpinBox *m_serial;
    QSpinBox *m_bow;
    QSpinBox *m_stern;
    QSpinBox *m_port;
    QSpinBox *m_starboard;
    QSpinBox *m_mothership;
    QSpinBox *m_interval;
};
