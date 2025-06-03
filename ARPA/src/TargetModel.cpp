
#include "TargetModel.h"
#include "NaviStrings.h"

#include <QSpinBox>
#include "CoordinateEdit.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include "nmea.h"
#include <QObject>
#include <QAbstractItemDelegate>
#include <QRandomGenerator>
#include "TableDelegate.h"



// ///==========================================================///==========================================================///==========================================================



TargetModel::TargetModel(QObject* parent ) : QStandardItemModel(parent) {
    setHorizontalHeaderLabels({"№", "Название","Широта", "Долгота", "Пеленг", "Дистанция", "Скорость", "Курс","Dкр", "tкр", "Статус", "Сопровождение","Отправка"});
    currentRow = QModelIndex();
}
TargetModel::~TargetModel(){

}
void TargetModel::addTarget(){
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    countTargets++;
    QList<QStandardItem*> rowItems;
    for (int col = 0; col < this->columnCount(); ++col) {
        rowItems.append(new QStandardItem());
    }
    rowItems[ColumnType::Number]->setData(countTargets, Qt::DisplayRole);
    rowItems[ColumnType::Reference]->setData(Qt::Unchecked, Qt::CheckStateRole);
    rowItems[ColumnType::Enabled]->setData(Qt::Checked, Qt::CheckStateRole);
    appendRow(rowItems);
    endInsertRows();
}
QVariant TargetModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid() ) {
        return QStandardItemModel::data(index,role );
    }
    if (role == Qt::TextAlignmentRole) {
        return QVariant::fromValue(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    int col = index.column();
    ColumnType tCol = (ColumnType)col;

    if (role == Qt::UserRole + 2) {
        if(suffix.contains(tCol)) return suffix.value(tCol);
        switch (tCol) {
            case Status: return QStringList({"Потерянная","Обнаруженая","Слежение"});  
            default: return QStandardItemModel::data(index,role );
        }
    }
    if (role == Qt::UserRole + 3) {
        switch (tCol) {
            case Bearing:
            case Course: 
            case Distance: 
            case Speed:
            case Tkr:  
            case Dkr:  
                return 0.0;
            default: return QStandardItemModel::data(index,role );
        }
    }
    if (role == Qt::UserRole + 4) {
        switch (tCol) {
            case Bearing:
            case Course: 
                return 360.0;
            case Distance: 
            case Speed:
            case Tkr:  
            case Dkr:  
                return 2000;
            default: return QStandardItemModel::data(index,role );
        }
    }

    if (role == Qt::UserRole + 1) {
        using ColumnType = TableWidgetsDelegate::ColumnType;
        switch (tCol) {
            case NameTarget: return QVariant::fromValue(ColumnType::LineEdit);       
        //     case 1: return QVariant::fromValue(ColumnType::);     
            case lat: return QVariant::fromValue(ColumnType::LatEdit);    
            case lon: return QVariant::fromValue(ColumnType::LongEdit);      
            case Bearing: return QVariant::fromValue(ColumnType::DoubleSpinBox);    
            case Distance: return QVariant::fromValue(ColumnType::DoubleSpinBox); 
            case Speed: return QVariant::fromValue(ColumnType::DoubleSpinBox);   
            case Course: return QVariant::fromValue(ColumnType::DoubleSpinBox);   
            case Dkr: return QVariant::fromValue(ColumnType::DoubleSpinBox);   
            case Tkr: return QVariant::fromValue(ColumnType::DoubleSpinBox);  
            case Status: return QVariant::fromValue(ColumnType::ComboBox);   
            case Reference: return QVariant::fromValue(ColumnType::Checkbox);  
            case Enabled: return QVariant::fromValue(ColumnType::Checkbox);  
            default: return QStandardItemModel::data(index,role );
        }
    }
    if (role == Qt::DisplayRole) {
        switch (tCol) {
            case lat: return geoLat(data(index,Qt::EditRole).toDouble());    
            case lon: return geoLon(data(index,Qt::EditRole).toDouble());   
            case Bearing:    
            case Distance: 
            case Course: 
            case Speed: 
            case Tkr: 
            case Dkr: 
               return QString::number(data(index,Qt::EditRole).toDouble(),'f', 2) + suffix.value(tCol);
            case Status:{
                int indexCom = data(index,Qt::EditRole).toInt();
                auto str=  QStringList({"Потерянная","Обнаруженая","Слежение"});
                return str.at(indexCom);
            }      
        default: return QStandardItemModel::data(index,role );
        }
    }

    return QStandardItemModel::data(index,role );
}
Qt::ItemFlags TargetModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    int column = index.column();
    ColumnType tCol = (ColumnType)column;

    if(tCol == ColumnType::Number) return defaultFlags;
    else if(tCol == ColumnType::Reference || tCol == ColumnType::Enabled) return defaultFlags | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    else return defaultFlags | Qt::ItemIsEditable;
    
    return QStandardItemModel::flags(index);
}
void TargetModel::updateData(){
    for (int row = 0; row < rowCount(); ++row) {
        if(item(row, ColumnType::Enabled)->data(Qt::CheckStateRole) == Qt::Unchecked ) continue;
        double targetLatitude = item(row, ColumnType::lat)->data(Qt::EditRole).toDouble();
        double targetLongitude = item(row, ColumnType::lon)->data(Qt::EditRole).toDouble();
        double speed = item(row, ColumnType::Speed)->data(Qt::EditRole).toDouble(); // в узлах
        double course = item(row, ColumnType::Course)->data(Qt::EditRole).toDouble(); // в градусах

        int deltaCOT = QRandomGenerator::global()->bounded(-5, 6);
        int deltaSOG = QRandomGenerator::global()->bounded(-5, 6);

        course += deltaCOT;
        if (course < 0) course += 360;
        if (course>= 360) course -= 360;

        // Обновляем SOG и обеспечиваем, что он остается неотрицательным
        speed += deltaSOG;
        if (speed < 0) speed = 0;

        QPair<double, double> newPosition = calculateNewPosition(targetLatitude, targetLongitude, course,speed* 0.51444444444);

        // Обновляем широту и долготу цели
        item(row, ColumnType::lat)->setData(newPosition.first, Qt::EditRole);
        item(row, ColumnType::lon)->setData(newPosition.second, Qt::EditRole);
       
        item(row, ColumnType::Speed)->setData(speed, Qt::EditRole);
        item(row, ColumnType::Course)->setData(course, Qt::EditRole);
      
        double bearing = calculateBearing(posShip.lat, posShip.lon, targetLatitude, targetLongitude);
        double distance = calculateDistance(posShip.lat, posShip.lon, targetLatitude, targetLongitude);

        item(row, ColumnType::Distance)->setData(distance, Qt::EditRole);
        item(row, ColumnType::Bearing)->setData(bearing, Qt::EditRole);
    }
}

