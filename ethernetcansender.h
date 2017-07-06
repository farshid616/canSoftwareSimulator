#ifndef ETHERNETCANSENDER_H
#define ETHERNETCANSENDER_H

#include <QtNetwork>
#include <QObject>
class QUdpSocket;

class EthernetCanSender: public QObject
{
public:
    EthernetCanSender();
    EthernetCanSender(QObject *parent);
    ~EthernetCanSender();
public slots:
    void sendPacket(QString msg);
private:
        QUdpSocket *udpSocket;
};

#endif // ETHERNETCANSENDER_H


