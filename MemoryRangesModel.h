#ifndef __MEMORYRANGESMODEL_H__
#define __MEMORYRANGESMODEL_H__

#include "MemoryRange.h"
#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include <qsharedpointer.h>

class MemoryRangesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryRangesModel();
    ~MemoryRangesModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    int columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex & index) const ;

    void addMemoryRange(const MemoryRange &memoryRange);
    void removeMemoryRange(int index);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setMemorySections(const QList<MemoryRange> &sections);
    QList<MemoryRange> getMemorySections();

    void update();
signals:
    void sectionsChanged();

private:
//    MapData *m_mapData;
    QList<MemoryRange> m_memoryRanges;
};

#endif // __MEMORYRANGESMODEL_H__
