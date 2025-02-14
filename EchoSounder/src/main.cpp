
#include <QApplication>
#include "EchoSounder.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  
    EchoSounder * w = new EchoSounder();
    w->show();
    return app.exec();

}