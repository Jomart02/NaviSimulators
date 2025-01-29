#pragma once

#include <QWidget>

namespace Ui {
class ToolBox;
}

class QVBoxLayout;
class ToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBox(QWidget *parent = nullptr);
    ~ToolBox();
    void addWidget(const QString &title, QWidget *widget, const QString &objectNamePage);
    QWidget* getWidgetContent(const QString &objectNamePage,const QString &objectNameContent);
private:
    Ui::ToolBox *ui;
    QVBoxLayout *ContentVBoxLayout;
    
};

