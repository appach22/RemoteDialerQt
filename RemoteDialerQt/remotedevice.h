#ifndef REMOTEDEVICE_H
#define REMOTEDEVICE_H

#include <QObject>
#include <QHostAddress>

class RemoteDevice : public QObject
{
    Q_OBJECT
public:
    explicit RemoteDevice(QObject *parent = 0);
    RemoteDevice(const RemoteDevice & other);
    RemoteDevice & operator=(const RemoteDevice & other);
    bool operator==(const RemoteDevice & other) const;
    RemoteDevice * InitFromBroadcast(QString * infoFromPacket, QHostAddress * deviceIP, int devicePort);

    QString mName;
    // Не требуется для PC, т.к. устройства могут быть только сетевыми, локального быть не может
    //int mType;
    QString mHost;
    int mPort;
    QString mModel;
    QString mUid;

signals:
    
public slots:

private:
    
};

QDataStream &operator<<(QDataStream &out, RemoteDevice device);
QDataStream &operator>>(QDataStream &in, RemoteDevice &device);
//Q_DECLARE_METATYPE(RemoteDevice*)

#endif // REMOTEDEVICE_H
