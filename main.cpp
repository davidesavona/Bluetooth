#include <QObject>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QtEndian>
#include <fstream>
#include <QtCore/QtAlgorithms>
#include <QtCore>
#include <math.h>
#include <unistd.h>
#include "concurrentbtle.h"
#include <QtCore>
#include <QtCore/QCoreApplication>
//#include "shared_memory.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ConcurrentBtle btle;

    return a.exec();

}