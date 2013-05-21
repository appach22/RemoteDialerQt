#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "remotedevice.h"
#include "devicedelegate.h"
#include <QtGui/QtGui>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    devices = new RemoteDialerDevices();
    devices->setParentView(ui->lvDevices);
    QFile cacheFile(QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0] + DEVICES_FILE_NAME);
    if (cacheFile.open(QIODevice::ReadOnly))
    {
        QDataStream in(&cacheFile);
        in >> *devices;
        devices->validateModel();
    }

    DeviceDelegate* delegate = new DeviceDelegate();
    ui->lvDevices->setItemDelegate(delegate);

    broadcastSocket = new QUdpSocket(this);
    broadcastSocket->bind(QHostAddress::Any, RDIALER_SERVICE_PORT);
    connect(broadcastSocket, SIGNAL(readyRead()), SLOT(receiveBroadcast()));
    commandSocket = new QTcpSocket(this);
    connect(commandSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(commandSocket, SIGNAL(connected()),
            this, SLOT(sendRequest()));
    connect(commandSocket, SIGNAL(readyRead()),
            this, SLOT(receiveReply()));
}

MainWindow::~MainWindow()
{
    delete ui;

    QString path(QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0]);
    QDir().mkpath(path);
    QFile cacheFile(path + DEVICES_FILE_NAME);
    if (cacheFile.open(QIODevice::WriteOnly))
    {
        QDataStream out(&cacheFile);
        out << *devices;
    }
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
        devices->addDevice(*device);
    }
}

void MainWindow::socketError(QAbstractSocket::SocketError)
{
    QMessageBox::information(this, tr("Connection error"), commandSocket->errorString());
    if (commandSocket->state() != QAbstractSocket::UnconnectedState)
        commandSocket->disconnectFromHost();
}

void MainWindow::sendRequest()
{
    QString request("DialNumber " + ui->edtNumber->text() + "\n");
    commandSocket->write(request.toUtf8());
}

void MainWindow::receiveReply()
{
    QString reply(commandSocket->readLine());
    qDebug() << "Got reply: " << reply;
    if (!reply.startsWith("Accepted", Qt::CaseInsensitive))
        QMessageBox::information(this, tr("Dialing error"), tr("Device reported") + " " + reply);
    commandSocket->disconnectFromHost();
}

void MainWindow::dialNumber()
{
    RemoteDevice device = devices->at(ui->lvDevices->currentIndex().row());
    if (ui->edtNumber->text().length() < 3)
        return;
    qDebug() << "Dialing on " << device.mName;
    commandSocket->connectToHost(device.mHost, device.mPort);
}

void MainWindow::numberChanged(QString _number)
{
    if (_number.length() < 3)
        ui->btnDial->setEnabled(false);
    else
        ui->btnDial->setEnabled(true);
}

