#include "bmsreceiver.h"
#include "canethernetframe.h"

BMSReceiver::BMSReceiver(QObject *parent):EthernetCanReceiver(parent)
{
//    noDataTimer_.start(3000);
}

bool BMSReceiver::processDatagram(QByteArray byteArray)
{
    emit allPacketReceived(byteArray);
    //qDebug()<<"process";
    CanEthernetFrame canEthernetFrame = CanEthernetFrame::deserialize(byteArray);
    //qDebug()<<"Command type = "<<canEthernetFrame.getCommandType();
    if(canEthernetFrame.getCommandType() == MEASUREMENT_FRAME || canEthernetFrame.getCommandType() == 504 ||
       canEthernetFrame.getCommandType() == 501 || canEthernetFrame.getCommandType() == 511 ||
       canEthernetFrame.getCommandType() == 512)
    {
        qDebug()<<"inja ";
        emit packetReceived(byteArray);
    }
    if(canEthernetFrame.getCommandType() == MEASUREMENT_FRAME) {
        qDebug()<<"mframe";
        CanPacket canPacket = CanPacket::deserialize(canEthernetFrame.getRawData());
        //noDataTimer_.stop();
        dataAvailable_ = true;
        qDebug()<<"deserial =  "<<canPacket.address();
        if(canPacket.address() == 0x3F1) {
            qDebug()<<"First add";
            if(BmsCanPacket::deserialize(bmsCanPacket_,canEthernetFrame.getRawData(),0)){
                emit bmsDataReceived(bmsCanPacket_);
            }
        } else if(canPacket.address() == 0x3F9) {
            qDebug()<<"Seccond add";
            if(BmsCanPacket::deserialize(bmsCanPacket_,canEthernetFrame.getRawData(),12)){
                emit bmsDataReceived(bmsCanPacket_);
            }
        }
        //noDataTimer_.start(3000);
    }


}
BmsCanPacket BMSReceiver::bmsCanPacket() const
{
    return bmsCanPacket_;
}

void BMSReceiver::onNoDataTimerTriggered()
{
   dataAvailable_ = false;
}


