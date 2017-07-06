#ifndef BMSRECEIVER_H
#define BMSRECEIVER_H

#include "ethernetcanreceiver.h"
#include "bmscanpacket.h"
#include <QTimer>

class BMSReceiver : public EthernetCanReceiver
{
    Q_OBJECT
public:
    BMSReceiver(QObject *parent = 0);

    BmsCanPacket bmsCanPacket() const;
    bool isBmsDataAvailable(){
        return dataAvailable_;
    }

public slots:
    void onNoDataTimerTriggered();

protected:
    bool processDatagram(QByteArray byteArray);

signals:
    void bmsDataReceived(BmsCanPacket bmsCanPacket);
    void packetReceived(QByteArray msg);
    void allPacketReceived(QString msg);

private:
    BmsCanPacket bmsCanPacket_;
    bool dataAvailable_ = false;
    QTimer noDataTimer_;
};

#endif // BMSRECEIVER_H
