#include "SNS.h"
#include "ui_SNS.h"
#include "GeoMatchFunc.h"



SNS::SNS(QWidget *parent) :
    BaseNaviWidget(parent),
    ui(new Ui::SNS){
        
        ui->setupUi(this);
        ui->sns_time->setDateTime(QDateTime::currentDateTime().currentDateTimeUtc());
        ui->scrollArea->installEventFilter(this);
        ui->scrollArea->setFocusPolicy(Qt::NoFocus);
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
    GEO_POINT posShip;
    
    QString source = "GN";

    if(ui->radioButton_Standart->isChecked()){
        source = ui->comboBox_sourses->currentText();
    }else if(ui->radioButton_Other->isChecked() && ui->lineEdit->text().size() ==2){
        source = ui->lineEdit->text();
    }

    if(ui->checkBox_Simulated->isChecked()){

        // Обновляем курс и скорость
        updateCOGandSOG();

        posShip = GEO_POINT( DegToRad(ui->lat->value()), DegToRad(ui->lon->value()));
        GEO_POINT posShipCalc;
        double dist = ui->vel->getValue();
        double pel = DegToRad(ui->cog->getValue());
        PositionByDistanceBearing(posShip, dist, pel,EllipsoidType::WGS84, DirectCalculationMethod::GEODESIC, posShipCalc);
        posShip = GEO_POINT( RadToDeg(posShipCalc.lat), RadToDeg(posShipCalc.lon));
        ui->lat->setValue(posShip.lat);
        ui->lon->setValue(posShip.lon);



    }else{
        posShip = GEO_POINT(ui->lat->value(),ui->lon->value());
    }

    std::string latString, latDirection, lonString, lonDirection;
    helpFuncNmea::formatLatitude(posShip.lat, latString, latDirection);
    helpFuncNmea::formatLongitude(posShip.lon, lonString, lonDirection);

    if(ui->rmc_check->isChecked()){

        std::string name = source.toStdString() + "RMC";
        Nmea rmc_nmea(name,{"000000.00","V","0000.00","N","0","E","0.0","0.0","000000","","","N"});

        // Время
        rmc_nmea.set(1, ui->sns_time->dateTime().toString("HHmmss.z").toStdString());

        // Статус
        rmc_nmea.set(2, ui->AUTO_STATUS->isChecked() ? "A" :
                    ui->DIFF_STATUS->isChecked() ? "D" :
                    ui->INVALID_STATUS->isChecked() ? "V" : "V");



        rmc_nmea.set(3, latString);
        rmc_nmea.set(4, latDirection);
        rmc_nmea.set(5, lonString);
        rmc_nmea.set(6, lonDirection);

        // Скорость (в узлах)
        double vel = ui->vel->getValue() * 1.944;
        rmc_nmea.set(7, QString("%1").arg(vel, 3, 'f', 1, QChar('0')).toStdString());

        // Курс
        double cog = ui->cog->getValue();
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
        std::string name = source.toStdString() + "VTG";
        Nmea vtg_nmea(name,{"0.0","F","","M","0.0","N","0.0","K","N"});

         // Курс (COG)
        vtg_nmea.set(1, QString("%1").arg(ui->cog->getValue(), 3, 'f', 1, QChar('0')).toStdString()); // COG
        // Направление (True north)
        vtg_nmea.set(2, "T"); // True north
        // Магнитный курс (COG + магнитное склонение)
        double magnCog = ui->cog->getValue() + ui->magn->value();
        vtg_nmea.set(3, QString("%1").arg(magnCog, 3, 'f', 1, QChar('0')).toStdString()); // Magnetic COG
        // Магнитное направление
        vtg_nmea.set(4, "M"); // Magnetic
        // Скорость в узлах
        double velKnots = ui->vel->getValue() * 1.944;
        vtg_nmea.set(5, QString("%1").arg(velKnots, 3, 'f', 1, QChar('0')).toStdString()); // Knots
        // Единица измерения (узлы)
        vtg_nmea.set(6, "N"); // Knots
        // Скорость в км/ч
        double velKmph = ui->vel->getValue() * 3.6;
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
        std::string name = source.toStdString() + "ZDA";
        Nmea zda_nmea(name,{"000000.00","00","00","0000","00","00"});
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
        std::string name = source.toStdString() + "GGA";
        Nmea gga_nmea(name,{"000000.00","0000.0000","N","00000.0000","E","0","00","0.0","0.0","M","0.0","M","0.0","0000"});

        // Время
        gga_nmea.set(1, ui->sns_time->dateTime().toString("HHmmss.z").toStdString());

        gga_nmea.set(2, latString);
        gga_nmea.set(3, latDirection);
        gga_nmea.set(4, lonString);
        gga_nmea.set(5, lonDirection);

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
        std::string name = source.toStdString() + "GLL";
        Nmea gll_nmea(name,{"0000.0000","N","00000.0000","E","000000.00","V","V"});
        // Широта
        
        gll_nmea.set(1, latString);
        gll_nmea.set(2, latDirection);
        gll_nmea.set(3, lonString);
        gll_nmea.set(4, lonDirection);

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
        std::string name = source.toStdString() + "GSV";
        Nmea gsv_nmea(name, {"0", "0", "00"});

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

void SNS::setPos(double lat, double lon){
    ui->lat->setValue(lat);
    ui->lon->setValue(lon);
}

void SNS::retranslate(){
    ui->retranslateUi(this);
}

QString SNS::getRetranslateName(QString retranslateName){
    return QString(":/translations/" + retranslateName + PROJECT_NAME);
}

void SNS::updateCOGandSOG() {
    if (freezeCounter > 0) {
        freezeCounter--;
        return; // Заморожено — ничего не меняем
    }

    // Случайное число от 0 до 100
    double chance = rand() % 100;

    // С вероятностью 20% — замораживаем на 3-10 секунд
    if (chance < 20) {
        freezeDuration = 3 + rand() % 8; // от 3 до 10 секунд
        freezeCounter = freezeDuration;
        return;
    }

    double currentCOG = ui->cog->getValue();
    double currentSOG = ui->vel->getValue();
    // Меняем курс
    double deltaCOG = cogChangeDist(generator); // Небольшое изменение
    currentCOG = fmod(currentCOG + deltaCOG, 360.0);
    if (currentCOG < 0) currentCOG += 360.0;

    // Меняем скорость
    double deltaSOG = sogChangeDist(generator); // Небольшое изменение
    currentSOG = qMax(0.0, currentSOG + deltaSOG); // Не меньше 0

    // С вероятностью 10% немного увеличиваем скорость
    if (chance < 10 && currentSOG < 30) { // ограничение на макс. скорость
        currentSOG += 0.5;
    }
    // С вероятностью 10% немного уменьшаем
    else if (chance < 20 && currentSOG > 0.5) {
        currentSOG -= 0.5;
    }

    ui->cog->setValue(currentCOG);      // Курс (Course Over Ground)
    ui->vel->setValue(currentSOG);      // Скорость (Speed Over Ground)
}