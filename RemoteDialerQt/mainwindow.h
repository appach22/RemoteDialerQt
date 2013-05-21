#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTcpSocket>

#include "remotedialerdevices.h"


#define RDIALER_SERVICE_PORT    52836
#define DEVICES_FILE_NAME       "/devices.cache"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QUdpSocket * broadcastSocket;
    QTcpSocket * commandSocket;
    RemoteDialerDevices * devices;

private slots:
    void receiveBroadcast();
    void socketError(QAbstractSocket::SocketError);
    void sendRequest();
    void receiveReply();
    void dialNumber();
    void numberChanged(QString _number);

public slots:
    void addDigit();

};

#endif // MAINWINDOW_H
