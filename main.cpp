
#include <QApplication>
#include <QWidget>
#include "BaseNaviWidget.h"
#include "SNS.h"
#include "Compass.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    
    SNS * w = new SNS(nullptr);
    w->show();

    Compass * c = new Compass(nullptr);
    c->show();
    
    return app.exec();

}

