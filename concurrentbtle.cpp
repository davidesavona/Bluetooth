#include "concurrentbtle.h"
#include <QDebug>
#include <QTimer>
#include <QtEndian>
#include <iostream>
#include <fstream>
#include <QtCore/QtAlgorithms>
#include <QtCore>
#include <math.h>
#include <unistd.h>

ofstream CSVfileL; //dato grezzo pedale sinistro ed efficiency
ofstream CSVfileR; //dato grezzo pedale destro ed efficiency
ofstream CSVfileC; //dato grezzo cardio
ofstream CSVfilePL; //dato po media e smoothness sinistro
ofstream CSVfilePR; //dato po media e smoothness destro
double Angle_old_left = 0.0;
double Angle_old_right = 0.0;
double sumpo_right = 0.0;
double sumpo_left = 0.0;
int countpo_right=0;
int countpo_left=0;
double po_old_right=0.0;
double po_old_left=0.0;
double po_old_old_left=0.0;
double po_old_old_right=0.0;
double massimo_right=0.0;
double massimo_left=0.0;
bool temp_check = 0;
double massimo = 0.0;


double massimo_sx = 0.0;
double massimo_dx = 0.0;

double massimo_sxpo = 0.0;
double massimo_dxpo = 0.0;

bool ok_pleft = 0;
bool ok_pright = 0;
bool ok_cardio = 0;

double sumtf_left=0.0;
double medciclotf_left=0.0;
int cicli_left=0;
double tfmean_left=0.0;
double sumtf_right=0.0;

double sum_cicloquadr_right=0.0;
double sum_cicloquadr_left=0.0;

double sum_cicloham_right=0.0;
double sum_cicloham_left=0.0;

int cicli_right=0;
double tfmean_right=0.0;


double sum_force_right = 0.0;
int count_force_right = 0;
double sum_meanforce_right = 0.0;
bool flag_ciclo_right = 0;

double sum_force_left = 0.0;
int count_force_left = 0;
double sum_meanforce_left= 0.0;
bool flag_ciclo_left = 0;


double pmax_right=0.0;
double pmin_right=0.0;

double pmin_left=0.0;
double pmax_left=0.0;

double pmax_old_right=0.0;
double pmax_old_left=0.0;
double pmax_old_old_left=0.0;
double pmax_old_old_right=0.0;

double maxpeak_right=0.0;
double maxpeak_left=0.0;

double pmin_old_right=0.0;
double pmin_old_left=0.0;
double pmin_old_old_left=0.0;
double pmin_old_old_right=0.0;

double minpeak_right=0.0;
double minpeak_left=0.0;

double minimo_right=0.0;
double minimo_left=0.0;
double minimo = 0.0;
double minimo_dx = 0.0;
double minimo_sx = 0.0;

//media mobile
double picco_0ql=0.0;
double picco_1ql=0.0;
double picco_2ql=0.0;

double picco_0hl=0.0;
double picco_1hl=0.0;
double picco_2hl=0.0;

double picco_0qr=0.0;
double picco_1qr=0.0;
double picco_2qr=0.0;

double picco_0hr=0.0;
double picco_1hr=0.0;
double picco_2hr=0.0;

double  media_forcequadr_left=0.0;
double  media_forcequadr_right=0.0;





// code to write data on the  shared memory //










