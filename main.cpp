
#include <QApplication>
#include <QWidget>
#include "BaseNaviWidget.h"
#include "SimulatorAIS.h"
#include "SNS.h"
#include <QObject>
//для теста заменить SNS ... = new SNS(nullptr) на SimulatorAIS
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    SNS * c = new SNS(nullptr);
    c->show();

    QObject::connect(c, &BaseNaviWidget::sendData,[](QStringList data){
        qDebug() << data;
    });
    c->startSend();
    return app.exec();

}

