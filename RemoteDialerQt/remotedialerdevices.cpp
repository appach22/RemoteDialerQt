#include "remotedialerdevices.h"

/*RemoteDialerDevices::RemoteDialerDevices(RemoteDialerDevices * parent) :
    QList(*parent)
{
}*/

void RemoteDialerDevices::addDevice(const RemoteDevice & _device)
{
    // Если список уже содержит это устройство - обновляем его в списке (на случай, если порт или имя поменялись)
    QStandardItem * item = NULL;
    int index = indexOf(_device);
    if (index != -1)
    {
        // Удаляем уже существующее устройство
        removeAt(index);
        if (index < rowCount())
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

void RemoteDialerDevices::validateModel()
{
    dynamic_cast<QStandardItemModel *>(this)->clear();
    for (int i = 0; i < size(); ++i)
    {
        QStandardItem * item = new QStandardItem();
        item->setText(QString() + "<b>" + at(i).mName + "</b><br/>" + at(i).mModel);
        appendRow(item);
    }
    parentView->setModel(this);
}

void RemoteDialerDevices::setParentView(QAbstractItemView * _view)
{
    parentView = _view;
}
