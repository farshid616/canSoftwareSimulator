#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ethernetcansender.h"
#include "canethernetframe.h"
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    initUi();
    isPaused = false;
    bmsReceiver = new BMSReceiver;
    connect(bmsReceiver,SIGNAL(bmsDataReceived(BmsCanPacket)),this,SLOT(onBmsDataReceived(BmsCanPacket)));
    connect(bmsReceiver,SIGNAL(packetReceived(QByteArray)),this,SLOT(onPacketReceived(QByteArray)));
    connect(bmsReceiver,SIGNAL(allPacketReceived(QString)),this,SLOT(onAllPacketReceived(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBmsDataReceived(BmsCanPacket bmsPacket)
{
    ui->packetVisualizer->setPlainText(bmsPacket.toString());
}

void MainWindow::onPacketReceived(QByteArray msg)
{
    qDebug()<<"Packet Received";
    ui->packetVisualizer->setPlainText(msg);
    packetAnalyzer(msg);

}

void MainWindow::on_sendbutton_clicked()
{
    EthernetCanSender canSender;
    QString frame = "{007,232,001,";
    if(ui->measurementQueryRadio->isChecked())
    {
        QString id = ui->deviceIdBox->text();
        if(id == "")
        {
            QMessageBox::critical(this,"Error","Please Fill the device id!",QMessageBox::Ok);
            return;
        }
        frame.append("20");
        frame.append(id);
        frame.append(",1,10}");
        canSender.sendPacket(frame);
        ui->sentPacket->setText(frame);
        ui->msgStatus->setText("Mesurement Query Sent");

    }
    else if(ui->measurementCapacityRadio->isChecked())
    {
        QString id = ui->deviceIdBox->text();
        if(id == "")
        {
            QMessageBox::critical(this,"Error","Please Fill the device id!",QMessageBox::Ok);
            return;
        }
        frame.append("20");
        frame.append(id);
        frame.append(",1,11}");
        canSender.sendPacket(frame);
        ui->sentPacket->setText(frame);
        ui->msgStatus->setText("Mesurement Capacity Sent");
    }
    else if(ui->pingRadio->isChecked())
    {
        frame.append("200,1,12}");
        canSender.sendPacket(frame);
        ui->sentPacket->setText(frame);
        ui->msgStatus->setText("ping Sent");
    }
    else if(ui->setParametersRadio->isChecked())
    {
        QString id = ui->deviceIdBox->text();
        QString voltage = ui->VoltageBox->text();
        QString current = ui->CurrentBox->text();
        if(id == "")
        {
            QMessageBox::critical(this,"Error","Please Fill the device id!",QMessageBox::Ok);
            return;
        }
        else if(voltage == "")
        {
            QMessageBox::critical(this,"Error","Please Fill the voltage!",QMessageBox::Ok);
            return;
        }
        else if(current == "")
        {
            QMessageBox::critical(this,"Error","Please Fill the current!",QMessageBox::Ok);
            return;
        }
        threeByte voltageB,currentB;
        voltageB = byteShift(QString::number(voltage.toFloat()*10),8,"LEFT");
        currentB = byteShift(QString::number(current.toFloat()*10),8,"LEFT");
        frame.append("20");
        frame.append(id);
        frame.append(",5,13,");
        frame.append(voltageB.first);
        //frame.append(",");
        frame.append(voltageB.seccond);
        frame.append(",");
        frame.append(currentB.first);
        //frame.append(",");
        frame.append(currentB.seccond);
        frame.append("}");
        canSender.sendPacket(frame);
        ui->sentPacket->setText(frame);
        ui->msgStatus->setText("setting parameter Sent");
    }
    else if(ui->rawFrameRadio->isChecked())
    {
        QString msg = ui->msgEditor->text();
        if(msg == "")
        {
            QMessageBox::critical(this,"Error","Please Fill the packet msg!",QMessageBox::Ok);
            return;
        }
        canSender.sendPacket(msg);
        ui->sentPacket->setText(msg);
        ui->msgStatus->setText("Msg Sent");
    }

}

void MainWindow::statusClear()
{
    sleep(3);

    ui->msgStatus->setText("");
}

void MainWindow::on_pauseButton_clicked()
{
    if(isPaused)
    {
        isPaused = false;
        ui->pauseButton->setText("Pause");
    }
    else
    {
        isPaused = true;
        ui->pauseButton->setText("Play");
    }

}

void MainWindow::onAllPacketReceived(QString msg)
{
    if(ui->RepeatRequest->isChecked())
    {
        sendRepeatReq();
    }
    if(!isPaused)
    {
        ui->allPacketVisualizer->append(msg);
    }
}

void MainWindow::initUi()
{
    ui->deviceIdBox->setDisabled(true);
    ui->VoltageBox->setDisabled(true);
    ui->CurrentBox->setDisabled(true);
    ui->VoltageViewerBox->setDisabled(true);
    ui->CurrentViewerBox->setDisabled(true);
    ui->Temprature1ViewerBox->setDisabled(true);
    ui->temprature2ViewerBox->setDisabled(true);
    ui->capacityViewerBox->setDisabled(true);
    ui->exceptionViewerBox->setDisabled(true);
    ui->msgEditor->setDisabled(true);
    ui->deviceIdViewerBox->setDisabled(true);
}

void MainWindow::on_measurementQueryRadio_toggled(bool checked)
{
    if(checked)
    {
        ui->deviceIdBox->setDisabled(false);
        ui->VoltageBox->setDisabled(true);
        ui->CurrentBox->setDisabled(true);
        ui->msgEditor->setDisabled(true);
    }
}

void MainWindow::on_measurementCapacityRadio_toggled(bool checked)
{
    if(checked)
    {
        ui->deviceIdBox->setDisabled(false);
        ui->VoltageBox->setDisabled(true);
        ui->CurrentBox->setDisabled(true);
        ui->msgEditor->setDisabled(true);
    }

}

void MainWindow::on_pingRadio_toggled(bool checked)
{
    if(checked)
    {
        ui->deviceIdBox->setDisabled(true);
        ui->VoltageBox->setDisabled(true);
        ui->CurrentBox->setDisabled(true);
        ui->msgEditor->setDisabled(true);
    }
}

void MainWindow::on_setParametersRadio_toggled(bool checked)
{
    if(checked)
    {
        ui->deviceIdBox->setDisabled(false);
        ui->VoltageBox->setDisabled(false);
        ui->CurrentBox->setDisabled(false);
        ui->msgEditor->setDisabled(true);
    }
}

void MainWindow::on_rawFrameRadio_toggled(bool checked)
{
    if(checked)
    {
        ui->deviceIdBox->setDisabled(true);
        ui->VoltageBox->setDisabled(true);
        ui->CurrentBox->setDisabled(true);
        ui->msgEditor->setDisabled(false);
    }
}

void MainWindow::stringToHex(QString msg, QString *h, QString *l)
{
    int hi = 0,lo = 0;
    QStringList parsedStringList = msg.trimmed().split(".");
    if(parsedStringList.size()>1)
    {
        hi = parsedStringList.at(0).toInt();
        lo = parsedStringList.at(1).toInt();
    }
    else if(parsedStringList.size()>0)
    {
        hi = parsedStringList.at(0).toInt();
    }
    *h = QString().number(hi, 16).prepend("0x");
    *l = QString().number(lo, 16).prepend("0x");
}

void MainWindow::on_turnOffButton_clicked()
{
    EthernetCanSender canSender;
    QString msg = "{007,232,001,20";
    QString id = ui->deviceIdBox->text();
    if(id == "")
    {
        QMessageBox::critical(this,"Error","Please Fill the device id!",QMessageBox::Ok);
        return;
    }
    msg.append(id);
    //msg.append(",4,14,0x45,0x65,0xA0}");
    msg.append(",4,144565a0}");
    canSender.sendPacket(msg);
    ui->sentPacket->setText(msg);
    ui->msgStatus->setText("turn off Sent");
}

void MainWindow::on_turnOnButton_clicked()
{
    EthernetCanSender canSender;
    QString msg = "{007,232,001,20";
    QString id = ui->deviceIdBox->text();
    if(id == "")
    {
        QMessageBox::critical(this,"Error","Please Fill the device id!",QMessageBox::Ok);
        return;
    }
    msg.append(id);
    //msg.append(",4,14,0x45,0x65,0x0A}");
    msg.append(",5,1445650a}");
    canSender.sendPacket(msg);
    ui->sentPacket->setText(msg);
    ui->msgStatus->setText("turn on Sent");
}

void MainWindow::packetAnalyzer(QByteArray packet)
{
//{507,232,4,309,6,14,00,04,00,00,01,00,0A}
    CanEthernetFrame canEthernetFrame = CanEthernetFrame::deserialize(packet);
    QString rawpacket(packet);
    if(canEthernetFrame.getCommandType() == MEASUREMENT_FRAME)
    {
        //CanPacket canPacket = CanPacket::deserialize(canEthernetFrame.getRawData());
        QStringList parsedStringList = rawpacket.trimmed().remove('{').remove('}').split(",");
        qDebug()<<"address ============= "<<parsedStringList.at(5);
        if(parsedStringList.size() > 4 && parsedStringList.at(5) == "10")
        {
            //Measurement
            QString temp;
            ui->deviceIdViewerBox->setText(parsedStringList.at(3));
            temp.append(parsedStringList.at(6));
            temp.append(parsedStringList.at(7));
            temp = QString::number(temp.toInt(0,16));
            ui->VoltageViewerBox->setText(temp);
            temp.clear();
            temp.append(parsedStringList.at(8));
            temp.append(parsedStringList.at(9));
            temp = QString::number(temp.toInt(0,16));
            ui->CurrentViewerBox->setText(temp);
            ui->Temprature1ViewerBox->setText(parsedStringList.at(10));
            ui->temprature2ViewerBox->setText(parsedStringList.at(11));
        }
        else if(parsedStringList.size() > 4 && parsedStringList.at(5) == "11")
        {
            QString temp;
            temp.append(parsedStringList.at(6));
            temp.append(parsedStringList.at(7));
            temp.append(parsedStringList.at(8));
            temp = QString::number(temp.toInt(0,16));
            ui->deviceIdViewerBox->setText(parsedStringList.at(3));
            ui->capacityViewerBox->setText(temp);
            ui->exceptionViewerBox->setText(parsedStringList.at(9));

        }
        else if(parsedStringList.size() > 4 && parsedStringList.at(5) == "12")
        {
            ui->deviceIdViewerBox->setText(parsedStringList.at(3));

        }
    }


}

threeByte MainWindow::byteShift(QString num, int byteShift,QString direction)
{
    threeByte byte;
    bool ok;
    if(direction == "LEFT")
    {
        int nValue = num.toInt();
        quint8 seccondbyte = ((nValue & 0xff00) >> byteShift);
        quint8 firstbyte = (nValue & 0x00ff);
        byte.seccond = QString::number(firstbyte,16);
        byte.first = QString::number(seccondbyte,16);
        return byte;
    }
    else if(direction == "RIGHT")
    {
        int nValue = num.toInt(&ok,16);
        quint8 seccondbyte = ((nValue & 0xff00) << byteShift);
        quint8 firstbyte = (nValue & 0x00ff);
        byte.seccond = QString::number(firstbyte,16);
        byte.first = QString::number(seccondbyte,16);
        return byte;
    }


    return byte;
}

void MainWindow::sendRepeatReq()
{
    emit on_sendbutton_clicked();
}
