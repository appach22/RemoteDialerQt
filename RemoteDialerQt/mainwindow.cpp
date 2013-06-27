#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "remotedevice.h"
#include "devicedelegate.h"
#include <QtGui/QtGui>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isDialing = false;

//    QFile style("style.qss");
//    style.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(style.readAll());
//    qApp->setStyleSheet(styleSheet);

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

    connect(ui->lvDevices->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), SLOT(selectionChanged()));

    broadcastSocket = new QUdpSocket(this);
    broadcastSocket->bind(QHostAddress::Any, RDIALER_SERVICE_PORT);
    connect(broadcastSocket, SIGNAL(readyRead()), SLOT(receiveBroadcast()));
    commandSocket = new QTcpSocket(this);
    connect(commandSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(commandSocket, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(commandSocket, SIGNAL(readyRead()), this, SLOT(receiveReply()));
    checkSocket = new QTcpSocket(this);
    connect(checkSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(checkSocket, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(checkSocket, SIGNAL(readyRead()), this, SLOT(receiveReply()));

    checkTimer.setSingleShot(true);
    connect(&checkTimer, SIGNAL(timeout()), this, SLOT(connectionTimeout()));

    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface iface, ifaces)
    {
        QList<QNetworkAddressEntry> addresses = iface.addressEntries();
        foreach(QNetworkAddressEntry entry, addresses)
            broadcastAddresses.append(entry.broadcast());
    }
    qDebug() << broadcastAddresses;
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
    ui->lblStatus->clear();
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
    if (sender() == commandSocket)
    {
        QMessageBox::information(this, tr("Connection error"), commandSocket->errorString());
        if (commandSocket->state() != QAbstractSocket::UnconnectedState)
            commandSocket->disconnectFromHost();
        isDialing = false;
        ui->btnDial->setEnabled(true);
    }
    else if (sender() == checkSocket)
    {
        //markAsUnavailable();
        checkNextDeviceAvailability();
    }
}

void MainWindow::sendRequest()
{
    if (sender() == commandSocket)
    {
        QString request("DialNumber " + ui->edtNumber->text() + "\n");
        commandSocket->write(request.toUtf8());
    }
    else if (sender() == checkSocket)
    {
        QString request("CheckAvailability\n");
        QByteArray arr = request.toUtf8();
        checkSocket->write(request.toUtf8());
    }
}

void MainWindow::receiveReply()
{
    if (sender() == commandSocket)
    {
        QString reply(commandSocket->readLine());
        qDebug() << "Got reply: " << reply;
        if (!reply.startsWith("Accepted", Qt::CaseInsensitive))
        {
            ui->lblStatus->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 63, 63)");
            ui->lblStatus->setText(tr("Dialing error:") + reply);
        }
        else
        {
            ui->lblStatus->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(63, 255, 63)");
            ui->lblStatus->setText(tr("Number dialed successfully."));
        }
        commandSocket->disconnectFromHost();
        isDialing = false;
        ui->btnDial->setEnabled(true);
    }
    else if (sender() == checkSocket)
    {
        QString reply(checkSocket->readLine());
        qDebug() << "Got check reply: " << reply;
        if (reply.startsWith("Accepted", Qt::CaseInsensitive))
        {;}//markAsAvailable();
        else
        {;}//markAsUnavailable();
        checkSocket->disconnectFromHost();
        checkNextDeviceAvailability();
    }
}

void MainWindow::dialNumber()
{
    if (isDialing)
        return;

    if (ui->edtNumber->text().length() < 3)
    {
        ui->lblStatus->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 63, 63)");
        ui->lblStatus->setText(tr("Number is too short"));
        return;
    }

    if (ui->lvDevices->currentIndex().row() == -1)
    {
        ui->lblStatus->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 63, 63)");
        ui->lblStatus->setText(tr("Device not selected"));
        return;
    }

    RemoteDevice device = devices->at(ui->lvDevices->currentIndex().row());
    qDebug() << "Dialing on " << device.mName;
    ui->btnDial->setEnabled(false);
    isDialing = true;
    ui->lblStatus->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(63, 255, 63)");
    ui->lblStatus->setText(tr("Dialing number..."));
    QTimer::singleShot(7000, this, SLOT(connectionTimeout()));
    commandSocket->connectToHost(device.mHost, device.mPort);
}

void MainWindow::numberChanged(QString _number)
{
    if (_number.length() < 3)
        ui->btnDial->setEnabled(false);
    else
        ui->btnDial->setEnabled(true);
    if (!isDialing)
        ui->lblStatus->clear();
}

void MainWindow::connectionTimeout()
{
    if (sender() == &checkTimer)
    {
        checkSocket->abort();
        //markAsUnavailable();
        checkNextDeviceAvailability();
    }
    else
    {
        commandSocket->abort();
        ui->lblStatus->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(255, 63, 63)");
        ui->lblStatus->setText(tr("Dialing error!"));
        isDialing = false;
        ui->btnDial->setEnabled(true);
    }
}

void MainWindow::selectionChanged()
{
    if (!isDialing)
        ui->lblStatus->clear();
}

void MainWindow::searchForDevices()
{
    QByteArray request = QString("GetDeviceInfo\n").toUtf8();
    foreach(QHostAddress address, broadcastAddresses)
        broadcastSocket->writeDatagram(request.data(), address, RDIALER_SERVICE_PORT);

    if (devices->size() == 0)
        return;

    ui->lblSearch->setText(tr("Searching for devices..."));
    currentCheckIndex = 0;
    qDebug() << "Checking device " << currentCheckIndex;
    checkTimer.start(5000);
    RemoteDevice device = devices->at(currentCheckIndex);
    checkSocket->connectToHost(device.mHost, device.mPort);
}

void MainWindow::checkNextDeviceAvailability()
{
    currentCheckIndex++;
    if (currentCheckIndex < devices->size())
    {
        qDebug() << "Checking device " << currentCheckIndex;
        checkTimer.start(5000);
        RemoteDevice device = devices->at(currentCheckIndex);
        checkSocket->connectToHost(device.mHost, device.mPort);
    }
    else
    {
        checkTimer.stop();
        ui->lblSearch->clear();
    }

}


bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::FileOpen)
    {
        QFileOpenEvent* fileEvent = static_cast<QFileOpenEvent*>(event);
        if (!fileEvent->url().isEmpty())
        {
            QString lastUrl = fileEvent->url().toString();
            QString number = fileEvent->url().path().split(":").at(1);
            emit gotNumber(number);
            //qDebug() << lastUrl;
            //qDebug() << fileEvent->url().host() << fileEvent->url().port() << fileEvent->url().path() << fileEvent->url().query() << fileEvent->url().scheme();
            //QMessageBox::information(this, QString("URL opened"), lastUrl);
            //emit urlOpened(m_lastUrl);
        }
        else if (!fileEvent->file().isEmpty())
        {
            QMessageBox::information(this, QString("File opened"), fileEvent->file());
//            emit fileOpened(fileEvent->file());
        }

        return false;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
