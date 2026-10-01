#pragma once

#include "BaseAISSimulator.h"

class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QSpinBox;
class LatitudeEdit;
class LongitudeEdit;

// Тип 4 (отчёт базовой станции) и тип 11 (ответ на запрос UTC и даты): одинаковый формат
class Type4Simulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit Type4Simulator(bool response, QWidget *parent = nullptr);
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
signals:
    void sendRequested();
private:
    bool m_response;
    QCheckBox *m_systemTime;
    QDateTimeEdit *m_utc;
    LatitudeEdit *m_lat;
    LongitudeEdit *m_lon;
    QComboBox *m_accuracy;
    QComboBox *m_epfd;
    QComboBox *m_raim;
    QSpinBox *m_interval;
};
