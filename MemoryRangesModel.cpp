#include "MemoryRangesModel.h"
#include "MemoryRange.h"
#include <QString>
#include <QList>
#include <qsharedpointer.h>

MemoryRangesModel::MemoryRangesModel()
{
}

MemoryRangesModel::~MemoryRangesModel()
{
}

int MemoryRangesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_memoryRanges.count();
}

int MemoryRangesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant MemoryRangesModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            return m_memoryRanges.at(index.row()).name();
        }
        else if(index.column() == 1)
        {
            return m_memoryRanges.at(index.row()).lowerLimit();
        }
        else if(index.column() == 2)
        {
            return m_memoryRanges.at(index.row()).upperLimit();
        }
    }
    return QVariant();
}

bool MemoryRangesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            m_memoryRanges[index.row()].setName(value.toString());
        }
        else if(index.column() == 1)
        {
             m_memoryRanges[index.row()].setLowerLimit(value.toInt());
        }
        else if(index.column() == 2)
        {
             m_memoryRanges[index.row()].setUpperLimit(value.toInt());
        }

        emit sectionsChanged();
    }
    return true;
}

Qt::ItemFlags MemoryRangesModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

void MemoryRangesModel::addMemoryRange(const MemoryRange &memoryRange)
{
    beginResetModel();
    m_memoryRanges.append(memoryRange);
    qSort(m_memoryRanges);
    endResetModel();
    emit sectionsChanged();
}

void MemoryRangesModel::removeMemoryRange(int index)
{
    beginResetModel();
    m_memoryRanges.removeAt(index);
    endResetModel();
    emit sectionsChanged();
}

QVariant MemoryRangesModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return QString("Name");
    }
    if (section == 1 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return QString("Lower limit");
    }
    if (section == 2 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return QString("Upper limit");
    }

    return QVariant();
}

void MemoryRangesModel::setMemorySections(const QList<MemoryRange> &sections)
{
    beginResetModel();
    m_memoryRanges = sections;
    endResetModel();
    emit sectionsChanged();
}

QList<MemoryRange> MemoryRangesModel::getMemorySections()
{
    return m_memoryRanges;
}

void MemoryRangesModel::update()
{
    emit sectionsChanged();
}

