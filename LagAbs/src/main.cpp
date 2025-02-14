
#include <QApplication>
#include "LagAbs.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  
    LagAbs * w = new LagAbs();
    w->show();
    return app.exec();

}