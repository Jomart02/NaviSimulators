
#include <QApplication>
#include "SimulatorAIS.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  
    SimulatorAIS * w = new SimulatorAIS();
    w->show();
    return app.exec();

}