// Ручной запуск плагина: PluginHost <путь к плагину>. Показывает окно и печатает NMEA.
#include <QApplication>
#include <QPluginLoader>
#include <QDebug>
#include "BaseNaviWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (argc < 2) {
        qCritical() << "usage: PluginHost <plugin>";
        return 1;
    }
    QPluginLoader loader(QString::fromLocal8Bit(argv[1]));
    auto *w = qobject_cast<BaseNaviWidget *>(loader.instance());
    if (!w) {
        qCritical() << loader.errorString();
        return 1;
    }
    QObject::connect(w, &BaseNaviWidget::sendData, [](QStringList data) {
        for (const QString &s : data) qInfo().noquote() << s.trimmed();
    });
    w->show();
    w->startSend();
    return app.exec();
}
