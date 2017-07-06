#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bmsreceiver.h"
#include "bmscanpacket.h"


struct threeByte{
    QString first;
    QString seccond;
    QString three;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void statusClear();

private:
    Ui::MainWindow *ui;
    BMSReceiver *bmsReceiver;
    bool isPaused;

private slots:
    void onBmsDataReceived(BmsCanPacket bmsPacket);
    void onPacketReceived(QByteArray msg);
    void on_sendbutton_clicked();
    void on_pauseButton_clicked();
    void onAllPacketReceived(QString msg);
    void on_measurementQueryRadio_toggled(bool checked);

    void on_measurementCapacityRadio_toggled(bool checked);

    void on_pingRadio_toggled(bool checked);

    void on_setParametersRadio_toggled(bool checked);

    void on_rawFrameRadio_toggled(bool checked);

    void on_turnOffButton_clicked();

    void on_turnOnButton_clicked();

private:
    void initUi();
    void stringToHex(QString msg, QString *h, QString *l);
    void packetAnalyzer(QByteArray packet);
    threeByte byteShift(QString num, int byteShift, QString direction);
    void sendRepeatReq();
};

#endif // MAINWINDOW_H
