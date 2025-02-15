#include "ToolItem.h"
#include <QLabel>
#include "ui_ToolItem.h"

ToolItem::ToolItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolItem),
    Expanded(true),
    Label(nullptr)
{
    ui->setupUi(this);

    ui->widgetContent->setAttribute(Qt::WA_StyledBackground);

    Label = new QLabel(this);
    Label->setFixedSize(20, 20);
    Label->setText("<");
    // m_pLabel->setPixmap(QPixmap(":/img/down-arrow.png").scaled(m_pLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    QHBoxLayout *layout = new QHBoxLayout(ui->pushButton);
    layout->setContentsMargins(0, 0, 5, 0);
    layout->addStretch(1);
    layout->addWidget(Label);

    // QFile file(":/qss/ToolItem.qss");
    // if (file.open(QIODevice::ReadOnly)) {
    //     setStyleSheet(file.readAll());
    // }
    // file.close();

    connect(ui->pushButton, &QPushButton::clicked, this, &ToolItem::onPushButtonClicked);
}

ToolItem::~ToolItem()
{
    delete ui;
}
QWidget* ToolItem::getWidgetContent(const QString &objectName){
    QWidget *child = findChild<QWidget*>(objectName);
    if(child){
        return child;
    }else return nullptr;
}
void ToolItem::addWidget(const QString &title, QWidget *widget)
{
    ui->pushButton->setText(title);
    ui->verticalLayoutContent->addWidget(widget);

}

void ToolItem::expand()
{
    ui->widgetContent->show();
    Expanded = true;
    // m_pLabel->setPixmap(QPixmap(":/img/down-arrow.png").scaled(m_pLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void ToolItem::collapse()
{
    ui->widgetContent->hide();
    Expanded = false;
    // m_pLabel->setPixmap(QPixmap(":/img/left-arrow.png").scaled(m_pLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void ToolItem::onPushButtonClicked()
{
    if (Expanded) {
        collapse();
    } else {
        expand();
    }
}
