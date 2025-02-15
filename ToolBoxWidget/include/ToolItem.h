#pragma once
#include <QWidget>

namespace Ui {
class ToolItem;
}

class QFormLayout;
class QLabel;

class ToolItem : public QWidget
{
    Q_OBJECT
public:
    explicit ToolItem(QWidget *parent = nullptr);
    ~ToolItem();
    QWidget* getWidgetContent(const QString &objectName);
public slots:
    void addWidget(const QString &title, QWidget *widget);
    void expand();
    void collapse();
private slots:
    void onPushButtonClicked();
private:
    Ui::ToolItem *ui;
    bool Expanded;
    QLabel *Label;
};

