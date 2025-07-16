#include "BaseAisPage.h"

BaseAisPage::BaseAisPage(QWidget *parent) : QWidget(parent) {}


void BaseAisPage::setComboBoxMMSI(QComboBox* box){
     m_comboBox_NumbersMMSI = box;
     connect(m_comboBox_NumbersMMSI, &QComboBox::currentIndexChanged, this, &BaseAisPage::boxIndexChange);
}
void BaseAisPage::setButtonAdd(QPushButton* add){
     connect(add, &QPushButton::clicked, this, &BaseAisPage::addNewTargetClass);
}
void BaseAisPage::setCheckBoxManual(QCheckBox* manual){
     
}

void BaseAisPage::setCheckBoxActive(QCheckBox* active){
     m_active = active;
     connect(m_active, &QCheckBox::toggled, this, &BaseAisPage::activeChange);
}

void BaseAisPage::boxIndexChange(int index){
     m_sending = false;
     unsigned int mmsi = m_comboBox_NumbersMMSI->currentData().toUInt();
     unsigned int mmsi_prev = 0;
     if(previousIndex != -1)  mmsi_prev = m_comboBox_NumbersMMSI->itemData(previousIndex).toUInt();
     if(paramsShip.find(mmsi) != paramsShip.end()) swapTarget(mmsi_prev,mmsi);
     previousIndex = index;
     m_sending = true;

     if(m_active){
          const QSignalBlocker blocker(m_active);
          if(paramsShip.find(mmsi) != paramsShip.end()){
               m_active->setChecked(paramsShip.at(mmsi)->getEnabled());
          }
     }

}

void BaseAisPage::activeChange(bool flag){
     if(m_comboBox_NumbersMMSI->count() == 0) return;
     unsigned int mmsi = m_comboBox_NumbersMMSI->currentData().toUInt();
     paramsShip[mmsi]->setEnabled(flag);
}    

void BaseAisPage::addNewTargetClass() {
     m_sending = false;

     // Генерация случайного MMSI
     unsigned int min = 100000000;
     unsigned int max = 999999999;
     unsigned int randomNumber = std::random_device{}() % (max - min + 1) + min;
     // Добавление в QComboBox
     int index = m_comboBox_NumbersMMSI->count();
     m_comboBox_NumbersMMSI->addItem(QString::number(randomNumber), QVariant::fromValue(randomNumber));
     // Создание объекта параметров (определяется в производных классах)
     auto param = createParam();
     param->setMMSI(randomNumber);

     // Добавление в карту
     paramsShip[randomNumber] = std::move(param);
     m_comboBox_NumbersMMSI->setCurrentIndex(index);
     m_sending = true;
     
}