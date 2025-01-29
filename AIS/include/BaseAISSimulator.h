#pragma once 
#include <QWidget>

class BaseAISSimulator : public QWidget
{
    Q_OBJECT
public:
    explicit BaseAISSimulator(QWidget *parent = nullptr): QWidget(parent){}
    ~BaseAISSimulator(){}
public slots:
    virtual void updateAisData(QStringList &aisMess) = 0;
};