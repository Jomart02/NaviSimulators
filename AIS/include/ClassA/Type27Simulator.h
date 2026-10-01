#pragma once

#include "BaseAISSimulator.h"

class QComboBox;
class QSpinBox;

// Тип 27: сообщение дальнего действия. Позиция, курс, скорость, точность, RAIM и статус
// берутся из сообщения 1-3 того же судна, здесь только параметры самого типа 27.
class Type27Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type27Simulator(QWidget *parent = nullptr);
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
private:
    QComboBox *m_gnss;
    QSpinBox *m_interval;
};
