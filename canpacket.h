#ifndef CANPACKET_H
#define CANPACKET_H

#include <QStringList>

typedef unsigned char byte;

class CanPacket {

public:
    static CanPacket deserialize(QStringList rawData, bool *ok = 0 ) {
        CanPacket canPacket;
        canPacket.reserved_ = rawData.at(0).toShort();
        if(rawData.size() >= 4) {
            canPacket.address_ = rawData.at(1).toShort(0,16);
            canPacket.length_ = rawData.at(2).toShort();
            canPacket.id_ = rawData.at(3).toShort(0,16);
            if(ok!=NULL) {
                *ok = true;
            }
            return canPacket;
        }
        else {
            if(ok != NULL) {
                *ok = false;
            }
            return CanPacket();
        }
    }
//{507,232,4,308,8,12,01,01,00,00,00,00,00}
    QString toString() {
        return QString("\t\tCanPacket:\n\t\tReserved: "+QString::number(reserved_)
                       +"\n\t\taddress: "+QString::number(address_)
                       +"\n\t\tlength: "+QString::number(length_)+
                       +"\n\t\tid: "+QString::number(id_));
    }

    short address() const
    {
        return address_;
    }


private:
    short reserved_;
    short address_;
    byte length_;
    byte id_;
};
#endif // CANPACKET_H

