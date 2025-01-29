#pragma once 
#include <QWidget>
#include "AisStructures.h"
class BaseAISSimulator : public QWidget
{
    Q_OBJECT
public:
    explicit BaseAISSimulator(QWidget *parent = nullptr): QWidget(parent){}
    ~BaseAISSimulator(){}
    virtual QVariant getData() = 0;
    virtual void setData(QVariant data) = 0;
public slots:
    virtual void updateAisData(QStringList &aisMess) = 0;

};