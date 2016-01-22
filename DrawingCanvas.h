#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QList>
#include <QWidget>
#include <QPainter>
#include <QSvgGenerator>
#include "MemoryRange.h"
#include "MapData.h"

class DrawingCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingCanvas(QWidget *parent = 0);
    ~DrawingCanvas();

    void paintEvent(QPaintEvent *);

//    void setMemorySections(QList<MemoryRange> memorySections);

    void setMemorySize(int memorySize);

    void setMapData(MapData &mapData);


    void drawMemorySection(QPainter &painter, float startRange, float endRange, QString name);

    void exportToSvg(QString path);
    void exportToPng(QString path);
    void paint(QPainter &painter);
    int fieldWidth() const;
    void setFieldWidth(int fieldWidth);

    bool showAddressLabelLeft() const;
    bool showAddressLabelRight() const;
    QString prefix() const;
    QString suffix() const;

    int width() const;
    int height() const;
signals:

//public slots:
//    void setShowAddressLabelLeft(bool showAddressLabelLeft);
//    void setShowAddressLabelRight(bool showAddressLabelRight);
//    void setShowAddressPrefix(bool isShowAddressPrefixChecked);
//    void setShowAddressSuffix(bool isShowAddressSuffixChecked);
//    void setSuffix(const QString &suffix);
//    void setPrefix(const QString &prefix);
//    void setWidth(int width);
//    void setHeight(int height);
private:

    QString getPrefix();
    QString getSuffix();

    QSize calculateCanvasSize();

//    QList<MemoryRange> m_memorySections;

//    int m_width;
//    int m_height;
//    int m_fontSize;
//    QPoint m_boxOffset;
//    int m_addressXOffset;
//    int m_maxAddress;
//    int m_fieldWidth;

//    bool m_showAddressLabelLeft;
//    bool m_showAddressLabelRight;

//    bool m_showPrefix;
//    bool m_showSuffix;

//    QString m_prefix;
//    QString m_suffix;

    QList<int> m_edges;

    MapData m_mapData;
};

#endif // DRAWINGCANVAS_H
