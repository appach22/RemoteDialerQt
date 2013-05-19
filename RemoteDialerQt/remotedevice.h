#ifndef REMOTEDEVICE_H
#define REMOTEDEVICE_H

#include <QObject>
#include <QHostAddress>

class RemoteDevice : public QObject
{
    Q_OBJECT
public:
    explicit RemoteDevice(QObject *parent = 0);
    QString mName;
    // Не требуется для PC, т.к. устройства могут быть только сетевыми, локального быть не может
    //int mType;
    QString mHost;
    int mPort;
    QString mModel;
    QString mUid;

    RemoteDevice * InitFromBroadcast(QString * infoFromPacket, QHostAddress * deviceIP, int devicePort);

signals:
    
public slots:
    
};

#endif // REMOTEDEVICE_H
