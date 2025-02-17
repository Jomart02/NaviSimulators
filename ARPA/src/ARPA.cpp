#include "ARPA.h"
#include "ui_ARPA.h"

ARPA::ARPA(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::ARPA),
    modelT(new TargetModel(this))
{
    ui->setupUi(this);
    ui->targetTable->setModel(modelT);
    TargetDataDelegate *del = new TargetDataDelegate(this);
    ui->targetTable->setItemDelegate(del);
    connect(ui->pushButton, &QPushButton::clicked, modelT, &TargetModel::addTarget);

    connect(ui->doubleSpinBox_lat, qOverload<double>(&LatitudeEdit::valueChanged),this, &ARPA::updatePositionShip);
    connect(ui->doubleSpinBox_lon, qOverload<double>(&LongitudeEdit::valueChanged),this, &ARPA::updatePositionShip);

        // Настройка ширины колонок
    QHeaderView *horizontalHeader =  ui->targetTable->horizontalHeader();

    // Запрещаем изменение размера первой колонки и устанавливаем фиксированную ширину
    horizontalHeader->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader->resizeSection(0, 80); // Устанавливаем ширину первой колонки

    // Растягиваем остальные колонки
    for (int i = 1; i < modelT->columnCount(); ++i) {
        horizontalHeader->setSectionResizeMode(i, QHeaderView::Stretch);
    }

}

void ARPA::updatePositionShip(double v){
    PointGeo pos = {ui->doubleSpinBox_lat->value(),ui->doubleSpinBox_lon->value()};
    modelT->setPosShip(pos);
}

ARPA::~ARPA()
{
    delete ui;
}

QIcon ARPA::icon() const {
    return QIcon();
}
QString ARPA::name() const {
    return tr("ARPA");
}
QString ARPA::description() const {
    return QString("");
}

QStringList ARPA::getNavigationData(){

    return modelT->getNMEA();
}
