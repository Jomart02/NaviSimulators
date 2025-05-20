#include "BaseAisPage.h"

BaseAisPage::BaseAisPage(QWidget *parent) : QWidget(parent) {}


void BaseAisPage::setComboBoxMMSI(QComboBox* box){
     comboBox_NumbersMMSI = box;
}
void BaseAisPage::setButtonAdd(QPushButton* add){
     connect(add, &QPushButton::clicked, this, &BaseAisPage::addNewTargetClass);
}
void BaseAisPage::setCheckBoxManual(QCheckBox* manual){
     
}

void BaseAisPage::addNewTargetClass() {
     sending = false;

     // Генерация случайного MMSI
     unsigned int min = 100000000;
     unsigned int max = 999999999;
     unsigned int randomNumber = std::random_device{}() % (max - min + 1) + min;

     // Добавление в QComboBox
     int index = comboBox_NumbersMMSI->count();
     comboBox_NumbersMMSI->addItem(QString::number(randomNumber), randomNumber);
     comboBox_NumbersMMSI->setCurrentIndex(index);

     // Создание объекта параметров (определяется в производных классах)
     auto param = createParam();
     param->setMMSI(randomNumber);

     // Добавление в карту
     paramsShip[randomNumber] = std::move(param);

     sending = true;
}