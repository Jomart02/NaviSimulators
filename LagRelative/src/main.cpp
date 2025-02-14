
#include <QApplication>
#include "LagRel.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  
    LagRel * w = new LagRel();
    w->show();
    return app.exec();

}