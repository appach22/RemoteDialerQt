#include "remotedevice.h"

#include <QStringList>

RemoteDevice::RemoteDevice(QObject *parent) :
    QObject(parent)
{
}

RemoteDevice::RemoteDevice(const RemoteDevice & other) :
    QObject(0)
{
    mName   = other.mName;
    mUid    = other.mUid;
    mModel  = other.mModel;
    mHost   = other.mHost;
    mPort   = other.mPort;
    //mType   = other.mType;
}

RemoteDevice & RemoteDevice::operator=(const RemoteDevice & other)
{
    mName   = other.mName;
    mUid    = other.mUid;
    mModel  = other.mModel;
    mHost   = other.mHost;
    mPort   = other.mPort;
    //mType   = other.mType;
    return *this;
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

bool RemoteDevice::operator==(const RemoteDevice & other) const
{
    if (mUid.compare("") && other.mUid.compare(""))
        return !mUid.compare(other.mUid, Qt::CaseInsensitive);
    else
        return !mName.compare(other.mName, Qt::CaseInsensitive);
    //return false;
}

