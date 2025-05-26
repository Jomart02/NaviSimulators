#pragma once

#include <QWidget>
#include <QTcpSocket>

namespace Ui { class RemoteAis; }

class RemoteAis : public QWidget{
    Q_OBJECT

public:
    RemoteAis(QWidget *parent);
    ~RemoteAis();
    void setActiveSend(bool active);
signals:
    void dataResived(const QStringList &messages);
private slots:
    void receiveData();
private:
    QStringList extractNMEASentences(const QString& input);

    Ui::RemoteAis *ui;
    bool activeSend = false;
    QTcpSocket *socket = nullptr;
};

