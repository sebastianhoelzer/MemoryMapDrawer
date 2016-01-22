#include "MapData.h"
#include <QSharedPointer>

MapData::MapData() :
    m_name("Untitled"),
    m_width(150),
    m_height(350),
    m_fontSize(14),
    m_boxOffset(QPoint(300, 30)),
    m_addressXOffset(20),
    m_maxAddress(511),
    m_fieldWidth(2),
    m_showAddressLabelLeft(true),
    m_showAddressLabelRight(false),
    m_showPrefix(true),
    m_showSuffix(false),
    m_prefix("0x"),
    m_suffix("h")
{
}

int MapData::fontSize() const
{
    return m_fontSize;
}

void MapData::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
}

QList<MemoryRange> MapData::memorySections() const
{
    return m_memorySections;
}

void MapData::setMemorySections(const QList<MemoryRange> &memorySections)
{
    m_memorySections = memorySections;
}

void MapData::clearMemorySections()
{
    m_memorySections.clear();
}

void MapData::addMemorySection(MemoryRange &memorySection)
{
    m_memorySections.append(memorySection);
}

void MapData::removeMemorySection(int index)
{
    m_memorySections.removeAt(index);
}

QString MapData::name() const
{
    return m_name;
}

void MapData::setName(const QString &name)
{
    m_name = name;
}
int MapData::width() const
{
    return m_width;
}

void MapData::setWidth(int width)
{
    m_width = width;
}
int MapData::height() const
{
    return m_height;
}

void MapData::setHeight(int height)
{
    m_height = height;
}
QPoint MapData::boxOffset() const
{
    return m_boxOffset;
}

void MapData::setBoxOffset(const QPoint &boxOffset)
{
    m_boxOffset = boxOffset;
}

void MapData::setBoxOffsetX(int x)
{
    m_boxOffset.setX(x);
}

void MapData::setBoxOffsetY(int y)
{
    m_boxOffset.setY(y);
}

int MapData::addressXOffset() const
{
    return m_addressXOffset;
}

void MapData::setAddressXOffset(int addressXOffset)
{
    m_addressXOffset = addressXOffset;
}
int MapData::maxAddress() const
{
    return m_maxAddress;
}

void MapData::setMaxAddress(int maxAddress)
{
    m_maxAddress = maxAddress;
}
int MapData::fieldWidth() const
{
    return m_fieldWidth;
}

void MapData::setFieldWidth(int fieldWidth)
{
    m_fieldWidth = fieldWidth;
}
bool MapData::showAddressLabelLeft() const
{
    return m_showAddressLabelLeft;
}

void MapData::setShowAddressLabelLeft(bool showAddressLabelLeft)
{
    m_showAddressLabelLeft = showAddressLabelLeft;
}
bool MapData::showAddressLabelRight() const
{
    return m_showAddressLabelRight;
}

void MapData::setShowAddressLabelRight(bool showAddressLabelRight)
{
    m_showAddressLabelRight = showAddressLabelRight;
}
bool MapData::showPrefix() const
{
    return m_showPrefix;
}

void MapData::setShowPrefix(bool showPrefix)
{
    m_showPrefix = showPrefix;
}
bool MapData::showSuffix() const
{
    return m_showSuffix;
}

void MapData::setShowSuffix(bool showSuffix)
{
    m_showSuffix = showSuffix;
}
QString MapData::prefix() const
{
    return m_prefix;
}

void MapData::setPrefix(const QString &prefix)
{
    m_prefix = prefix;
}
QString MapData::suffix() const
{
    return m_suffix;
}

void MapData::setSuffix(const QString &suffix)
{
    m_suffix = suffix;
}

QString MapData::toString()
{
    QString output;
    output = m_name;
    output.append("\n");
    foreach(const MemoryRange &memorySection, m_memorySections)
    {
         output.append(QString("name: %1 lower:%2 upper:%3")
                       .arg(memorySection.name())
                       .arg(memorySection.lowerLimit())
                       .arg(memorySection.upperLimit()));
    }
    return output;
}

bool operator!=(const MapData &lhs, const MapData &rhs)
{
    return !operator==(lhs,rhs);
}

bool operator==(const MapData &lhs, const MapData &rhs)
{
    if(lhs.memorySections() != rhs.memorySections()) return false;
    if(lhs.name() != rhs.name()) return false;
    if(lhs.width() != rhs.width()) return false;
    if(lhs.height() != rhs.height()) return false;
    if(lhs.fontSize() != rhs.fontSize()) return false;
    if (lhs.boxOffset() != rhs.boxOffset()) return false;
    if(lhs.addressXOffset() != rhs.addressXOffset()) return false;
    if(lhs.maxAddress() != rhs.maxAddress()) return false;
    if(lhs.fieldWidth() != rhs.fieldWidth()) return false;
    if(lhs.showAddressLabelLeft() != rhs.showAddressLabelLeft()) return false;
    if(lhs.showAddressLabelRight() != rhs.showAddressLabelRight()) return false;
    if(lhs.showPrefix() != rhs.showPrefix()) return false;
    if(lhs.showSuffix() != rhs.showSuffix()) return false;
    if(lhs.prefix() != rhs.prefix()) return false;
    if(lhs.suffix() != rhs.suffix()) return false;

    return true;
}
