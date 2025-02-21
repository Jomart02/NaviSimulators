
#pragma once
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QtMath>

enum ColumnType {
    Number = 0,       // Не редактируется
    NameTarget,       // QLineEdit максимум 20 символов
    lat,              // LatitudeEdit
    lon,              // LongitudeEdit
    Bearing,          // QDoubleSpinBox
    Distance,         // QDoubleSpinBox
    Speed,            // QDoubleSpinBox
    Course,           // QDoubleSpinBox
    Dkr,              // QDoubleSpinBox
    Tkr,              // QDoubleSpinBox
    Status,           // QComboBox
    Reference         // QCheckBox
};

struct PointGeo{
    double lat;
    double lon;
};
inline QMap<ColumnType, QString > suffix = {
    {ColumnType::Bearing, "°"},
    {ColumnType::Speed, " уз"},
    {ColumnType::Distance, " М"},
    {ColumnType::Dkr, " М"},
    {ColumnType::Course, "°"},
};
class TargetDataDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit TargetDataDelegate(QObject *parent = nullptr);

    // Создание редактора (в данном случае QSpinBox)
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // Установка данных из модели в редактор
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    // Передача данных из редактора обратно в модель
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    // Отображение данных в ячейке
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:


    ColumnType columnType(const QModelIndex &index) const;
};


class TargetModel : public QStandardItemModel {
    Q_OBJECT
public:    
    TargetModel(QObject* parent = nullptr);
    ~TargetModel();
    void addTarget();
    QStringList getNMEA();
public slots:
    void setPosShip(PointGeo pos);
private:
    void updateData();

    inline QPair<double, double> calculateNewPosition(double lat, double lon, double COG, double distance) {
        // Константы
        const double DEG_TO_RAD = M_PI / 180.0; // Преобразование градусов в радианы
        const double EARTH_RADIUS = 6371000.0;  // Радиус Земли в метрах

        // Преобразование входных данных в радианы
        double latRadians = lat * DEG_TO_RAD;
        double lonRadians = lon * DEG_TO_RAD;
        double cogRadians = COG * DEG_TO_RAD;

        // Вычисление углового расстояния
        double angularDistance = distance / EARTH_RADIUS;

        // Вычисление новой широты
        double newLatRadians = asin(sin(latRadians) * cos(angularDistance) +
                                    cos(latRadians) * sin(angularDistance) * cos(cogRadians));

        // Вычисление изменения долготы
        double deltaLon = atan2(sin(cogRadians) * sin(angularDistance) * cos(latRadians),
                                cos(angularDistance) - sin(latRadians) * sin(newLatRadians));

        // Вычисление новой долготы
        double newLonRadians = lonRadians + deltaLon;

        // Нормализация долготы в диапазон [-180, 180]
        if (newLonRadians > M_PI) newLonRadians -= 2 * M_PI;
        if (newLonRadians < -M_PI) newLonRadians += 2 * M_PI;

        // Преобразование обратно в градусы
        double newLat = newLatRadians * (180.0 / M_PI);
        double newLon = newLonRadians * (180.0 / M_PI);

        return qMakePair(newLat, newLon);
    }

    inline double calculateBearing(double startLat, double startLon, double endLat, double endLon) {
        // Конвертируем разницу долготы в радианы
        double dLon = qDegreesToRadians(endLon - startLon);

        // Вычисляем y и x для формулы азимута
        double y = qSin(dLon) * qCos(qDegreesToRadians(endLat));
        double x = qCos(qDegreesToRadians(startLat)) * qSin(qDegreesToRadians(endLat)) -
                qSin(qDegreesToRadians(startLat)) * qCos(qDegreesToRadians(endLat)) * qCos(dLon);

        // Вычисляем азимут (пеленг)
        double bearing = qRadiansToDegrees(qAtan2(y, x));

        // Нормализуем результат в диапазон [0, 360]
        return fmod(bearing + 360.0, 360.0);
    }
    inline double calculateDistance(double startLat, double startLon, double endLat, double endLon) {
        const double earthRadiusKm = 6371.0; // Радиус Земли в километрах
        double dLat = qDegreesToRadians(endLat - startLat);
        double dLon = qDegreesToRadians(endLon - startLon);

        double a = qSin(dLat / 2) * qSin(dLat / 2) +
                qCos(qDegreesToRadians(startLat)) * qCos(qDegreesToRadians(endLat)) *
                qSin(dLon / 2) * qSin(dLon / 2);
        double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));

        return earthRadiusKm * c; // Дистанция в километрах
    }

private:
    int countTargets = 0;
    PointGeo posShip = {0,0};


};