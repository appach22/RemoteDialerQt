#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

#include "remotedialerdevices.h"


#define RDIALER_SERVICE_PORT 52836

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
    RemoteDialerDevices * devices;

private slots:
    void receiveBroadcast();

public slots:
    void addDigit();

};

#endif // MAINWINDOW_H
