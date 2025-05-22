#include "ARPA.h"
#include "ui_ARPA.h"
#include "TableDelegate.h"
ARPA::ARPA(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::ARPA),
    modelT(new TargetModel(this))
{
    ui->setupUi(this);
    ui->targetTable->setModel(modelT);
    QItemSelectionModel *selectionModel = ui->targetTable->selectionModel();

    connect(selectionModel, &QItemSelectionModel::currentRowChanged, modelT, &TargetModel::rowSelect);

    TableWidgetsDelegate *del = new TableWidgetsDelegate(this);
    ui->targetTable->setItemDelegate(del);
    del->offColumnBorger({0,11,12});
    connect(ui->pushButton, &QPushButton::clicked, modelT, &TargetModel::addTarget);

    connect(ui->doubleSpinBox_lat, qOverload<double>(&LatitudeEdit::valueChanged),this, &ARPA::updatePositionShip);
    connect(ui->doubleSpinBox_lon, qOverload<double>(&LongitudeEdit::valueChanged),this, &ARPA::updatePositionShip);
    connect(ui->checkBox_simulated, &QCheckBox::clicked, modelT, &TargetModel::setSimulationStatus);
    // Настройка ширины колонок
    QHeaderView *horizontalHeader =  ui->targetTable->horizontalHeader();
    horizontalHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    // Растягиваем остальные колонки
    for (int i = 1; i < 11; ++i) {
        horizontalHeader->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    horizontalHeader->setSectionResizeMode(11, QHeaderView::ResizeToContents);
    horizontalHeader->setSectionResizeMode(12, QHeaderView::ResizeToContents);
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

void ARPA::setPos(double lat, double lon){
    modelT->setPos(lat, lon);
}