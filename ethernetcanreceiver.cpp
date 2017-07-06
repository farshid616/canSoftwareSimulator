#include "ethernetcanreceiver.h"

EthernetCanReceiver::EthernetCanReceiver(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"Init EthernetCanReceiver...";
    udpSocket = new QUdpSocket;
    udpSocket->bind(ETHERNET_CAN_RECEIVE_PORT);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    qDebug()<<"coneccted";

}

void EthernetCanReceiver::onReadyRead()
{
    //qDebug()<<"on ready read";
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processDatagram(datagram);
    }
}