QStringList TargetModel::getNMEA(){
    if(rowCount() == 0) return QStringList();
    QStringList  nmea;
    if(simulationActive) updateData();
    for (int row = 0; row < rowCount(); ++row) {
        if(item(row, ColumnType::Enabled)->data(Qt::CheckStateRole) == Qt::Unchecked ) continue;

        Nmea TLL = Nmea("RATLL" , {"00","0000.00","N","00000.00","W","","000000.00","T",""});
        TLL.set(1, QString("%1").arg(item(row, ColumnType::Number)->data(Qt::DisplayRole).toInt(), 2, 10, QChar('0')).toStdString());
        
        // Широта Долгота
        double lat = item(row, ColumnType::lat)->data(Qt::EditRole).toDouble();
        double lon = item(row, ColumnType::lon)->data(Qt::EditRole).toDouble();
        std::string latString, latDirection, lonString, lonDirection;
        helpFuncNmea::formatLatitude(lat, latString, latDirection);
        helpFuncNmea::formatLongitude(lon, lonString, lonDirection);

        TLL.set(2, latString);
        TLL.set(3, latDirection);

        TLL.set(4, lonString);
        TLL.set(5, lonDirection);
        
        TLL.set(6,  item(row, ColumnType::NameTarget)->data(Qt::DisplayRole).toString().toStdString());
        TLL.set(8,  status.value(item(row, ColumnType::Status)->data(Qt::EditRole).toInt()).toStdString());

        nmea.push_back(QString::fromStdString( TLL.get_string()));

        Nmea TTM = Nmea("RATTM" , {"00","0.0","0.0","T","0.0","0.0","T","0.0","0.0","N","","T","","000000.00","A"});
        TTM.set(1, QString("%1").arg(item(row, ColumnType::Number)->data(Qt::DisplayRole).toInt(), 2, 10, QChar('0')).toStdString());

        TTM.set(2, QString("%1").arg(item(row, ColumnType::Distance)->data(Qt::EditRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(3, QString("%1").arg(item(row, ColumnType::Bearing)->data(Qt::EditRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(5, QString("%1").arg(item(row, ColumnType::Speed)->data(Qt::EditRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(6, QString("%1").arg(item(row, ColumnType::Course)->data(Qt::EditRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(8, QString("%1").arg(item(row, ColumnType::Dkr)->data(Qt::EditRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(9, QString("%1").arg(item(row, ColumnType::Tkr)->data(Qt::EditRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(11,  item(row, ColumnType::NameTarget)->data(Qt::DisplayRole).toString().toStdString());
        TTM.set(12,  status.value(item(row, ColumnType::Status)->data(Qt::EditRole).toInt()).toStdString());
        TTM.set(13, item(row, ColumnType::Reference)->data(Qt::CheckStateRole) == Qt::Checked ? "R" : "" );
        nmea.push_back(QString::fromStdString( TTM.get_string()));
    }   
    return nmea;
}

void TargetModel::setPosShip(PointGeo pos){
    posShip = pos;
}

void TargetModel::setSimulationStatus(bool status){
    simulationActive = status;
}
void TargetModel::setPos(double lat, double lon){
    if (currentRow.isValid()) {
        item(currentRow.row(), ColumnType::lat)->setData(lat, Qt::EditRole);
        item(currentRow.row(), ColumnType::lon)->setData(lon, Qt::EditRole);
    }
}

void TargetModel::rowSelect(const QModelIndex &current, const QModelIndex &previous){
    currentRow = current;
}