
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

TargetDataDelegate::TargetDataDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *TargetDataDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    ColumnType type = columnType(index);
    switch (type) {
    case Number: // Не редактируется
        return nullptr;

    case NameTarget: { // QLineEdit с ограничением длины
        QLineEdit *editor = new QLineEdit(parent);
        editor->setMaxLength(20);
        QObject::connect(editor, &QLineEdit::textEdited, this, [=](const QString &text) {
            // Используем const_cast для вызова non-const метода
            const_cast<TargetDataDelegate*>(this)->commitData(editor);
        });
        return editor;
    }

    case lat: { // LatitudeEdit
        LatitudeEdit *editor = new LatitudeEdit(parent);
        QObject::connect(editor, QOverload<double>::of(&LatitudeEdit::valueChanged), this, [=](double value) {
            const_cast<TargetDataDelegate*>(this)->commitData(editor);
        });
        return editor;
    }

    case lon: { // LongitudeEdit
        LongitudeEdit *editor = new LongitudeEdit(parent);
        QObject::connect(editor, QOverload<double>::of(&LongitudeEdit::valueChanged), this, [=](double value) {
            const_cast<TargetDataDelegate*>(this)->commitData(editor);
        });
        return editor;
    }

    case Bearing:
    case Distance:
    case Speed:
    case Course:
    case Dkr:
    case Tkr: { // QDoubleSpinBox
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0); // Пример диапазона
        editor->setWrapping(true);
        if(type == Bearing || type ==  Course) editor->setMaximum(360);
        else editor->setMaximum(2000);

        if(suffix.contains(type)) editor->setSuffix(suffix.value(type));
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);

        editor->setDecimals(2);
        QObject::connect(editor, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            const_cast<TargetDataDelegate*>(this)->commitData(editor);
        });
        return editor;
    }

    case Status: { // QComboBox
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("Потерянная", "L");
        editor->addItem("Обнаруженая", "Q");
        editor->addItem("Слежение", "T");
        QObject::connect(editor, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index) {
            const_cast<TargetDataDelegate*>(this)->commitData(editor);
        });
        return editor;
    }

    case Reference: { // QCheckBox
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("Сопровождение", "R");
        editor->addItem("Нет", "");
        QObject::connect(editor, qOverload<int>(&QComboBox::currentIndexChanged), this, [=](int index) {
            const_cast<TargetDataDelegate*>(this)->commitData(editor);
        });
        return editor;
        // QCheckBox *editor = new QCheckBox(parent);
        // QObject::connect(editor, &QCheckBox::stateChanged, this, [=](int state) {
        //     const_cast<TargetDataDelegate*>(this)->commitData(editor);
        // });
        // return editor;
    }

    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void TargetDataDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    ColumnType type = columnType(index);

    switch (type) {
    case Number:
        break;

    case NameTarget: {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(index.model()->data(index, Qt::EditRole).toString());
        break;
    }

    case lat:{
        LatitudeEdit *latEdit = static_cast<LatitudeEdit*>(editor);
        latEdit->setValue(index.model()->data(index, Qt::UserRole).toDouble());
        break;
    }
    case lon: {
        LongitudeEdit *lonEdit = static_cast<LongitudeEdit*>(editor);
        lonEdit->setValue(index.model()->data(index, Qt::UserRole).toDouble());
        break;
    }

    case Bearing:
    case Distance:
    case Speed:
    case Course:
    case Dkr:
    case Tkr: {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(index.model()->data(index, Qt::UserRole).toDouble());
        break;
    }

    case Status: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
        break;
    }

    case Reference: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
        break;
    }

    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TargetDataDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    ColumnType type = columnType(index);
    switch (type) {
    case Number:
        break;

    case NameTarget: {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        model->setData(index, lineEdit->text(), Qt::EditRole);
        break;
    }

    case lat:{
        LatitudeEdit *latEdit = static_cast<LatitudeEdit*>(editor);
        model->setData(index, latEdit->value(), Qt::UserRole);
        model->setData(index, latEdit->text(), Qt::DisplayRole);
        break;
    }
    case lon: {
        LongitudeEdit *lonEdit = static_cast<LongitudeEdit*>(editor);
        model->setData(index, lonEdit->value(), Qt::UserRole);
        model->setData(index, lonEdit->text(), Qt::DisplayRole);
        break;
    }

    case Bearing:
    case Distance:
    case Speed:
    case Course:
    case Dkr:
    case Tkr: {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        QString suf ="";
        if(suffix.contains(type)) suf = suffix.value(type);
        model->setData(index, spinBox->value(), Qt::UserRole);
        model->setData(index, QString::number(spinBox->value()) + suf, Qt::DisplayRole);
        break;
    }

    case Status: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::DisplayRole);
        model->setData(index, comboBox->currentData(Qt::UserRole), Qt::UserRole);
        break;
    }

    case Reference: {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::DisplayRole);
        model->setData(index, comboBox->currentData(Qt::UserRole), Qt::UserRole);
        break;
    }

    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void TargetDataDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    ColumnType type = columnType(index);
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
//     if (type == Reference) {
//         // Центрируем редактор в ячейке
//         QRect rect = option.rect;
//         QSize sizeHint = editor->sizeHint();
//         editor->setGeometry(
//             rect.x() + (rect.width() - sizeHint.width()) / 2,
//             rect.y() + (rect.height() - sizeHint.height()) / 2,
//             sizeHint.width(),
//             sizeHint.height()
//         );
//     } else {
//         QStyledItemDelegate::updateEditorGeometry(editor, option, index);
//     }
// }
}
ColumnType TargetDataDelegate::columnType(const QModelIndex &index) const {
    // Предполагается, что тип колонки хранится в данных модели
    return static_cast<ColumnType>(index.column());
}

void TargetDataDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    ColumnType type = columnType(index);

    // if (type == Reference) { // Если это чекбокс
    //     QStyleOptionButton checkboxOption;
    //     checkboxOption.state = option.state | QStyle::State_Enabled;

    //     // Установка размера и положения чекбокса
    //     checkboxOption.rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxOption, nullptr);
    //     checkboxOption.rect.moveCenter(option.rect.center());

    //     // Установка состояния чекбокса
    //     bool isChecked = index.model()->data(index, Qt::EditRole).toBool();
    //     if (isChecked) {
    //         checkboxOption.state |= QStyle::State_On;
    //     } else {
    //         checkboxOption.state |= QStyle::State_Off;
    //     }

    //     // Рисуем чекбокс
    //     QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxOption, painter);
    // } else {
    //     // Для других типов используем стандартную отрисовку
    //     QStyledItemDelegate::paint(painter, option, index);
    // }
    QStyledItemDelegate::paint(painter, option, index);
}


///==========================================================///==========================================================///==========================================================



TargetModel::TargetModel(QObject* parent ) : QStandardItemModel(parent) {
    setHorizontalHeaderLabels({"№", "Название","Широта", "Долгота", "Пеленг", "Дистанция", "Скорость", "Курс","Dкр", "tкр", "Статус", "Сопровождение"});

}
TargetModel::~TargetModel(){

}
void TargetModel::addTarget(){
    countTargets++;
    QList<QStandardItem*> rowItems;
    for (int col = 0; col < this->columnCount(); ++col) {
        rowItems.append(new QStandardItem());
    }

    rowItems[ColumnType::Number]->setData(countTargets, Qt::DisplayRole);
    appendRow(rowItems);
}


void TargetModel::updateData(){
    for (int row = 0; row < rowCount(); ++row) {
        double targetLatitude = item(row, ColumnType::lat)->data(Qt::UserRole).toDouble();
        double targetLongitude = item(row, ColumnType::lon)->data(Qt::UserRole).toDouble();
        double speed = item(row, ColumnType::Speed)->data(Qt::UserRole).toDouble(); // в узлах
        double course = item(row, ColumnType::Course)->data(Qt::UserRole).toDouble(); // в градусах

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
        item(row, ColumnType::lat)->setData(newPosition.first, Qt::UserRole);
        item(row, ColumnType::lon)->setData(newPosition.second, Qt::UserRole);
        item(row, ColumnType::lat)->setData(geoLat(newPosition.first), Qt::DisplayRole);
        item(row, ColumnType::lon)->setData(geoLon(newPosition.second), Qt::DisplayRole);
        
        item(row, ColumnType::Speed)->setData(QString::number(speed) + suffix.value(ColumnType::Speed), Qt::DisplayRole);
        item(row, ColumnType::Speed)->setData(speed, Qt::UserRole);
        item(row, ColumnType::Course)->setData(course, Qt::UserRole);
        item(row, ColumnType::Course)->setData(QString::number(course) + suffix.value(ColumnType::Course), Qt::DisplayRole);

        double bearing = calculateBearing(posShip.lat, posShip.lon, targetLatitude, targetLongitude);
        double distance = calculateDistance(posShip.lat, posShip.lon, targetLatitude, targetLongitude);

        item(row, ColumnType::Distance)->setData(QString::number(distance) + suffix.value(ColumnType::Distance), Qt::DisplayRole);
        item(row, ColumnType::Distance)->setData(distance, Qt::UserRole);
        item(row, ColumnType::Bearing)->setData(bearing, Qt::UserRole);
        item(row, ColumnType::Bearing)->setData(QString::number(bearing) + suffix.value(ColumnType::Bearing), Qt::DisplayRole);
    }
}

