#include "ethernetcansender.h"
#include <QtNetwork>


EthernetCanSender::EthernetCanSender()
{
    qDebug()<<"sender";
    udpSocket = new QUdpSocket(this);
    //connect(udpSocket, SIGNAL(connected()), this, SLOT(sendPacket()));
}

EthernetCanSender::~EthernetCanSender()
{
    udpSocket->close();
}

void EthernetCanSender::sendPacket(QString msg)
{
    QByteArray datagram = msg.toStdString().c_str();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress("192.168.1.101"), 56800);
                             //QHostAddress::Broadcast, 56800);
    qDebug()<<"packet sent";
}




