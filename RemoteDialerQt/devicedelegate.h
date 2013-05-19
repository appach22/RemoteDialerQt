#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H

#include <QStyledItemDelegate>

class DeviceDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
public:
    DeviceDelegate();
};

#endif // DEVICEDELEGATE_H
