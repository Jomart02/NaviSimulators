
#include <QApplication>
#include "Weather.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  
    Weather * w = new Weather();
    w->show();
    return app.exec();

}