#ifndef REMOTEDIALERDEVICES_H
#define REMOTEDIALERDEVICES_H

#include <QList>
#include <QStandardItemModel>
#include <QListView>

#include "remotedevice.h"

class RemoteDialerDevices : public QList<RemoteDevice *>, QStandardItemModel
{
    //Q_OBJECT
public:
    //explicit RemoteDialerDevices(RemoteDialerDevices * parent = 0);
    void addDevice(RemoteDevice * _device);

    void setParentView(QAbstractItemView * _view);

signals:
    
public slots:

private:

    QAbstractItemView * parentView;
    
};

#endif // REMOTEDIALERDEVICES_H
