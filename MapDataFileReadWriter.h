#ifndef MAPDATAFILEREADWRITER_H
#define MAPDATAFILEREADWRITER_H

#include <QString>

#include "MapData.h"

class MapDataFileReadWriter
{
public:
    MapDataFileReadWriter();

    void writeFile(QString path, MapData mapData);
    MapData readFile(QString path);
};

#endif // MAPDATAFILEREADWRITER_H
