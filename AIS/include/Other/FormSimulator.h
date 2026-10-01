#pragma once

#include <QMap>
#include "AisMessages.h"
#include "BaseAISSimulator.h"

class QSpinBox;

// Форма сообщения, собранная по описанию AIS_Messages::MessageDef.
// getData()/setData() работают с QVariantMap (ключи - поля описания и "_interval").
class FormSimulator : public BaseAISSimulator
{
    Q_OBJECT

public:
    explicit FormSimulator(const AIS_Messages::MessageDef &def, QWidget *parent = nullptr);
    virtual QVariant getData() override;
    virtual void setData(QVariant data) override;
    virtual void clearParam() override;
public slots:
    virtual void updateAisData(QStringList &aisMess) override;
signals:
    void sendRequested();
private:
    AIS_Messages::MessageDef m_def;
    QMap<QString, QWidget *> m_widgets;
    QSpinBox *m_interval;
};
