#ifndef ETHERNETPACKET_H
#define ETHERNETPACKET_H
#include <QStringList>
#include <QDebug>

#define MEASUREMENT_FRAME 507
#define MAGIC_PACKET_FRAME 510

class CanEthernetFrame {
public:

    static CanEthernetFrame deserialize(QByteArray byteArray, bool *ok = 0) {
        if(byteArray == NULL){
            qDebug()<<" given byte array is null";
            if(ok!=NULL) {
                *ok = false;
            }
            return CanEthernetFrame();
        }

        QString rawString(byteArray);
        if(rawString.at(0) != '{' || rawString.at(rawString.length()-1) != '}') {
            qDebug()<<" malformed ethernet packet(header or footer does not exist)!";
            if(ok!=NULL) {
                *ok = false;
            }
            return CanEthernetFrame();
        }

        QStringList parsedStringList = rawString.trimmed().remove('{').remove('}').split(",");
        if(parsedStringList.length() < 3) {
            qDebug()<<" invalid length of packet parts!";
            if(ok!=NULL) {
                *ok = false;
            }
            return CanEthernetFrame();
        }


        CanEthernetFrame ethernetPacket;

        ethernetPacket.commandType_ = parsedStringList.at(0).toShort();
        ethernetPacket.address_ = parsedStringList.at(1).toShort();
        for(int index=2;index<parsedStringList.size();index++){
            ethernetPacket.rawData_.append(parsedStringList.at(index));
        }
        if(ok!=NULL) {
            *ok = true;
        }
        return ethernetPacket;
    }

    QString toString() {
        QString string = QString("EthernetPacket:"
                                 "\nCommand Type: "+QString::number(commandType_)+
                                 "\nAddress: "+QString::number(address_));
        string+="\nRaw Data: ";
        for(int index = 0;index<rawData_.size();index++) {
            string+=rawData_.at(index)+",";
        }
        return string;
    }

    QStringList getRawData() const {
        return rawData_;
    }

    short getCommandType() const {
        return commandType_;
    }

    short address() const
    {
        return address_;
    }

private:
    short commandType_;
    short address_;
    QStringList rawData_;
};


#endif // ETHERNETPACKET_H
