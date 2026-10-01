#pragma once

#include "BaseAISSimulator.h"

class QLineEdit;
class QSpinBox;

// Тип 14: широковещательное сообщение о безопасности (текст)
class Type14Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type14Simulator(QWidget *parent = nullptr);
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
signals:
    void sendRequested();
private:
    QLineEdit *m_text;
    QSpinBox *m_interval;
};
