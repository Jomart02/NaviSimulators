#include "SNS.h"

SNS::SNS(QWidget *parent) :
    BaseNaviWidget(parent),ui(new Ui::SNS){
    ui->setupUi(this);
        ui->sns_time->setDateTime(QDateTime::currentDateTime());

}

SNS::~SNS()
{
    delete ui;
}
void SNS::setText(){

}

// void GnssForm::tick()
// {
//     // ui->sns_time->setDateTime(ui->sns_time->dateTime().addSecs(1));

//     // if (!ui->healthy->isChecked()){
//     //     return;
//     // }

//     // if(ui->rmc_check->isChecked()){
//     //     rmc_nmea.set(1,ui->sns_time->dateTime().toString("HHmmss.z").toStdString()); // time / форматирование в qt5/6 отличается!!
//     //     rmc_nmea.set(2,ui->AUTO_STATUS->isChecked() ? "A" :
//     //         ui->DIFF_STATUS->isChecked() ? "D" :
//     //         ui->INVALID_STATUS->isChecked() ? "V" : "V" 
//     //     ); // status
//     //     rmc_nmea.set(3,(boost::format("%02d") % (int)(fabs(ui->lat->value()))).str() + (boost::format("%07.4f") % (fmod(fabs(ui->lat->value())*60,60))).str());
//     //     rmc_nmea.set(4,ui->lat->value()>=0 ? "N" : "S");
//     //     rmc_nmea.set(5,(boost::format("%03d") % (int)(fabs(ui->lon->value()))).str() + (boost::format("%07.4f") % (fmod(fabs(ui->lon->value())*60,60))).str()); // longitude
//     //     rmc_nmea.set(6,ui->lon->value()>=0 ? "E" : "W");
//     //     rmc_nmea.set(7,(boost::format("%03.1f") % (ui->vel->value()*1.944)).str()); // knots
//     //     rmc_nmea.set(8,(boost::format("%06.2f") % ui->cog->value()).str()); // cog
//     //     rmc_nmea.set(9,ui->sns_time->dateTime().toString("ddMMyy").toStdString()); // date
//     //     rmc_nmea.set(10,(boost::format("%03.1f") % ui->magn->value()).str()); // magnetic declination
//     //     rmc_nmea.set(11,"E"); // magnetic east
//     //     rmc_nmea.set(12,ui->AUTO->isChecked() ? "A" :
//     //         ui->DIFF->isChecked() ? "D" :
//     //         ui->WAIT->isChecked() ? "E" :
//     //         ui->MANUAL->isChecked() ? "M" :
//     //         ui->IMITATION->isChecked() ? "S" :
//     //         ui->INVALID_STATUS->isChecked() ? "N" : "N"
//     //     ); // mode
        
//     //     select->Send(rmc_nmea.get_string());
        

//     // }

//     // if(ui->vtg_check->isChecked()){
//     //     vtg_nmea.set(1,(boost::format("%03.1f") % ui->cog->value()).str()); // cog
//     //     vtg_nmea.set(2,"T"); // True north
//     //     vtg_nmea.set(3,(boost::format("%03.1f") % (ui->cog->value() + ui->magn->value())).str()); // magn cog / я надеюсь это правильно))
//     //     vtg_nmea.set(4,"M"); // magnetic
//     //     vtg_nmea.set(5,(boost::format("%03.1f") % (ui->vel->value()*1.944)).str()); // knots
//     //     vtg_nmea.set(6,"N"); // knots
//     //     vtg_nmea.set(7,(boost::format("%03.1f") % (ui->vel->value()*3.6)).str()); // kmph
//     //     vtg_nmea.set(8,"K"); // kmph
//     //     vtg_nmea.set(9,ui->AUTO->isChecked() ? "A" :
//     //         ui->DIFF->isChecked() ? "D" :
//     //         ui->WAIT->isChecked() ? "E" :
//     //         ui->MANUAL->isChecked() ? "M" :
//     //         ui->IMITATION->isChecked() ? "S" :
//     //         ui->INVALID_STATUS->isChecked() ? "N" : "N"
//     //     ); // mode

//     //     select->Send(vtg_nmea.get_string());
//     // }

//     // if(ui->zda_check->isChecked()){
//     //     zda_nmea.set(1,ui->sns_time->dateTime().toString("HHmmss.z").toStdString()); // time / форматирование в qt5/6 отличается!!
//     //     zda_nmea.set(2,ui->sns_time->dateTime().toString("dd").toStdString()); // day
//     //     zda_nmea.set(3,ui->sns_time->dateTime().toString("MM").toStdString()); // month
//     //     zda_nmea.set(4,ui->sns_time->dateTime().toString("yyyy").toStdString()); // year
//     //     zda_nmea.set(5,(boost::format("%02d") % (ui->sns_time->dateTime().offsetFromUtc()/3600)).str()); // utc hours offset
//     //     zda_nmea.set(6,(boost::format("%02d") % ((ui->sns_time->dateTime().offsetFromUtc()/60)%60)).str()); // utc mins offset