// code to initialize btle function
ConcurrentBtle::ConcurrentBtle(QObject *parent) : QObject(parent)
{



//    desiredDevices << QBluetoothAddress(QStringLiteral("C6:21:8B:A7:24:5F")); /*SRM_XP_L_1818*/
    desiredDevices << QBluetoothAddress(QStringLiteral("F6:D0:29:C5:60:4C")); /*SRM_XP_L_2623*/
//    desiredDevices << QBluetoothAddress(QStringLiteral("ED:86:C3:29:8A:05")); /*SRM_XP_R_1968*/
    desiredDevices << QBluetoothAddress(QStringLiteral("D5:5E:63:D1:CE:BF")); /*SRM_XP_R_2971*/
    desiredDevices << QBluetoothAddress(QStringLiteral("EE:5D:EE:37:DE:25")); /*Polar H10 8E5AB228*/

    agent = new QBluetoothDeviceDiscoveryAgent(this);
    agent->setLowEnergyDiscoveryTimeout(10000);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, [this](const QBluetoothDeviceInfo &info){
        qDebug() << "Found device: " << info.address();

        foundDevices.append(info);
    });


    connect(agent,
            QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, [](QBluetoothDeviceDiscoveryAgent::Error error){
        qDebug() << "Discovery error" << error;
    });

    connect(agent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, [this](){
        temp_check = 1;
        
        qDebug() << "temp check" << temp_check;
        qDebug() << "Discovery finished";
        // add a boolean to check connection with shared memory

        for (auto desiredDevice: qAsConst(desiredDevices)) {

            bool found = false;

            for (auto foundDevice: foundDevices) {
                if (foundDevice.address() == desiredDevice) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                qDebug() << "Cannot find" << desiredDevice;
                return;
            }
        }
        qDebug() << "Found required devices" << desiredDevices;
        establishConnection();

    });

    startSearch();



    auto timer = new QTimer(this);
    timer->setInterval(10000);
    connect(timer, &QTimer::timeout, this, [this]() {
        if (agent->isActive())
            return;
        if (!device1 || !device2 || !device3 )
            establishConnection();
    });
    timer->start();
}

// function to initialize shared memory

void OpenFileLeft(){
    // Get system  time
    time_t t = time(nullptr);
    struct tm * now = localtime( & t );
    char buffer [80];

    // Log directory
    strftime (buffer,80,"/home/pi/Desktop/AcquisizioneTESTcodice/LeftAD-%Y-%m-%d-%H-%M-%S.csv",now);
    CSVfileL.open (buffer);
    if(CSVfileL.is_open()){

    CSVfileL << endl
            << "Time" << ","
            << "Tangential Force" << ","
            << "Radial Force" << ","
            << "Angle" << ","
            << "Cadence" << ","
            << "Efficiency" << endl;

    }
}

void writeFileLeft(double parameter1, double parameter2, double parameter3, double parameter4, double parameter5, double parameter6){
    // clock_gettime ( CLOCK_MONOTONIC, &timeLoop);
    CSVfileL << endl
             << parameter1 << ","
             << parameter2 << ","
             << parameter3 << ","
             << parameter4 << ","
             << parameter5 << ","
             << parameter6;

}


void OpenFileRight(){
    // Get system  time
    time_t t = time(nullptr);
    struct tm * now = localtime( & t );
    char buffer [80];

    // Log directory
    strftime (buffer,80,"/home/pi/Desktop/AcquisizioneTESTcodice/RightAD-%Y-%m-%d-%H-%M-%S.csv",now);
    CSVfileR.open (buffer);
    if(CSVfileR.is_open()){

    CSVfileR << endl
            << "Time" << ","
            << "Tangential Force" << ","
            << "Radial Force" << ","
            << "Angle" << ","
            << "Cadence" << ","
            << "Efficiency" << endl;


    }
}

void writeFileRight(double parameter1, double parameter2, double parameter3, double parameter4, double parameter5, double parameter6){
    // clock_gettime ( CLOCK_MONOTONIC, &timeLoop);
    CSVfileR << endl
            << parameter1 << ","
            << parameter2 << ","
            << parameter3 << ","
            << parameter4 << ","
            << parameter5 << ","
            << parameter6;

}

void OpenFileCardio(){
    // Get system  time
    time_t t = time(nullptr);
    struct tm * now = localtime( & t );
    char buffer [80];

    // Log directory
    strftime (buffer,80,"/home/pi/Desktop/Dati_Ble/Cardio/HRAcquiredData-%Y-%m-%d-%H-%M-%S.csv",now);
    CSVfileC.open (buffer);
    if(CSVfileC.is_open()){

    CSVfileC << endl
            << "HR" << endl;

    }
}

void writeFileCardio(double parameter1){
    // clock_gettime ( CLOCK_MONOTONIC, &timeLoop);
    CSVfileC << endl
            << parameter1;

}

void OpenFilePOL(){
    // Get system  time
    time_t t = time(nullptr);
    struct tm * now = localtime( & t );
    char buffer [80];

    // Log directory
    strftime (buffer,80,"/home/pi/Desktop/Lavoro/BLEData/PO/POL-%Y-%m-%d-%H-%M-%S.csv",now);
    CSVfilePL.open (buffer);
    if(CSVfilePL.is_open()){

    CSVfilePL << endl
            << "PO" << ","
            << "Smoothness" << ","
            << "Campioni" << ","
            << "Max" << endl;

    }
}

