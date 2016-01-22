#include "MemoryRange.h"

MemoryRange::MemoryRange()
{

}

MemoryRange::~MemoryRange()
{

}
int MemoryRange::lowerLimit() const
{
    return m_lowerLimit;
}

void MemoryRange::setLowerLimit(int lowerLimit)
{
    m_lowerLimit = lowerLimit;
}
int MemoryRange::upperLimit() const
{
    return m_upperLimit;
}

void MemoryRange::setUpperLimit(int upperLimit)
{
    m_upperLimit = upperLimit;
}
QString MemoryRange::name() const
{
    return m_name;
}

void MemoryRange::setName(const QString &name)
{
    m_name = name;
}

void MemoryRange::readFromJsonObject(const QJsonObject &json)
{
    m_name = json["name"].toString();
    m_lowerLimit = json["lowerLimit"].toInt();
    m_upperLimit = json["upperLimit"].toInt();
}

void MemoryRange::writeToJsonObject(QJsonObject &json) const
{
    json["name"] = m_name;
    json["lowerLimit"] = m_lowerLimit;
    json["upperLimit"] = m_upperLimit;
}

bool operator!=(const MemoryRange &lhs, const MemoryRange &rhs)
{
    return !operator==(lhs,rhs);
}

bool operator==(const MemoryRange &lhs, const MemoryRange &rhs)
{
    if (lhs.lowerLimit() != rhs.lowerLimit()) return false;
    if (lhs.upperLimit() != rhs.upperLimit()) return false;
    if (lhs.name() != rhs.name()) return false;
    return true;
}


bool operator<(const MemoryRange &lhs, const MemoryRange &rhs)
{
    if(lhs.lowerLimit() < rhs.lowerLimit())
    {
        return true;
    }
    return false;
}
