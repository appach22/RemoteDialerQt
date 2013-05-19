#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "remotedevice.h"
#include "devicedelegate.h"
#include <QtGui/QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    devices = new RemoteDialerDevices();
    devices->setParentView(ui->lvDevices);
    DeviceDelegate* delegate = new DeviceDelegate();
    ui->lvDevices->setItemDelegate(delegate);
    broadcastSocket = new QUdpSocket(this);
    broadcastSocket->bind(QHostAddress::Any, RDIALER_SERVICE_PORT);
    connect(broadcastSocket, SIGNAL(readyRead()), SLOT(receiveBroadcast()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDigit()
{
    QString digit = sender()->objectName().split("_")[1];
    if (digit == "10") digit = "*";
    else if (digit == "11") digit = "#";
    ui->edtNumber->setText(ui->edtNumber->text() + digit);
}

void MainWindow::receiveBroadcast()
{
    QByteArray datagram;
    datagram.resize(broadcastSocket->pendingDatagramSize());
    QHostAddress *address = new QHostAddress();
    broadcastSocket->readDatagram(datagram.data(), datagram.size(), address);

    QDataStream in(&datagram, QIODevice::ReadOnly);
    QString request(in.device()->readLine());
    qDebug() << "Got broadcast " << request;
    if (request.startsWith("DeviceInfo"))
    {
        RemoteDevice * device = new RemoteDevice();
        device->InitFromBroadcast(&request, address, RDIALER_SERVICE_PORT);
        devices->addDevice(device);
    }
}
