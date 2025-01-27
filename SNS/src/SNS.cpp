#include "SNS.h"

SNS::SNS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SNS),
    rmc_nmea("GNRMC",{"000000.00","V","0000.0000","N","00000.0000","E","0.0","0.0","000000","","","N"}),
    vtg_nmea("GNVTG",{"0.0","F","","M","0.0","N","0.0","K","N"}),
    zda_nmea("GNZDA",{"000000.00","00","00","0000","00","00"}),
    gga_nmea("GNGGA",{"000000.00","0000.0000","N","00000.0000","E","0","00","0.0","0.0","M","0.0","M","0.0","0000"}),
    gll_nmea("GNGLL",{"0000.0000","N","00000.0000","E","000000.00","V","V"}){
        
        ui->setupUi(this);
        ui->sns_time->setDateTime(QDateTime::currentDateTime().currentDateTimeUtc());

}

SNS::~SNS()
{
    delete ui;
}

QIcon SNS::icon() const {
    return QIcon();
}
QString SNS::name() const {
    return tr("СНС");
}
QString SNS::description() const {
    return QString("");
}
QStringList SNS::getNavigationData() {
    QStringList nmea;
    ui->sns_time->setDateTime(ui->sns_time->dateTime().addSecs(1));

    if(ui->rmc_check->isChecked()){
        // Время
        rmc_nmea.set(1, ui->sns_time->dateTime().toString("HHmmss.z").toStdString());

        // Статус
        rmc_nmea.set(2, ui->AUTO_STATUS->isChecked() ? "A" :
                    ui->DIFF_STATUS->isChecked() ? "D" :
                    ui->INVALID_STATUS->isChecked() ? "V" : "V");

        // Широта
        double lat = ui->lat->value();
        QString latDeg = QString("%1").arg(qAbs(static_cast<int>(lat)), 2, 10, QChar('0'));
        QString latMin = QString("%1").arg(qAbs(fmod(lat * 60, 60)), 7, 'f', 4, QChar('0'));
        rmc_nmea.set(3, (latDeg + latMin).toStdString());
        rmc_nmea.set(4, lat >= 0 ? "N" : "S");

        // Долгота
        double lon = ui->lon->value();
        QString lonDeg = QString("%1").arg(qAbs(static_cast<int>(lon)), 3, 10, QChar('0'));
        QString lonMin = QString("%1").arg(qAbs(fmod(lon * 60, 60)), 7, 'f', 4, QChar('0'));
        rmc_nmea.set(5, (lonDeg + lonMin).toStdString());
        rmc_nmea.set(6, lon >= 0 ? "E" : "W");

        // Скорость (в узлах)
        double vel = ui->vel->value() * 1.944;
        rmc_nmea.set(7, QString("%1").arg(vel, 3, 'f', 1, QChar('0')).toStdString());

        // Курс
        double cog = ui->cog->value();
        rmc_nmea.set(8, QString("%1").arg(cog, 6, 'f', 2, QChar('0')).toStdString());

        // Дата
        rmc_nmea.set(9, ui->sns_time->dateTime().toString("ddMMyy").toStdString());

        // Магнитное склонение
        double magn = ui->magn->value();
        rmc_nmea.set(10, QString("%1").arg(magn, 3, 'f', 1, QChar('0')).toStdString());
        rmc_nmea.set(11, "E");

        // Режим
        rmc_nmea.set(12, ui->AUTO->isChecked() ? "A" :
                    ui->DIFF->isChecked() ? "D" :
                    ui->WAIT->isChecked() ? "E" :
                    ui->MANUAL->isChecked() ? "M" :
                    ui->IMITATION->isChecked() ? "S" :
                    ui->INVALID_STATUS->isChecked() ? "N" : "N");
        nmea.push_back(QString::fromStdString(rmc_nmea.get_string()));
    }

    if(ui->vtg_check->isChecked()){
         // Курс (COG)
        vtg_nmea.set(1, QString("%1").arg(ui->cog->value(), 3, 'f', 1, QChar('0')).toStdString()); // COG
        // Направление (True north)
        vtg_nmea.set(2, "T"); // True north
        // Магнитный курс (COG + магнитное склонение)
        double magnCog = ui->cog->value() + ui->magn->value();
        vtg_nmea.set(3, QString("%1").arg(magnCog, 3, 'f', 1, QChar('0')).toStdString()); // Magnetic COG
        // Магнитное направление
        vtg_nmea.set(4, "M"); // Magnetic
        // Скорость в узлах
        double velKnots = ui->vel->value() * 1.944;
        vtg_nmea.set(5, QString("%1").arg(velKnots, 3, 'f', 1, QChar('0')).toStdString()); // Knots
        // Единица измерения (узлы)
        vtg_nmea.set(6, "N"); // Knots
        // Скорость в км/ч
        double velKmph = ui->vel->value() * 3.6;
        vtg_nmea.set(7, QString("%1").arg(velKmph, 3, 'f', 1, QChar('0')).toStdString()); // km/h
        // Единица измерения (км/ч)
        vtg_nmea.set(8, "K"); // km/h
        // Режим
        QString mode = ui->AUTO->isChecked() ? "A" :
                    ui->DIFF->isChecked() ? "D" :
                    ui->WAIT->isChecked() ? "E" :
                    ui->MANUAL->isChecked() ? "M" :
                    ui->IMITATION->isChecked() ? "S" :
                    ui->INVALID_STATUS->isChecked() ? "N" : "N";
        vtg_nmea.set(9, mode.toStdString()); // Mode
        
        nmea.push_back(QString::fromStdString(vtg_nmea.get_string()));
    }

    if (ui->zda_check->isChecked()) {
        // Время
        zda_nmea.set(1, ui->sns_time->dateTime().toString("HHmmss.z").toStdString());

        // День
        zda_nmea.set(2, ui->sns_time->dateTime().toString("dd").toStdString());

        // Месяц
        zda_nmea.set(3, ui->sns_time->dateTime().toString("MM").toStdString());

        // Год
        zda_nmea.set(4, ui->sns_time->dateTime().toString("yyyy").toStdString());

        // Смещение UTC (часы)
        int utcHoursOffset = ui->sns_time->dateTime().offsetFromUtc() / 3600;
        zda_nmea.set(5, QString("%1").arg(utcHoursOffset, 2, 10, QChar('0')).toStdString());

        // Смещение UTC (минуты)
        int utcMinsOffset = (ui->sns_time->dateTime().offsetFromUtc() / 60) % 60;
        zda_nmea.set(6, QString("%1").arg(utcMinsOffset, 2, 10, QChar('0')).toStdString());
        nmea.push_back(QString::fromStdString(zda_nmea.get_string()));
    }
    if (ui->gga_check->isChecked()) {
        // Время
        gga_nmea.set(1, ui->sns_time->dateTime().toString("HHmmss.z").toStdString());

        // Широта
        double lat = ui->lat->value();
        QString latDeg = QString("%1").arg(qAbs(static_cast<int>(lat)), 2, 10, QChar('0'));
        QString latMin = QString("%1").arg(qAbs(fmod(lat * 60, 60)), 7, 'f', 4, QChar('0'));
        gga_nmea.set(2, (latDeg + latMin).toStdString());

        // Направление широты
        gga_nmea.set(3, lat >= 0 ? "N" : "S");

        // Долгота
        double lon = ui->lon->value();
        QString lonDeg = QString("%1").arg(qAbs(static_cast<int>(lon)), 3, 10, QChar('0'));
        QString lonMin = QString("%1").arg(qAbs(fmod(lon * 60, 60)), 7, 'f', 4, QChar('0'));
        gga_nmea.set(4, (lonDeg + lonMin).toStdString());

        // Направление долготы
        gga_nmea.set(5, lon >= 0 ? "E" : "W");

        // Статус
        gga_nmea.set(6, ui->AUTO_STATUS_2->isChecked() ? "1" :
                    ui->DIFF_STATUS_2->isChecked() ? "2" :
                    ui->INVALID_STATUS_2->isChecked() ? "0" : "0");

        // Количество спутников
        gga_nmea.set(7, QString("%1").arg(static_cast<int>(ui->satn->value()), 2, 10, QChar('0')).toStdString());

        // HDOP
        gga_nmea.set(8, QString("%1").arg(ui->HDOP->value(), 3, 'f', 1, QChar('0')).toStdString());

        // Высота над уровнем моря
        gga_nmea.set(9, QString("%1").arg(ui->HSEA->value(), 3, 'f', 1, QChar('0')).toStdString());

        // Высота геоида
        gga_nmea.set(11, QString("%1").arg(ui->HGEO->value(), 3, 'f', 1, QChar('0')).toStdString());

        // Дифференциальные данные
        if (ui->DIFF_STATUS_2->isChecked()) {
            gga_nmea.set(13, QString("%1").arg(static_cast<int>(ui->dif_correct->value()), 3, 'f', 1, QChar('0')).toStdString());
            gga_nmea.set(14, QString("%1").arg(static_cast<int>(ui->dif_identifier->value()), 4, 10, QChar('0')).toStdString());
        }
        nmea.push_back(QString::fromStdString(gga_nmea.get_string()));
    }

    if (ui->gll_check->isChecked()) {
        // Широта
        double lat = ui->lat->value();
        QString latDeg = QString("%1").arg(qAbs(static_cast<int>(lat)), 2, 10, QChar('0'));
        QString latMin = QString("%1").arg(qAbs(fmod(lat * 60, 60)), 7, 'f', 4, QChar('0'));
        gll_nmea.set(1, (latDeg + latMin).toStdString());

        // Направление широты
        gll_nmea.set(2, lat >= 0 ? "N" : "S");

        // Долгота
        double lon = ui->lon->value();
        QString lonDeg = QString("%1").arg(qAbs(static_cast<int>(lon)), 3, 10, QChar('0'));
        QString lonMin = QString("%1").arg(qAbs(fmod(lon * 60, 60)), 7, 'f', 4, QChar('0'));
        gll_nmea.set(3, (lonDeg + lonMin).toStdString());

        // Направление долготы
        gll_nmea.set(4, lon >= 0 ? "E" : "W");

        // Время
        gll_nmea.set(5, ui->sns_time->dateTime().toString("HHmmss.z").toStdString());

        // Статус
        gll_nmea.set(6, ui->AUTO_STATUS->isChecked() ? "A" :
                    ui->DIFF_STATUS->isChecked() ? "D" :
                    ui->INVALID_STATUS->isChecked() ? "V" : "V");

        // Режим
        gll_nmea.set(7, ui->AUTO->isChecked() ? "A" :
                    ui->DIFF->isChecked() ? "D" :
                    ui->WAIT->isChecked() ? "E" :
                    ui->MANUAL->isChecked() ? "M" :
                    ui->IMITATION->isChecked() ? "S" :
                    ui->INVALID_STATUS->isChecked() ? "N" : "N");
        nmea.push_back(QString::fromStdString(gll_nmea.get_string()));
    }
    
    if (ui->gsv_check->isChecked()) {
        int countSP = 0;
        Nmea gsv_nmea("GNGSV", {"0", "0", "00"});

        // Установка количества сообщений и номера сообщения
        gsv_nmea.set(1, QString("%1").arg(1).toStdString());
        gsv_nmea.set(2, QString("%1").arg(1).toStdString());

        // Добавление данных о спутниках
        if (ui->satn->value() > 0) {
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->satn_number->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->up_gor->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->azimut_->value()), 3, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->sign->value()), 2, 10, QChar('0')).toStdString());
            countSP++;
        }
        if (ui->satn->value() > 1) {
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->satn_number_2->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->up_gor_2->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->azimut_1->value()), 3, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->sign_2->value()), 2, 10, QChar('0')).toStdString());
            countSP++;
        }
        if (ui->satn->value() > 2) {
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->satn_number_3->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->up_gor_3->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->azimut_2->value()), 3, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->sign_3->value()), 2, 10, QChar('0')).toStdString());
            countSP++;
        }
        if (ui->satn->value() > 3) {
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->satn_number_4->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->up_gor_4->value()), 2, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->azimut_4->value()), 3, 10, QChar('0')).toStdString());
            gsv_nmea.push(QString("%1").arg(static_cast<int>(ui->sign_4->value()), 2, 10, QChar('0')).toStdString());
            countSP++;
        }

        // Установка количества спутников
        gsv_nmea.set(3, QString("%1").arg(countSP, 2, 10, QChar('0')).toStdString());
        nmea.push_back(QString::fromStdString(gsv_nmea.get_string()));
    }

    return nmea;
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