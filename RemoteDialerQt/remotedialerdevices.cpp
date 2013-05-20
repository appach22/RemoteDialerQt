#include "remotedialerdevices.h"

/*RemoteDialerDevices::RemoteDialerDevices(RemoteDialerDevices * parent) :
    QList(*parent)
{
}*/

void RemoteDialerDevices::addDevice(RemoteDevice & _device)
{
    // Если список уже содержит это устройство - обновляем его в списке (на случай, если порт или имя поменялись)
    QStandardItem * item = NULL;
    int index = indexOf(_device);
    if (index != -1)
    {
        // Удаляем уже существующее устройство
        removeAt(index);
        item = takeRow(index).at(0);
    }
    // Добавляем новое и сообщаем GUI, что надо обновить список
    append(_device);
    if (!item)
        item = new QStandardItem();
    item->setText(QString() + "<b>" + _device.mName + "</b><br/>" + _device.mModel);
    appendRow(item);
    parentView->setModel(this);
}

void RemoteDialerDevices::setParentView(QAbstractItemView * _view)
{
    parentView = _view;
}