//     //     select->Send(zda_nmea.get_string());
//     // }
//     // if(ui->gga_check->isChecked()){
//     //     gga_nmea.set(1,ui->sns_time->dateTime().toString("HHmmss.z").toStdString());
//     //     gga_nmea.set(2,(boost::format("%02d") % (int)(fabs(ui->lat->value()))).str() + (boost::format("%07.4f") % (fmod(fabs(ui->lat->value())*60,60))).str());
//     //     gga_nmea.set(3,ui->lat->value()>=0 ? "N" : "S");
//     //     gga_nmea.set(4,(boost::format("%03d") % (int)(fabs(ui->lon->value()))).str() + (boost::format("%07.4f") % (fmod(fabs(ui->lon->value())*60,60))).str()); // longitude
//     //     gga_nmea.set(5,ui->lon->value()>=0 ? "E" : "W");
//     //     rmc_nmea.set(6,ui->AUTO_STATUS_2->isChecked() ? "1" :
//     //         ui->DIFF_STATUS_2->isChecked() ? "2" :
//     //         ui->INVALID_STATUS_2->isChecked() ? "0" : "0" 
//     //     ); // status
//     //     gga_nmea.set(7,(boost::format("%02d") % (int)(fabs(ui->satn->value()))).str());
//     //     gga_nmea.set(8,(boost::format("%03.1f") % (ui->HDOP->value())).str());
//     //     gga_nmea.set(9,(boost::format("%03.1f") % (ui->HSEA->value())).str());
//     //     gga_nmea.set(11,(boost::format("%03.1f") % (ui->HGEO->value())).str());
//     //     if(ui->DIFF_STATUS_2->isChecked()){
//     //         gga_nmea.set(13,(boost::format("%03.1f") % (int)(fabs(ui->dif_correct->value()))).str());
//     //         gga_nmea.set(14,(boost::format("%04d") % (int)(fabs(ui->dif_identifier->value()))).str());
//     //     }

//     //     select->Send(gga_nmea.get_string());
//     // }

//     // if(ui->gll_check->isChecked()){
//     //     gll_nmea.set(1,(boost::format("%02d") % (int)(fabs(ui->lat->value()))).str() + (boost::format("%07.4f") % (fmod(fabs(ui->lat->value())*60,60))).str());
//     //     gll_nmea.set(2,ui->lat->value()>=0 ? "N" : "S");
//     //     gll_nmea.set(3,(boost::format("%03d") % (int)(fabs(ui->lon->value()))).str() + (boost::format("%07.4f") % (fmod(fabs(ui->lon->value())*60,60))).str()); // longitude
//     //     gll_nmea.set(4,ui->lon->value()>=0 ? "E" : "W");
//     //     gll_nmea.set(5,ui->sns_time->dateTime().toString("HHmmss.z").toStdString());
//     //     gll_nmea.set(6,ui->AUTO_STATUS->isChecked() ? "A" :
//     //         ui->DIFF_STATUS->isChecked() ? "D" :
//     //         ui->INVALID_STATUS->isChecked() ? "V" : "V" 
//     //     ); // status
//     //     gll_nmea.set(7,ui->AUTO->isChecked() ? "A" :
//     //         ui->DIFF->isChecked() ? "D" :
//     //         ui->WAIT->isChecked() ? "E" :
//     //         ui->MANUAL->isChecked() ? "M" :
//     //         ui->IMITATION->isChecked() ? "S" :
//     //         ui->INVALID_STATUS->isChecked() ? "N" : "N"
//     //     ); // mode

//     //     select->Send(gll_nmea.get_string());
//     // }
    
//     // if(ui->gsv_check->isChecked()){
//     //     countSP=0;
//     //     Nmea gsv_nmea("GNGSV",{"0","0","00"});
//     //     gsv_nmea.set(1,(boost::format("%d") % (int)(1)).str());
//     //     gsv_nmea.set(2,(boost::format("%d") % (int)(1)).str());
        
//     //     //Nmea gsv_nmea("GNGSV",{"0","0","00","00","00","000","00","00","00","000","00","00","00","000","00","00","00","000","00"});
//     //     if(ui->satn->value()>0){
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->satn_number->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->up_gor->value()))).str());
//     //         gsv_nmea.push((boost::format("%03d") % (int)(fabs(ui->azimut_->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->sign->value()))).str());
//     //         countSP++;
//     //     }
//     //     if(ui->satn->value()>1){
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->satn_number_2->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->up_gor_2->value()))).str());
//     //         gsv_nmea.push((boost::format("%03d") % (int)(fabs(ui->azimut_1->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->sign_2->value()))).str());
//     //         countSP++;
//     //     }
//     //     if(ui->satn->value()>2){
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->satn_number_3->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->up_gor_3->value()))).str());
//     //         gsv_nmea.push((boost::format("%03d") % (int)(fabs(ui->azimut_2->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->sign_3->value()))).str());
//     //         countSP++;
//     //     }
//     //     if(ui->satn->value()>3){
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->satn_number_4->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->up_gor_4->value()))).str());
//     //         gsv_nmea.push((boost::format("%03d") % (int)(fabs(ui->azimut_4->value()))).str());
//     //         gsv_nmea.push((boost::format("%02d") % (int)(fabs(ui->sign_4->value()))).str());
//     //         countSP++;
//     //     }

//     //     gsv_nmea.set(3,(boost::format("%02d") % (int)(fabs(countSP))).str());
//     //     select->Send(gsv_nmea.get_string());
        
//     // }


// }