#ifndef BMSCANPACKET_H
#define BMSCANPACKET_H
#include "canpacket.h"

#define BMS_PACKET_LENGTH 11
#define BMS_MAX_CELLS_NUM 16
#define BMS_MAX_TEMPERATURE 3
#define BMS_CELL_VOLTAGE_WARN_UPPER_VALUE 4.3
#define BMS_CELL_VOLTAGE_WARN_LOWER_VALUE 2.25


class BmsCanPacket
{
public:

    BmsCanPacket() {
        for(int index = 0; index < BMS_MAX_CELLS_NUM; index++) {
            cells[index] = 0;
        }
    }

    static BmsCanPacket deserialize(QStringList rawData, bool *ok = 0) {
        bool isOk = false;
        CanPacket canPacket = CanPacket::deserialize(rawData,&isOk);
        if(!isOk || rawData.length() != BMS_PACKET_LENGTH){
            if(ok != NULL){
                *ok = false;
            }
            return BmsCanPacket();
        }
        BmsCanPacket bmsCanPacket;
        bmsCanPacket.header = canPacket;
        bmsCanPacket.mode = rawData.at(4).toShort(0,16);
        if(bmsCanPacket.mode < 5) {
            int index = (bmsCanPacket.mode-1)*3;

            for(int cell_counter = 5;cell_counter<11;cell_counter+=2) {
                bmsCanPacket.cells[index++] = QString(rawData[cell_counter]+rawData[cell_counter+1]).toInt(0,16);
            }
        }
        if(ok != NULL){
            *ok = true;
        }
        //temperature can be handled
        return bmsCanPacket;
    }

    static bool deserialize(BmsCanPacket &bmsCanPacket, QStringList rawData, int startingCellIndex = 0) {
        bool isOk = false;
        bmsCanPacket.header = CanPacket::deserialize(rawData,&isOk);
        if(!isOk || rawData.length() != BMS_PACKET_LENGTH){
            return false;
        }

        bmsCanPacket.mode = rawData.at(4).toShort(0,16);
        if(bmsCanPacket.mode < 5) {
            int index = startingCellIndex+(bmsCanPacket.mode-1)*3;
            for(int cell_counter = 5;cell_counter<11;cell_counter+=2) {
                bmsCanPacket.cells[index++] = QString(rawData[cell_counter]+rawData[cell_counter+1]).toInt(0,16);
            }
        }
        //temperature can be handled
        return true;
    }

    QString toString(){

        QString cellsStr;
        for(int i=0;i<BMS_MAX_CELLS_NUM;i++){
            cellsStr+="["+QString::number(cells[i])+"],";
        }
        return QString("BMS CAN PACKET:"
                       "\n\tHeader:\n"+header.toString()
                       +"\n\tMode: "+QString::number(mode)
                       +"\n\tCells: "+cellsStr);
    }

    const int* getCells() const {
        return cells;
    }

    QString getCellsStr(int numOfCells) {
        QString cellsStr;
        if(numOfCells > BMS_MAX_CELLS_NUM) {
            numOfCells = BMS_MAX_CELLS_NUM;
        }
        for(int i=0;i<numOfCells;i++){
            cellsStr+="["+QString::number(cells[i])+"],";
        }
        return cellsStr;
    }

    CanPacket getHeader() const
    {
    return header;
    }


private:
    CanPacket header;
    byte mode;
    int cells[BMS_MAX_CELLS_NUM];
    int temperature[BMS_MAX_TEMPERATURE];
};
#endif // BMSCANPACKET_H
