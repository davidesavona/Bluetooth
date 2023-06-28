#ifndef CONCURRENTBTLE_H
#define CONCURRENTBTLE_H

#include <QObject>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <iostream>
#include <QCoreApplication>


using namespace std;




class ConcurrentBtle : public QObject
{
    Q_OBJECT
public:
    explicit ConcurrentBtle(QObject *parent = nullptr);


    void init_sm();


signals:

public slots:
    void startSearch();
    void establishConnection();
    void setupNotificationLeft(QLowEnergyController *device, const QString &name);
//    void setupNotificationCPLeft(QLowEnergyController *device, const QString &name);
    void setupNotificationRight(QLowEnergyController *device, const QString &name);
//    void setupNotificationCPRight(QLowEnergyController *device, const QString &name);
    void setupNotificationCardio(QLowEnergyController *device, const QString &name);

private:
    QBluetoothDeviceDiscoveryAgent *agent = nullptr;
    QList<QBluetoothAddress> desiredDevices;
    QList<QBluetoothDeviceInfo> foundDevices;

    QLowEnergyController *device1 = nullptr;
    QLowEnergyController *device2 = nullptr;
    QLowEnergyController *device3 = nullptr;
};




#endif // CONCURRENTBTLE_H



