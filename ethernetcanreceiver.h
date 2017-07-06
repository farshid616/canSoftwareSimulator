#ifndef ETHERNETCANRECEIVER_H
#define ETHERNETCANRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>


#define ETHERNET_CAN_RECEIVE_PORT 56801

class EthernetCanReceiver : public QObject
{
    Q_OBJECT
public:
    explicit EthernetCanReceiver(QObject *parent = 0);

protected:
    virtual bool processDatagram(QByteArray byteArray) = 0;

signals:

private slots:

    virtual void onReadyRead();

private:
    const QString TAG = "EthernetCanReceiver";
    QUdpSocket *udpSocket;

};

#endif // ETHERNETCANRECEIVER_H
