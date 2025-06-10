#include "ClassBPage.h"
#include "ui_ClassBPage.h"
#include "Type18Simulator.h"
#include "Type19Simulator.h"

ClassBPage::ClassBPage(QWidget *parent):BaseAisPage(parent), ui(new Ui::ClassBPage)
{
	ui->setupUi(this);
	type18 = new Type18Simulator();
	type19 = new Type19Simulator();
	ui->widgetSimulatorsB->addWidget("Type 18", type18, "Type18");
	ui->widgetSimulatorsB->addWidget("Type 19", type19, "Type19"); 
	

}

ClassBPage::~ClassBPage()
{
	delete ui;
}
