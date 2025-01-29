#include "Type123Simulator.h"

Type123Simulator::Type123Simulator(QWidget *parent) :
    BaseAISSimulator(parent),
    ui(new Ui::Type123Simulator){
        
    ui->setupUi(this);
}

Type123Simulator::~Type123Simulator(){

}
void Type123Simulator::updateAisData(QStringList &aisMess){

}