#include "remotedevice.h"

#include <QStringList>

RemoteDevice::RemoteDevice(QObject *parent) :
    QObject(parent)
{
}

RemoteDevice * RemoteDevice::InitFromBroadcast(QString * infoFromPacket, QHostAddress * deviceIP, int devicePort)
{
    QStringList infos = infoFromPacket->split("|");
    mName = infos[1];
    //mType = DEVICE_TYPE_LOCAL_NETWORK;
    mUid = infos[2];
    mModel = infos[3];
    mHost = deviceIP->toString();
    mPort = devicePort;
    return this;
}
