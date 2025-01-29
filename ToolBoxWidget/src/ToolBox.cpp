#include "ToolBox.h"
#include "ui_ToolBox.h"
#include "ToolItem.h"

#include <QVBoxLayout>

ToolBox::ToolBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBox),
    ContentVBoxLayout(nullptr)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget(this);
    ContentVBoxLayout = new QVBoxLayout;
    ContentVBoxLayout->setContentsMargins(0, 0, 0, 0);
    ContentVBoxLayout->setSpacing(0);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(widget);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->addLayout(ContentVBoxLayout);
    vBoxLayout->addStretch(0);

    ui->scrollArea->setWidget(widget);
}

ToolBox::~ToolBox()
{
    delete ui;
}

void ToolBox::addWidget(const QString &title, QWidget *widget,const QString &objectNamePage)
{
    ToolItem *page = new ToolItem(this);
    page->setObjectName(objectNamePage);
    page->addWidget(title, widget);
    ContentVBoxLayout->addWidget(page);
}
QWidget* ToolBox::getWidgetContent(const QString &objectNamePage,const QString &objectNameContent){

    ToolItem *child = findChild<ToolItem*>(objectNamePage);
    if(child){
        return child->getWidgetContent(objectNameContent);
    }else return nullptr;
}