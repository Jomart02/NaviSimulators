
#include <QApplication>
#include <QWidget>
#include "BaseNaviWidget.h"
#include "SimulatorAIS.h"
#include "ARPA.h"
#include <QObject>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    SimulatorAIS * c = new SimulatorAIS(nullptr);
    c->show();

    QObject::connect(c, &BaseNaviWidget::sendData,[](QStringList data){
        qDebug() << data;
    });
    c->startSend();
    return app.exec();

}

