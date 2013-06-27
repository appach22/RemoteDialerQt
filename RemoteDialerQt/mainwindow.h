#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTimer>

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
    QTcpSocket * dialSocket;
    QTcpSocket * checkSocket;
    RemoteDialerDevices * devices;
    bool isDialing;
    int currentCheckIndex;
    QList<QHostAddress> broadcastAddresses;
    QTimer dialTimer;
    QTimer checkTimer;

    void checkNextDeviceAvailability();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void receiveBroadcast();
    void socketError(QAbstractSocket::SocketError);
    void sendRequest();
    void receiveReply();
    void dialNumber();
    void numberChanged(QString _number);
    void connectionTimeout();
    void selectionChanged();
    void searchForDevices();

public slots:
    void addDigit();

signals:
    void gotNumber(QString number);

};

#endif // MAINWINDOW_H
