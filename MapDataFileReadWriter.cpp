#include "MapDataFileReadWriter.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

MapDataFileReadWriter::MapDataFileReadWriter()
{
}

void MapDataFileReadWriter::writeFile(QString path, MapData mapData)
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject mapDataObject;
    mapDataObject["name"] = mapData.name();
    mapDataObject["width"] = mapData.width();
    mapDataObject["height"] = mapData.height();
    mapDataObject["showPrefix"] = mapData.showPrefix();
    mapDataObject["showSuffix"] = mapData.showSuffix();
    mapDataObject["prefix"] = mapData.prefix();
    mapDataObject["suffix"] = mapData.suffix();
    mapDataObject["showAddressLabelLeft"] = mapData.showAddressLabelLeft();
    mapDataObject["showAddressLabelRight"] = mapData.showAddressLabelRight();
    mapDataObject["addressXOffset"] = mapData.addressXOffset();
    mapDataObject["boxOffsetX"] = mapData.boxOffset().x();
    mapDataObject["boxOffsetY"] = mapData.boxOffset().y();
    mapDataObject["fieldWidth"] = mapData.fieldWidth();
    mapDataObject["fontSize"] = mapData.fontSize();
    mapDataObject["maxAddress"] = mapData.maxAddress();

    QJsonArray memorySectionsArray;
    foreach (const MemoryRange section, mapData.memorySections()) {
        QJsonObject sectionObject;
        section.writeToJsonObject(sectionObject);
        memorySectionsArray.append(sectionObject);
    }
    mapDataObject["memorySections"] = memorySectionsArray;

    QJsonDocument mapDataDocument(mapDataObject);
    file.write(mapDataDocument.toJson());

    file.close();
}

MapData MapDataFileReadWriter::readFile(QString path)
{
    QFile loadFile(path);
    MapData loadedMapData;

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return loadedMapData;
    }

    QByteArray loadData = loadFile.readAll();

    QJsonDocument loadDocument(QJsonDocument::fromJson(loadData));

    QJsonObject rootObject = loadDocument.object();

    loadedMapData.setName(rootObject["name"].toString());
    loadedMapData.setWidth(rootObject["width"].toInt());
    loadedMapData.setHeight(rootObject["height"].toInt());
    loadedMapData.setPrefix(rootObject["prefix"].toString());
    loadedMapData.setSuffix(rootObject["suffix"].toString());
    loadedMapData.setShowPrefix(rootObject["showPrefix"].toBool());
    loadedMapData.setShowSuffix(rootObject["showSuffix"].toBool());
    loadedMapData.setShowAddressLabelLeft(rootObject["showAddressLabelLeft"].toBool());
    loadedMapData.setShowAddressLabelRight(rootObject["showAddressLabelRight"].toBool());
    loadedMapData.setAddressXOffset(rootObject["addressXOffset"].toInt());
    loadedMapData.setBoxOffsetX(rootObject["boxOffsetX"].toInt());
    loadedMapData.setBoxOffsetY(rootObject["boxOffsetY"].toInt());
    loadedMapData.setFieldWidth(rootObject["fieldWidth"].toInt());
    loadedMapData.setFontSize(rootObject["fontSize"].toInt());
    loadedMapData.setMaxAddress(rootObject["maxAddress"].toInt());

    QList<MemoryRange> loadedSections;
    QJsonArray sectionsArray = rootObject["memorySections"].toArray();

    for (int index = 0; index < sectionsArray.size(); ++index)
    {
        QJsonObject sectionObject = sectionsArray[index].toObject();
        MemoryRange memorySection;
        memorySection.readFromJsonObject(sectionObject);
        loadedSections.append(memorySection);
    }

    loadedMapData.setMemorySections(loadedSections);
    qDebug() << loadedMapData.toString();
    return loadedMapData;
}