void writeFilePOL(double parameter1, double parameter2, double parameter3, double parameter4){
    // clock_gettime ( CLOCK_MONOTONIC, &timeLoop);
    CSVfilePL << endl
             << parameter1 << ","
             << parameter2 << ","
             << parameter3 << ","
             << parameter4;

}

void OpenFilePOR(){
    // Get system  time
    time_t t = time(nullptr);
    struct tm * now = localtime( & t );
    char buffer [80];

    // Log directory
    strftime (buffer,80,"/home/pi/Desktop/Lavoro/BLEData/PO/POR-%Y-%m-%d-%H-%M-%S.csv",now);
    CSVfilePR.open (buffer);
    if(CSVfilePR.is_open()){

    CSVfilePR << endl
              << "PO" << ","
              << "Smoothness" << ","
              << "Campioni" << ","
              << "Max" << endl;

    }
}

void writeFilePOR(double parameter1, double parameter2, double parameter3, double parameter4){
    // clock_gettime ( CLOCK_MONOTONIC, &timeLoop);
    CSVfilePR << endl
              << parameter1 << ","
              << parameter2 << ","
              << parameter3 << ","
              << parameter4;

}

double findmax_dxpo (double p_o, double p_o_o, double p){

    if((p<p_o)&&(p_o_o<p_o)){
        if (p_o>massimo_dxpo){
        massimo_dxpo=p_o; //solo se mi trovo in questa condizione altrimento max resta quello che ho già
             }
    else {
        massimo_dxpo=massimo_dxpo;
    }
    }
   return massimo_dxpo;

}

double findmax_sxpo (double p_o, double p_o_o, double p){

    if((p<p_o)&&(p_o_o<p_o)){
        if (p_o>massimo_sxpo){
        massimo_sxpo=p_o; //solo se mi trovo in questa condizione altrimento max resta quello che ho già
             }
    else {
        massimo_sxpo=massimo_sxpo;
    }
    }
   return massimo_sxpo;

}

double findmax_dx (double p_o, double p_o_o, double p){

    if((p<p_o)&&(p_o_o<p_o)){
        if (p_o>massimo_dx){
        massimo_dx=p_o; //solo se mi trovo in questa condizione altrimento max resta quello che ho già
             }
    else {
        massimo_dx=massimo_dx;
    }
    }
   return massimo_dx;

}

double findmax_sx (double p_o, double p_o_o, double p){

    if((p<p_o)&&(p_o_o<p_o)){
        if (p_o>massimo_sx){
        massimo_sx=p_o; //solo se mi trovo in questa condizione altrimento max resta quello che ho già
             }
    else {
        massimo_sx=massimo_sx;
    }
    }
   return massimo_sx;

}
double findmin_dx (double m_o, double m_o_o, double m){

    if((m>m_o)&&(m_o_o>m_o)){
        if (m_o<minimo_dx){
        minimo_dx=m_o; //solo se mi trovo in questa condizione altrimento max resta quello che ho già
             }
    else {
        minimo_dx=minimo_dx;
    }
    }
   return minimo_dx;

}

double findmin_sx (double m_o, double m_o_o, double m){

    if((m>m_o)&&(m_o_o>m_o)){
        if (m_o<minimo_sx){
        minimo_sx=m_o; //solo se mi trovo in questa condizione altrimento max resta quello che ho già
             }
    else {
        minimo_sx=minimo_sx;
    }
    }
   return minimo_sx;

}






