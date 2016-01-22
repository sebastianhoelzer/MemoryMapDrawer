#ifndef __MEMORYRANGE_H__
#define __MEMORYRANGE_H__

#include <QString>
#include <QJsonObject>

class MemoryRange
{
public:
    MemoryRange();
    ~MemoryRange();

    int lowerLimit() const;
    void setLowerLimit(int lowerLimit);

    int upperLimit() const;
    void setUpperLimit(int upperLimit);

    QString name() const;
    void setName(const QString &name);

    void readFromJsonObject(const QJsonObject &json);
    void writeToJsonObject(QJsonObject &json) const;

private:
    int m_lowerLimit;
    int m_upperLimit;
    QString m_name;
};

bool operator==(const MemoryRange& lhs, const MemoryRange& rhs);
bool operator!=(const MemoryRange& lhs, const MemoryRange& rhs);
bool operator<(const MemoryRange& lhs, const MemoryRange& rhs);

#endif // __MEMORYRANGE_H__
