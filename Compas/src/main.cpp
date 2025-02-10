
#include <QApplication>
#include "Compas.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  
    Compas * w = new Compas();
    w->show();
    return app.exec();

}