void poweroutputL (double tf, double anglevel, double angle){
//    qDebug()<<"angle left"<<angle;
    if ((angle>=180) && (angle<=360)) {
        angle=angle-180;
    }
    else {
        angle=angle+180;
    }
    double diff=(angle+359)-Angle_old_left;
    if ((angle>=Angle_old_left)||((angle<Angle_old_left)&&(diff>=Angle_old_left))){
        //if pedals are going forward or are not moving
//        qDebug()<<"angle left"<<angle;
            double po_left=tf*(-anglevel)*172.5/1000;
//            qDebug()<<"po"<<po_left;

            sumpo_left=sumpo_left+po_left;
            countpo_left=countpo_left+1;

            massimo_left=findmax_sxpo(po_old_left,po_old_old_left,po_left);
            po_old_old_left=po_old_left;
            po_old_left=po_left;

            pmax_left=tf;
            pmin_left=tf;

            maxpeak_left=findmax_sx(pmax_old_left,pmax_old_old_left,pmax_left);
            pmax_old_old_left=pmax_old_left;
            pmax_old_left=pmax_left;

            minpeak_left=findmin_sx(pmin_old_left,pmin_old_old_left,pmin_left);
            pmin_old_old_left=pmin_old_left;
            pmin_old_left=pmin_left;
  }
    else if(angle<Angle_old_left) {
        //if new cycle
       if (diff<Angle_old_left){
//           qDebug()<<"rpm"<<-anglevel*60/2/M_PI;
//                 qDebug()<<"countpoL"<<countpo_left;

           picco_0ql=picco_1ql;
           picco_1ql=picco_2ql;
           picco_2ql=maxpeak_left;

           picco_0hl=picco_1hl;
           picco_1hl=picco_2hl;
           picco_2hl=minpeak_left;

                 double media_left=sumpo_left/countpo_left;
                 double smooth_left=media_left/massimo_left;
                
//                 qDebug()<<"POL"<<media_left;
//                 qDebug()<<"smoothL"<<media_left/massimo_left;
//                 qDebug()<<"maxL"<<massimo_left;
                
                 countpo_left=0;
                 sumpo_left=0;

          //       sum_cicloquadr_left=sum_cicloquadr_left+maxpeak_left;
                 sum_cicloquadr_left=picco_2ql+ picco_1ql+ picco_0ql;

           //      sum_cicloham_left=sum_cicloham_left+minpeak_left;
                  sum_cicloham_left=picco_2hl+ picco_1hl+ picco_0hl;

  //               double media_force_left=sum_force_left/countpo_left;
  //               write_tfmean_left(media_force_left);

                 cicli_left=cicli_left+1;



//                cout << "AAAAAAAAAAFsx_quadr" <<  media_forcequadr_left << endl ;
 //               cout << "AAAAAAAAAApeakleft" <<  maxpeak_left << endl ;



             sum_force_left=0;

             massimo_sx=0;
             minimo_sx=0;

             maxpeak_left=0;
             minpeak_left=0;

             pmax_old_old_left=0;
             pmax_old_left=0;

             pmin_old_old_left=0;
             pmin_old_left=0;

  //variabili PO
             massimo_left=0;
             po_old_old_left=0;
             po_old_left=0;
             massimo_sxpo=0;


    }

       if (cicli_left>2) {


       media_forcequadr_left=sum_cicloquadr_left/3;
       cout << "quadr_sx" << media_forcequadr_left << endl;

       double media_forceham_left=sum_cicloham_left/3;

      //         cicli_left=0;
               
               media_forceham_left=0;
              sum_cicloham_left=0;


//              qDebug()<<"tfmedia_left"<<tfmean_left;
//             write_tfmean_left(tfmean_left);
       }
    }
    Angle_old_left=angle;

}




