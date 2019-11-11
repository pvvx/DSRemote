#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include "osc_lan.h"

#define TMC_LAN_TIMEOUT   5000 // ms

osclan ocs;

osclan::osclan(QObject *parent) : QObject(parent)
{
    sock = new QTcpSocket(this);
    connect(sock, SIGNAL(connected()), this, SLOT(connected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(sock, SIGNAL(readyRead()), this, SLOT(read_Data()));
    connect(sock, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}

void osclan::connected() {
    qDebug() << "Connected!";
//    sock->write("?");
}

void osclan::disconnected() {
    qDebug() << "Disconnected!";
}

void osclan::bytesWritten(qint64 bytes) {
//    qDebug() << "we wrote: " << bytes;
}

void osclan::readyRead() {
    qDebug() << "Reading...";
//    qDebug() << sock->readAll();
}

int osclan::open(const char * host_or_ip) {
    sock->close();
    qDebug() << "Connecting to " << host_or_ip << "...";
    sock->connectToHost(host_or_ip, 5555);
    if(!sock->waitForConnected(TMC_LAN_TIMEOUT)) {
        qDebug() << "Error: " << sock->errorString();
        return -1;
    }
    sock->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    sock->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    return 0;
}

void osclan::close(void) {
    sock->close();
}

int osclan::write(const char * data, int len) {
    return (int)sock->write(data, (qint64) len);
}

int osclan::read(char * data, int len) {
    int ret;

    if(!sock->waitForReadyRead(7000)) { // ms
        qDebug() << "Error: " << sock->errorString();
        return -1;
    }
    ret = (int)sock->read(data, (qint64) len);
//    qDebug() << "sock->read() = " << ret;
    return ret;
}

void osclan::flush(void) {
    sock->readAll();
    sock->flush();
}

