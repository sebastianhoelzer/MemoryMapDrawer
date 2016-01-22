#ifndef MAPDATA_H
#define MAPDATA_H

#include <QString>
#include <QList>
#include <QPoint>
#include <QString>

#include "MemoryRange.h"

class MapData
{
public:
    MapData();

    int fontSize() const;
    void setFontSize(int fontSize);

    QList<MemoryRange> memorySections() const;
    void setMemorySections(const QList<MemoryRange> &memorySections);

    void clearMemorySections();
    void addMemorySection(MemoryRange &memorySection);
    void removeMemorySection(int index);

    QString name() const;
    void setName(const QString &name);

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    QPoint boxOffset() const;
    void setBoxOffset(const QPoint &boxOffset);
    void setBoxOffsetX(int x);
    void setBoxOffsetY(int y);

    int addressXOffset() const;
    void setAddressXOffset(int addressXOffset);

    int maxAddress() const;
    void setMaxAddress(int maxAddress);

    int fieldWidth() const;
    void setFieldWidth(int fieldWidth);

    bool showAddressLabelLeft() const;
    void setShowAddressLabelLeft(bool showAddressLabelLeft);

    bool showAddressLabelRight() const;
    void setShowAddressLabelRight(bool showAddressLabelRight);

    bool showPrefix() const;
    void setShowPrefix(bool showPrefix);

    bool showSuffix() const;
    void setShowSuffix(bool showSuffix);

    QString prefix() const;
    void setPrefix(const QString &prefix);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    QString toString();

private:

    QList<MemoryRange> m_memorySections;

    QString m_name;
    int m_width;
    int m_height;
    int m_fontSize;
    QPoint m_boxOffset;
    int m_addressXOffset;
    int m_maxAddress;
    int m_fieldWidth;

    bool m_showAddressLabelLeft;
    bool m_showAddressLabelRight;

    bool m_showPrefix;
    bool m_showSuffix;

    QString m_prefix;
    QString m_suffix;
};

bool operator==(const MapData& lhs, const MapData& rhs);
bool operator!=(const MapData& lhs, const MapData& rhs);

#endif // MAPDATA_H