QStringList TargetModel::getNMEA(){
    if(rowCount() == 0) return QStringList();
    QStringList  nmea;
    updateData();
    for (int row = 0; row < rowCount(); ++row) {
        Nmea TLL = Nmea("RATLL" , {"00","0000.00","N","00000.00","W","","000000.00","T",""});
        TLL.set(1, QString("%1").arg(item(row, ColumnType::Number)->data(Qt::DisplayRole).toInt(), 2, 10, QChar('0')).toStdString());
        
        // Широта
        double lat = item(row, ColumnType::lat)->data(Qt::UserRole).toDouble();
        QString latDeg = QString("%1").arg(qAbs(static_cast<int>(lat)), 2, 10, QChar('0'));
        QString latMin = QString("%1").arg(qAbs(fmod(lat * 60, 60)), 5, 'f', 2, QChar('0'));
        TLL.set(2, (latDeg + latMin).toStdString());
        TLL.set(3, lat >= 0 ? "N" : "S");

        // Долгота
        double lon = item(row, ColumnType::lon)->data(Qt::UserRole).toDouble();
        QString lonDeg = QString("%1").arg(qAbs(static_cast<int>(lon)), 3, 10, QChar('0'));
        QString lonMin = QString("%1").arg(qAbs(fmod(lon * 60, 60)), 5, 'f', 2, QChar('0'));
        TLL.set(4, (lonDeg + lonMin).toStdString());
        TLL.set(5, lon >= 0 ? "E" : "W");
        
        TLL.set(6,  item(row, ColumnType::NameTarget)->data(Qt::DisplayRole).toString().toStdString());
        if(!item(row, ColumnType::Status)->data(Qt::UserRole).toString().isEmpty())
            TLL.set(8,  item(row, ColumnType::Status)->data(Qt::UserRole).toString().toStdString());

        nmea.push_back(QString::fromStdString( TLL.get_string()));

        Nmea TTM = Nmea("RATTM" , {"00","0.0","0.0","T","0.0","0.0","T","0.0","0.0","N","","T","","000000.00","A"});
        TTM.set(1, QString("%1").arg(item(row, ColumnType::Number)->data(Qt::DisplayRole).toInt(), 2, 10, QChar('0')).toStdString());

        TTM.set(2, QString("%1").arg(item(row, ColumnType::Distance)->data(Qt::UserRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(3, QString("%1").arg(item(row, ColumnType::Bearing)->data(Qt::UserRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(5, QString("%1").arg(item(row, ColumnType::Speed)->data(Qt::UserRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(6, QString("%1").arg(item(row, ColumnType::Course)->data(Qt::UserRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(8, QString("%1").arg(item(row, ColumnType::Dkr)->data(Qt::UserRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(9, QString("%1").arg(item(row, ColumnType::Tkr)->data(Qt::UserRole).toDouble(), 6, 'f', 2, QChar('0')).toStdString());
        TTM.set(11,  item(row, ColumnType::NameTarget)->data(Qt::DisplayRole).toString().toStdString());
        if(!item(row, ColumnType::Status)->data(Qt::UserRole).toString().isEmpty())
            TTM.set(12,  item(row, ColumnType::Status)->data(Qt::UserRole).toString().toStdString());
        TTM.set(13,  item(row, ColumnType::Reference)->data(Qt::UserRole).toString().toStdString());

        nmea.push_back(QString::fromStdString( TTM.get_string()));
    }   
    return nmea;
}

void TargetModel::setPosShip(PointGeo pos){
    posShip = pos;
}