void poweroutputR (double tf, double anglevel, double angle){
    double diff=(angle+359)-Angle_old_right;
    if ((angle>=Angle_old_right)||((angle<Angle_old_right)&&(diff>=Angle_old_right))){
        //if pedals are going forward or are not moving
//        qDebug()<<"angle right"<<angle;
        double po_right=tf*(anglevel)*172.5/1000;
//                    qDebug()<<"tf_right"<<tf;
        sumpo_right=sumpo_right+po_right;
        countpo_right=countpo_right+1;

        massimo_right=findmax_dxpo(po_old_right,po_old_old_right,po_right);
        po_old_old_right=po_old_right;
        po_old_right=po_right;
        sum_force_right+=tf;
//        cout<<"Sumrightforce"<<sum_force_right<<endl;
        count_force_right=count_force_right+1;

        pmax_right=tf;
        pmin_right=tf;

        maxpeak_right=findmax_dx(pmax_old_right,pmax_old_old_right,pmax_right);
        pmax_old_old_right=pmax_old_right;
        pmax_old_right=pmax_right;

        minpeak_right=findmin_dx(pmin_old_right,pmin_old_old_right,pmin_right);
        pmin_old_old_right=pmin_old_right;
        pmin_old_right=pmin_right;


    }
    else if(angle<Angle_old_right) {
        //if new cycle
        if (diff<Angle_old_right){

            double media_right=sumpo_right/countpo_right;
            double smooth_right=media_right/massimo_right;

            

            writeFilePOR(media_right,smooth_right,countpo_right,massimo_right);
            countpo_right=0;
            sumpo_right=0;

            picco_0qr=picco_1qr;
            picco_1qr=picco_2qr;
            picco_2qr=maxpeak_right;

            picco_0hr=picco_1hr;
            picco_1hr=picco_2hr;
            picco_2hr=minpeak_right;

          //  sum_cicloquadr_right=sum_cicloquadr_right+maxpeak_right;

         //   sum_cicloham_right=sum_cicloham_right+minpeak_right;

            sum_cicloquadr_right=picco_2qr+ picco_1qr+ picco_0qr;


             sum_cicloham_right=picco_2hr+ picco_1hr+ picco_0hr;

            cicli_right=cicli_right+1;


//            cout << "AAAAAAAAAAFsx_quadr" <<  media_forcequadr_left << endl ;
//            cout << "AAAAAAAAAApeakright" <<  maxpeak_right << endl ;
     //       cout<<"rightforce"<<media_force_right<<endl;

            maxpeak_right=0;
            minpeak_right=0;

            massimo_dx=0;
            minimo_dx=0;

            pmax_old_old_right=0;
            pmin_old_old_right=0;

            pmax_old_right=0;
            pmin_old_right=0;

            count_force_right=0;
            sum_force_right=0;

//variabili PO
            massimo_right=0;
            po_old_old_right=0;
            po_old_right=0;
            massimo_dxpo=0;

        }




    if (cicli_right>2) {

         media_forcequadr_right=sum_cicloquadr_right/3;
        cout << "quadr_dx" << media_forcequadr_right << endl;


       

        double   media_forceham_right=sum_cicloham_right/3;

        

//                flag_ciclo_right = 1;
//                write_flag_right(flag_ciclo_right);


        sum_cicloquadr_right=0;
        media_forcequadr_right=0;
        sum_cicloham_right=0;
        media_forceham_right=0;
 //       cicli_right=0;

    }
}
//        else
//        { flag_ciclo_right = 0;
//            write_flag_right(flag_ciclo_right);}

    Angle_old_right=angle;

}




void ConcurrentBtle::startSearch()
{
    if (agent->isActive())
        agent->stop();

    foundDevices.clear();

    agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}


void ConcurrentBtle::establishConnection()
{
    if (!device1) {
        for (int i=0;i<3;i++) {
//           if (desiredDevices.at(i)==QBluetoothAddress(QStringLiteral("C6:21:8B:A7:24:5F")))
             if (desiredDevices.at(i)==QBluetoothAddress(QStringLiteral("F6:D0:29:C5:60:4C")))
            device1 = new QLowEnergyController(desiredDevices.at(i));
        }
        device1->setParent(this);
        connect(device1, &QLowEnergyController::connected, this, [&](){
            ok_pleft=1;
            
            qDebug() << "*********** Device 1 SRM_XP_L_1818 connected" << device1->remoteAddress();
            device1->discoverServices();
        });

        connect(device1, &QLowEnergyController::disconnected, this, [&](){
            ok_pleft=0;
            
            qDebug() << "*********** Device 1 Disconnected";
            QTimer::singleShot(1000, this, [&](){
                qDebug() << "Reconnecting device 1";
                device1->connectToDevice();
            });
        });

        connect(device1, &QLowEnergyController::discoveryFinished, this, [&](){
           qDebug() <<  "*********** Device 1 discovery finished";
           setupNotificationLeft(device1, QStringLiteral("Device 1"));
        });

        device1->connectToDevice();
    }

    if (!device2 && desiredDevices.count() >= 2) {
        for (int i=0;i<3;i++) {
//          if (desiredDevices.at(i)==QBluetoothAddress(QStringLiteral("ED:86:C3:29:8A:05")))
                if (desiredDevices.at(i)==QBluetoothAddress(QStringLiteral("D5:5E:63:D1:CE:BF")))
            device2 = new QLowEnergyController(desiredDevices.at(i));
        }
        device2->setParent(this);
        connect(device2, &QLowEnergyController::connected, this, [&](){
            ok_pright=1;
          
            qDebug() << "*********** Device 2 SRM_XP_R_1968 connected" << device2->remoteAddress();
            device2->discoverServices();
        });

        connect(device2, &QLowEnergyController::disconnected, this, [&](){
            ok_pright=0;
            
            qDebug() << "*********** Device 2 Disconnected";
            QTimer::singleShot(1000, this, [&](){
                qDebug() << "Reconnecting device 2";
                device2->connectToDevice();
            });
        });
        connect(device2, &QLowEnergyController::discoveryFinished, this, [&](){
           qDebug() <<  "*********** Device 2 discovery finished";
           setupNotificationRight(device2, QStringLiteral("Device 2"));
//           setupNotificationCPRight(device2,QStringLiteral("Device 2"));
        });

        device2->connectToDevice();
    }


    if (!device3 && desiredDevices.count() >= 3) {
        for (int i=0;i<3;i++) {
            if (desiredDevices.at(i)==QBluetoothAddress(QStringLiteral("EE:5D:EE:37:DE:25")))
            device3 = new QLowEnergyController(desiredDevices.at(i));
        }
        device3->setParent(this);
        connect(device3, &QLowEnergyController::connected, this, [&](){
            ok_cardio=1;
           
            qDebug() << "*********** Device 3 Polar H10 connected" << device3->remoteAddress();
            device3->discoverServices();
        });

        connect(device3, &QLowEnergyController::disconnected, this, [&](){
            ok_cardio=0;
           
            qDebug() << "*********** Device 3 Disconnected";
            QTimer::singleShot(1000, this, [&](){
                qDebug() << "Reconnecting device 3";
                device3->connectToDevice();
            });
        });

        connect(device3, &QLowEnergyController::discoveryFinished, this, [&](){
           qDebug() <<  "*********** Device 3 discovery finished";
           setupNotificationCardio(device3, QStringLiteral("Device 3"));
        });

        device3->connectToDevice();
    }
}

