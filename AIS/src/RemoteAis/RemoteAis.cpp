#include "RemoteAis.h"
#include "ui_RemoteAis.h"
#include <QRegularExpressionMatchIterator>


RemoteAis::RemoteAis(QWidget *parent) : QWidget(parent) , ui(new Ui::RemoteAis){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    
    ui->label_4->setText("Unconnected");
    ui->label_4->setStyleSheet("color: red");
    ui->lineEdit_IP->setText("153.44.253.27");
    ui->lineEdit_port->setText("5631");

    socket->connectToHost("153.44.253.27", 5631);
    
    // Handle connection success
    connect(socket, &QTcpSocket::connected, [&]() {
        ui->label_4->setStyleSheet("color: green");
        ui->label_4->setText("Connected");
    });
    
    // Handle incoming data
    connect(socket, &QTcpSocket::readyRead,  this, &RemoteAis::receiveData);
    
 
}

RemoteAis::~RemoteAis()
{
    delete ui;
}
void RemoteAis::setActiveSend(bool active){
    activeSend = active;
}


void RemoteAis::receiveData(){
    if(!activeSend) return;
    QByteArray data = socket->readAll();
    QString dataStr = QString::fromUtf8(data);

    QStringList nmeaList = extractNMEASentences(dataStr);
    emit dataResived(nmeaList);
}

QStringList RemoteAis::extractNMEASentences(const QString& input) {
    QStringList nmeaList;

    QStringList lines = input.split(QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        QString nmeaSentence;
        if (line.contains("!BSVDM") || line.contains("!B1VDM") || line.contains("!B2VDM")) {
            int nmeaStart = line.indexOf("!");
            if (nmeaStart >= 0) {
                nmeaSentence = line.mid(nmeaStart);             
            } else {
                nmeaSentence = line;             }
            if (!nmeaSentence.endsWith("\r\n")) {
                nmeaSentence += "\r\n";
            }
            if ((nmeaSentence.startsWith("!BSVDM") || 
                 nmeaSentence.startsWith("!B1VDM") || 
                 nmeaSentence.startsWith("!B2VDM")) && 
                !nmeaList.contains(nmeaSentence)) {
                nmeaList.append(nmeaSentence);
            }
        }
    }

    QRegularExpression re("![B1-2S]VDM,[^\\r\\n]*\\r\\n");
    QRegularExpressionMatchIterator it = re.globalMatch(input);
    while (it.hasNext()) {
        QString sentence = it.next().captured(); 
        if (!nmeaList.contains(sentence)) {
            nmeaList.append(sentence);
        }
    }

    return nmeaList;
}