void ConcurrentBtle::setupNotificationLeft(QLowEnergyController *device, const QString &name)
{

    if (!device)
        return;

//    //hook up force sensor
    QLowEnergyService *service = device->createServiceObject(QBluetoothUuid(QStringLiteral("7f510001-1b15-11e5-b60b-1697f925ec7b")));
    if (!service) {
        qDebug() << "***********" << name << "force service not found";
        return;
    }

    qDebug() << "#####" << name << service->serviceName() << service->serviceUuid();

    connect(service, &QLowEnergyService::stateChanged,
            this, [name, service](QLowEnergyService::ServiceState s){
        if (s == QLowEnergyService::ServiceDiscovered) {
            qDebug() << "***********" << name << "force service discovered" << service->serviceUuid();
            const QLowEnergyCharacteristic tempData = service->characteristic(QBluetoothUuid(QStringLiteral("7f510019-1b15-11e5-b60b-1697f925ec7b")));
//            const QLowEnergyCharacteristic tempConfig = service->characteristic(
//                        QBluetoothUuid(QStringLiteral("f000aa02-0451-4000-b000-000000000000")));
//            if (!tempData.isValid() || !tempConfig.isValid()) {
//                qDebug() << "***********" << name << "force char not valid";
//                return;
//            }

            if (!tempData.isValid()) {
                qDebug() << "***********" << name << "force char not valid";
                return;
            }

            const QLowEnergyDescriptor notification = tempData.descriptor(
                        QBluetoothUuid(QBluetoothUuid::ClientCharacteristicConfiguration));
            if (!notification.isValid()) {
                qDebug() << "***********" << name << "force notification not valid";
                return;
            }

//            if (tempConfig.value() != QByteArray::fromHex("01"))
//                service->writeCharacteristic(tempConfig, QByteArray::fromHex("01"));
//            service->writeDescriptor(notification, QByteArray::fromHex("0100"));

            service->writeDescriptor(notification, QByteArray::fromHex("0100"));

        }
    });


    connect(service, &QLowEnergyService::characteristicChanged,
            this, [name](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {

//        if (characteristic.uuid() != QBluetoothUuid(QStringLiteral("f510019-1b15-11e5-b60b-1697f925ec7b")))
//            return;


        const char *data = newValue.constData();
        //valid data set count U8
        quint8 flags = data[3];
//        qDebug() << "Valid data set count" << flags;


 //prendo il primo valore che mi invia per calcolare il power output
 //prendo forza tangenziale e la moltiplica per la lunghezza della pedivella (ottengo la coppia) e poi lo moltiplico per la cadenza
               qint16 *TFpo = (qint16 *) &data[8];
                    //qDebug() << "TF value:" << (double)(*TF)/10 << "N";
                double f=(double)(*TFpo)/10;
                qint16 *cadpo = (qint16 *) &data[14];
        //            qDebug() << "Cadence value:" << (double)(*cad)/1024*30/3.14 <<"rpm";
                double c=(double)(*cadpo)/1024;
                qint16 *angpo = (qint16 *) &data[12];
        //            qDebug() << "Cadence value:" << (double)(*cad)/1024*30/3.14 <<"rpm";
                double a=(double)(*angpo);

                double pol=f*c*170/1000;


//                poweroutputL(f,c,a);

//                qDebug() << "Power Output Left:" << pol <<"W";

        for(int k = 1; k <= flags; k++) {
            quint32 *Time = (quint32 *) &data[4+(12*(k-1))];
//            qDebug() << "Time Left value:" << (double)(*Time)/32768.0 << "s";
            double time_value=(double)(*Time)/32768.0;
            qint16 *TF = (qint16 *) &data[8+(12*(k-1))];
            //qDebug() << "TF Left value:" << (double)(*TF)/10 << "N";
            double Tforce_value=(double)(*TF)/10;
            qint16 *RF = (qint16 *) &data[10+(12*(k-1))];
//            qDebug() << "RF Left value:" << (double)(*RF)/10 << "N";
            double Rforce_value=(double)(*RF)/10;
            quint16 *A = (quint16 *) &data[12+(12*(k-1))];
            double angle_value=(double)(*A);
            qint16 *cad = (qint16 *) &data[14+(12*(k-1))];
//            qDebug() << "Cadence Left value:" << (double)(*cad)/1024*30/3.14 <<"rpm";
            double cadence_value=(double)(*cad)/1024*30/3.14;
            double avel_value=(double)(*cad)/1024;


            double ped_eff_left=(Tforce_value)/sqrt((pow(Tforce_value,2)+pow(Rforce_value,2)));

            poweroutputL(Tforce_value,avel_value,angle_value);
            
            writeFileLeft(time_value, Tforce_value, Rforce_value, angle_value,cadence_value,ped_eff_left);


        }




    });
   service->discoverDetails();
}


void ConcurrentBtle::setupNotificationRight(QLowEnergyController *device, const QString &name)
{

    if (!device)
        return;

//    //hook up force sensor
    QLowEnergyService *service = device->createServiceObject(QBluetoothUuid(QStringLiteral("7f510001-1b15-11e5-b60b-1697f925ec7b")));
    if (!service) {
        qDebug() << "***********" << name << "force service not found";
        return;
    }

    qDebug() << "#####" << name << service->serviceName() << service->serviceUuid();

    connect(service, &QLowEnergyService::stateChanged,
            this, [name, service](QLowEnergyService::ServiceState s){
        if (s == QLowEnergyService::ServiceDiscovered) {
            qDebug() << "***********" << name << "force service discovered" << service->serviceUuid();
            const QLowEnergyCharacteristic tempData = service->characteristic(QBluetoothUuid(QStringLiteral("7f510019-1b15-11e5-b60b-1697f925ec7b")));     
//            const QLowEnergyCharacteristic tempConfig = service->characteristic(
//                        QBluetoothUuid(QStringLiteral("f000aa02-0451-4000-b000-000000000000")));
//            if (!tempData.isValid() || !tempConfig.isValid()) {
//                qDebug() << "***********" << name << "force char not valid";
//                return;
//            }

            if (!tempData.isValid()) {
                qDebug() << "***********" << name << "force char not valid";
                return;
            }

            const QLowEnergyDescriptor notification = tempData.descriptor(
                        QBluetoothUuid(QBluetoothUuid::ClientCharacteristicConfiguration));
            if (!notification.isValid()) {
                qDebug() << "***********" << name << "force notification not valid";
                return;
            }

//            if (tempConfig.value() != QByteArray::fromHex("01"))
//                service->writeCharacteristic(tempConfig, QByteArray::fromHex("01"));
//            service->writeDescriptor(notification, QByteArray::fromHex("0100"));

            service->writeDescriptor(notification, QByteArray::fromHex("0100"));

        }
    });


    connect(service, &QLowEnergyService::characteristicChanged,
            this, [name](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {

//        if (characteristic.uuid() != QBluetoothUuid(QStringLiteral("f510019-1b15-11e5-b60b-1697f925ec7b")))
//            return;

        const char *data = newValue.constData();
        //valid data set count U8
        quint8 flags = data[3];
//        qDebug() << "Valid data set count" << flags;

//prendo il primo valore che mi invia per calcolare il power output
//prendo forza tangenziale e la moltiplica per la lunghezza della pedivella (ottengo la coppia) e poi lo moltiplico per la cadenza
        qint16 *TFpo = (qint16 *) &data[8];
//            qDebug() << "TF value:" << (double)(*TF)/10 << "N";
        double f=(double)(*TFpo)/10;
        qint16 *cadpo = (qint16 *) &data[14];
//            qDebug() << "Cadence value:" << (double)(*cad)/1024*30/3.14 <<"rpm";
        double c=(double)(*cadpo)/1024;
        qint16 *angpo = (qint16 *) &data[12];
//            qDebug() << "Cadence value:" << (double)(*cad)/1024*30/3.14 <<"rpm";
        double a=(double)(*angpo);
        double por=f*c*170/1000;
//        qDebug() << "Power Output Right:" << por <<"W";
//        poweroutputR(f,c,a);


        for(int k = 1; k <= flags; k++) {
            quint32 *Time = (quint32 *) &data[4+(12*(k-1))];
//            qDebug() << "Time Right value:" << (float)(*Time)/32768 << "s";
            double time_value=(double)(*Time)/32768.0;
            qint16 *TF = (qint16 *) &data[8+(12*(k-1))];
//            qDebug() << "TF value:" << (double)(*TF)/10 << "N";
            double Tforce_value=(double)(*TF)/10;
            qint16 *RF = (qint16 *) &data[10+(12*(k-1))];
           //qDebug() << "RF Left value:" << (double)(*RF)/10 << "N";
            double Rforce_value=(double)(*RF)/10;
            quint16 *A = (quint16 *) &data[12+(12*(k-1))];

            //qDebug() << "Angular value:" << (double)(*A)<<"°";
            double angle_value=(double)(*A);
            qint16 *cad = (qint16 *) &data[14+(12*(k-1))];
//            qDebug() << "Cadence value:" << (double)(*cad)/1024*30/3.14 <<"rpm";
            double cadence_value=(double)(*cad)/1024*30/3.14;
            double avel_value=(double)(*cad)/1024;
//            write_angle_pedals_right(angle_value);


            double ped_eff_right=(Tforce_value)/sqrt((pow(Tforce_value,2)+pow(Rforce_value,2)));

            poweroutputR(Tforce_value,avel_value,angle_value);
            
        }




    });
   service->discoverDetails();
}





void ConcurrentBtle::setupNotificationCardio(QLowEnergyController *device, const QString &name)
{

    if (!device)
        return;

    QLowEnergyService *service = device->createServiceObject(QBluetoothUuid::HeartRate);
    if (!service) {
        qDebug() << "***********" << name << "cardio service not found";
        return;
    }

    qDebug() << "#####" << name << service->serviceName() << service->serviceUuid();

    connect(service, &QLowEnergyService::stateChanged,
            this, [name, service](QLowEnergyService::ServiceState s){
        if (s == QLowEnergyService::ServiceDiscovered) {
            qDebug() << "***********" << name << "cardio service discovered" << service->serviceUuid();
            const QLowEnergyCharacteristic tempData = service->characteristic(QBluetoothUuid::HeartRateMeasurement);

            if (!tempData.isValid()) {
                qDebug() << "***********" << name << "cardio char not valid";
                return;
            }

            const QLowEnergyDescriptor notification = tempData.descriptor(
                        QBluetoothUuid(QBluetoothUuid::ClientCharacteristicConfiguration));
            if (!notification.isValid()) {
                qDebug() << "***********" << name << "cardio notification not valid";
                return;
            }

            service->writeDescriptor(notification, QByteArray::fromHex("0100"));

        }
    });


    connect(service, &QLowEnergyService::characteristicChanged,
            this, [name](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {

        int a = (newValue.size());
        qDebug() << "Size:" << a;

        const char *data = newValue.constData();

        quint8 flags = data[0];


        //HR 8bit
        quint8 *heartrate= (quint8 *) &data[1];
        qDebug() << "HR value" << name << *heartrate <<"bpm" ;
        double hr_value=*heartrate;
        writeFileCardio(hr_value);
        




    });
   service->discoverDetails();
}




